/*
 * This file is part of the Brick-o-Lage project.
 *
 * Copyright (C) 2014 Stefan Wendler <sw@kaltpost.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <msp430.h>
#include <legacymsp430.h>

/**
 * Pin connection for this brick is:
 *
 * RX 		- P1.1
 * TX 		- P1.2
 *
 * EN_M1	- P2.0
 * EN_M2	- P2.1
 * PWM_M1A	- P1.0
 * PWM_M1B	- P1.3
 * PWM_M2A	- P1.4
 * PWM_M2B 	- P1.5
 *
 * PWR_LED 	- P2.5
 * ACT_LED 	- P2.4
 */

// if thie is defined, debug information will be printed on serial port
#define SERIAL_DEBUG

#ifdef SERIAL_DEBUG
#include "serial.h"
#include "conio.h"
#endif

#include "cmd.h"

/* Commands */
#define CMD_SET_SPEED   0x01				// set speed (DC %) for the motor channels
#define CMD_SET_DIR     0x02				// set motor direction (STOP, CW or CCW)
#define CMD_RESET       0xA0				// reset the brick
#define CMD_FW_TYPE		0xF0				// get firmware-type
#define CMD_FW_VERSION	0xF1				// get firmware-version

/* PWM Settings */
#define PWM_PERIOD		8000				// cycles for one period (~ 2kHz)
#define PWM_DCPERCENT	(PWM_PERIOD / 100)	// cycles for one percent DC
#define	PWM_CCRD		PWM_DCPERCENT		// delta to add to CCR0 reg.

#define DC_DISABLE		0					// no DC for this PWM
#define DC_HIGH			1					// DC HIGH phase
#define DC_LOW			2					// DC LOW phase

static i2c_cmds cmds = {
     .count = 5,
     .cmds	= {
          {
               .cmd		= CMD_SET_SPEED,
			   .args	= 2,
               .func 	= cmd_set_speed,
          },
          {
               .cmd		= CMD_SET_DIR,
			   .args	= 1,
               .func 	= cmd_set_dir,
          },
          {
               .cmd		= CMD_RESET,
			   .args	= 0,
               .func 	= cmd_reset,
          },
          {
               .cmd		= CMD_FW_TYPE,
			   .args	= 0,
               .func 	= cmd_get_fwtype,
          },
          {
               .cmd		= CMD_FW_VERSION,
			   .args	= 0,
               .func 	= cmd_get_fwversion,
          },
 	},
};

typedef struct
{
	// M1A
	int  dc1a;								// targeted duty cycle
	int  dc1a_cur;							// current duty cycle
	unsigned char dc1a_act;					// state: High, Low, Deactivated

	// M1B
	int  dc1b;
	int  dc1b_cur;
	unsigned char dc1b_act;

	// M2A
	int  dc2a;
	int  dc2a_cur;
	unsigned char dc2a_act;

	// M2B
	int  dc2b;
	int  dc2b_cur;
	unsigned char dc2b_act;

} pwm_info;

static pwm_info pwm =
{
	.dc1a  		= PWM_PERIOD,				// 100% DC
	.dc1a_cur	= 0,
	.dc1a_act 	= DC_DISABLE,

	.dc1b  		= PWM_PERIOD,
	.dc1a_cur	= 0,
	.dc1b_act 	= DC_DISABLE,

	.dc2a  		= PWM_PERIOD,
	.dc1a_cur	= 0,
	.dc2a_act 	= DC_DISABLE,

	.dc2b  		= PWM_PERIOD,
	.dc1a_cur	= 0,
	.dc2b_act 	= DC_DISABLE,
};

void act()
{
	P2OUT ^= BIT4; // toggel activity LED
}

void i2c_cmd_init()
{
	i2cslave_cmdproc_init(I2C_ADDR, &cmds);

#ifdef SERIAL_DEBUG
	serial_clk_init(16000000L, 9600);
    cio_printf("\n\r%s\n\r", __func__);	
#endif
	
	cmd_reset(0L);
}

void cmd_set_speed(i2c_cmd_args *args)
{
	// 2 bytes: <addr><speed>
	//
	// speed: 0b0sssssss
	// 	s - speed, 0..100%
	// addr: 0b0000abAB
	//	a - if 1, set speed for speed M1A (CW)
	//	b - if 1, set speed for speed M1B (CCW)
	//	A - if 1, set speed for speed M2A (CW)
	//	B - if 1, set speed for speed M2B (CCW)

	// speed
	unsigned char s = (args->args[0] & 0b01111111);

	// addr
	unsigned char B = (args->args[1] & 0b00000001);
	unsigned char A = (args->args[1] & 0b00000010) >> 1;
	unsigned char b = (args->args[1] & 0b00000100) >> 2;
	unsigned char a = (args->args[1] & 0b00001000) >> 3;

	if(a)
	{
		pwm.dc1a = PWM_DCPERCENT * s;
	}
	if(b)
	{
		pwm.dc1b = PWM_DCPERCENT * s;
	}
	if(A)
	{
		pwm.dc2a = PWM_DCPERCENT * s;
	}
	if(B)
	{
		pwm.dc2b = PWM_DCPERCENT * s;
	}

	i2cslave_cmdproc_clrres();

#ifdef SERIAL_DEBUG
	cio_printf("%s::s=%i;a=%i;b=%i;A=%i;B=%i\n\r", __func__, s, a, b, A, B);
#endif
}

void cmd_set_dir(i2c_cmd_args *args)
{
	// 1 byte: <M1/2>
	//
	// M1/2: 0b0000aabb
	// 	aa - direction M1: 00 - don't change, 11 - stop, 01 - cw, 10 ccw
	// 	bb - direction M2: 00 - don't change, 11 - stop, 01 - cw, 10 ccw

	unsigned char aa = (args->args[0] & 0b00001100) >> 2;
	unsigned char bb = (args->args[0] & 0b00000011);

	if(aa != 0)
	{
		// stop PWM
		pwm.dc1a_cur = 0;
		pwm.dc1b_cur = 0;
		pwm.dc1a_act = DC_DISABLE;
		pwm.dc1b_act = DC_DISABLE;

		// stop motor
		P2OUT &= 0b11111110;	// disable 1,2EN
		P1OUT &= 0b11110110;	// 1A/2A off

		switch(aa)
		{
			case 0b10:
				P1OUT |= 0b00001000;	// on
				pwm.dc1b_act = DC_HIGH;	// start PWM
				break;
			case 0b01:
				P1OUT |= 0b00000001;	// on
				pwm.dc1a_act = DC_HIGH;	// start PWM
				break;
		}

		if(aa != 0b11)
		{
			// restart with new direction if not stopped
			P2OUT |= 0b00000001;	// enable 1,2EN
		}
	}

	if(bb != 0)
	{
		// stop PWM
		pwm.dc2a_cur = 0;
		pwm.dc2b_cur = 0;
		pwm.dc2a_act = DC_DISABLE;
		pwm.dc2b_act = DC_DISABLE;

		// stop motor
		P2OUT &= 0b11111101;	// disable 3,4EN
		P1OUT &= 0b11001111;	// 3A/4A off

		switch(bb)
		{
			case 0b10:
				P1OUT |= 0b00100000;	// on
				pwm.dc2b_act = DC_HIGH;	// start PWM
				break;
			case 0b01:
				P1OUT |= 0b00010000;	// on
				pwm.dc2a_act = DC_HIGH;	// start PWM
				break;
		}

		if(bb != 0b11)
		{
			// restart with new direction if not stopped
			P2OUT |= 0b00000010;	// enable 3,4EN
		}
	}

	i2cslave_cmdproc_clrres();

#ifdef SERIAL_DEBUG
	cio_printf("%s::aa=%i;bb=%i\n\r", __func__, aa, bb);
#endif
}

void cmd_reset(i2c_cmd_args *args)
{
	i2cslave_cmdproc_clrres();

	P1DIR |= 0b00111001; // outputs
	P2DIR |= 0b00110011;

	P2OUT &= 0b00100000; // only enable power-led

	// stop PWM M1
	pwm.dc1a_cur = 0;
	pwm.dc1b_cur = 0;
	pwm.dc1a_act = DC_DISABLE;
	pwm.dc1b_act = DC_DISABLE;

	// stop PWM M2
	pwm.dc2a_cur = 0;
	pwm.dc2b_cur = 0;
	pwm.dc2a_act = DC_DISABLE;
	pwm.dc2b_act = DC_DISABLE;

	CCTL0 = CCIE;                             // CCR0 interrupt enabled

	CCR0  = PWM_CCRD;
	TACTL = TASSEL_2 + MC_2;                  // SMCLK, contmode

	act();

#ifdef SERIAL_DEBUG
	cio_printf("%s\n\r", __func__);
#endif
}

void cmd_get_fwtype(i2c_cmd_args *args)
{
	act();

	i2cslave_cmdproc_clrres();
	i2cslave_cmdproc_addres(FW_TYPE);

#ifdef SERIAL_DEBUG
	cio_printf("%s\n\r", __func__);
#endif
}

void cmd_get_fwversion(i2c_cmd_args *args)
{
	act();

	i2cslave_cmdproc_clrres();
	i2cslave_cmdproc_addres(FW_VERSION);

#ifdef SERIAL_DEBUG
	cio_printf("%s\n\r", __func__);
#endif
}

interrupt(TIMER0_A0_VECTOR) isr_timer_a(void)
{
	/**
	 * PWM for M1A
	 */
	if(pwm.dc1a_act > DC_DISABLE)
	{
		pwm.dc1a_cur += PWM_CCRD;

		// at the moment the HIGH phase ends, we turn off output and start LOW phase
		if(pwm.dc1a_act == DC_HIGH && pwm.dc1a_cur >= pwm.dc1a)
		{
			if(pwm.dc1a < PWM_PERIOD)
			{
				P1OUT &= 0b11111110;	// off
			}
			pwm.dc1a_act = DC_LOW;	// enter LOW phase
		}
		// at the moment the LOW phase ends, we turn ON output and start HIGH phase
		// also a new period is started
		else if(pwm.dc1a_act == DC_LOW && pwm.dc1a_cur >= PWM_PERIOD)
		{
			P1OUT |= 0b00000001;	// on
			pwm.dc1a_act = DC_HIGH;	// enter HIGH phase
			pwm.dc1a_cur = 0;		// start new period
		}
	}

	/**
	 * PWM for M1B
	 */
	if(pwm.dc1b_act > DC_DISABLE)
	{
		pwm.dc1b_cur += PWM_CCRD;

		if(pwm.dc1b_act == DC_HIGH && pwm.dc1b_cur >= pwm.dc1b)
		{
			if(pwm.dc1b < PWM_PERIOD)
			{
				P1OUT &= 0b11110111;	// off
			}
			pwm.dc1b_act = DC_LOW;
		}
		else if(pwm.dc1b_act == DC_LOW && pwm.dc1b_cur >= PWM_PERIOD)
		{
			P1OUT |= 0b00001000;	// on
			pwm.dc1b_act = DC_HIGH;
			pwm.dc1b_cur = 0;
		}
	}

	/**
	 * PWM for M2A
	 */
	if(pwm.dc2a_act > DC_DISABLE) // && pwm.dc2a < PWM_PERIOD)
	{
		pwm.dc2a_cur += PWM_CCRD;

		if(pwm.dc2a_act == DC_HIGH && pwm.dc2a_cur >= pwm.dc2a)
		{
			if(pwm.dc2a < PWM_PERIOD)
			{
				P1OUT &= 0b11101111;	// off
			}
			pwm.dc2a_act = DC_LOW;
		}
		else if(pwm.dc2a_act == DC_LOW && pwm.dc2a_cur >= PWM_PERIOD)
		{
			P1OUT |= 0b00010000;	// on
			pwm.dc2a_act = DC_HIGH;
			pwm.dc2a_cur = 0;
		}
	}

	/**
	 * PWM for M2B
	 */
	if(pwm.dc2b_act > DC_DISABLE) // && pwm.dc2a < PWM_PERIOD)
	{
		pwm.dc2b_cur += PWM_CCRD;

		if(pwm.dc2b_act == DC_HIGH && pwm.dc2b_cur >= pwm.dc2b)
		{
			if(pwm.dc2b < PWM_PERIOD)
			{
				P1OUT &= 0b11011111;	// off
			}
			pwm.dc2b_act = DC_LOW;
		}
		else if(pwm.dc2b_act == DC_LOW && pwm.dc2b_cur >= PWM_PERIOD)
		{
			P1OUT |= 0b00100000;	// on
			pwm.dc2b_act = DC_HIGH;
			pwm.dc2b_cur = 0;
		}
	}

	CCR0 += PWM_CCRD;  // Add Offset to CCR0
}

