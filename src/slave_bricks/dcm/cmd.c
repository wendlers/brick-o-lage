/*
 * This file is part of the mps430-ioexp project.
 *
 * Copyright (C) 2011 Stefan Wendler <sw@kaltpost.de>
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

#define SERIAL_DEBUG

#ifdef SERIAL_DEBUG
#include "serial.h"
#include "conio.h"
#endif

#include "cmd.h"

/* Commands */
#define CMD_SET_SPEED   0x01
#define CMD_SET_DIR     0x02
#define CMD_RESET       0xA0
#define CMD_FW_TYPE		0xF0
#define CMD_FW_VERSION	0xF1

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
	int  dc1a;
	int  dc1a_cur;
	unsigned char dc1a_act;
	int  dc1b;
	int  dc1b_cur;
	unsigned char dc1b_act;
	int  dc2a;
	int  dc2a_cur;
	unsigned char dc2a_act;
	int  dc2b;
	int  dc2b_cur;
	unsigned char dc2b_act;
} pwm_info;

static pwm_info pwm =
{
	.dc1a  		= 16000,
	.dc1a_cur	= 0,
	.dc1a_act 	= 0,
	.dc1b  		= 16000,
	.dc1a_cur	= 0,
	.dc1b_act 	= 0,
	.dc2a  		= 16000,
	.dc1a_cur	= 0,
	.dc2a_act 	= 0,
	.dc2b  		= 16000,
	.dc1a_cur	= 0,
	.dc2b_act 	= 0,
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
	//
	// addr: 0b0000ddmm
	//	m - motor: 00 - ignored, 01 - M1, 10 - M2
	//	d - 00/11 - ignored, 01 - cw, 10 ccw

	// speed
	unsigned char s = (args->args[0] & 0b01111111);

	// addr
	unsigned char m = (args->args[1] & 0b00000011);
	unsigned char d = (args->args[1] & 0b00001100) >> 2;

	if(m == 0x01)
	{
		if(d == 0x01)
		{
			pwm.dc1a = 160 * s;
		}
		else if(d == 0x02)
		{
			pwm.dc1b = 160 * s;
		}
	}
	else if(m == 0x02)
	{
		if(d == 0x01)
		{
			pwm.dc2a = 160 * s;
		}
		else if(d == 0x02)
		{
			pwm.dc2b = 160 * s;
		}
	}

	i2cslave_cmdproc_clrres();

#ifdef SERIAL_DEBUG
	cio_printf("%s::s=%i;m=%i;d=%i\n\r", __func__, s, m, d);
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
		pwm.dc1a_act = 0;
		pwm.dc1b_act = 0;

		// stop motor
		P2OUT &= 0b11111110;	// disable 1,2EN
		P1OUT &= 0b11110110;	// 1A/2A off

		switch(aa)
		{
			case 0b10:
				P1OUT |= 0b00001000;	// on
				pwm.dc1b_act = 1;		// start PWM
				break;
			case 0b01:
				P1OUT |= 0b00000001;	// on
				pwm.dc1a_act = 1;		// start PWM
				break;
		}

		// restart with new direction
		P2OUT |= 0b00000001;	// enable 1,2EN
	}

	if(bb != 0)
	{
		// stop PWM
		pwm.dc2a_cur = 0;
		pwm.dc2b_cur = 0;
		pwm.dc2a_act = 0;
		pwm.dc2b_act = 0;

		// stop motor
		P2OUT &= 0b11111101;	// disable 3,4EN
		P1OUT &= 0b11001111;	// 3A/4A off

		switch(bb)
		{
			case 0b10:
				P1OUT |= 0b00100000;	// on
				pwm.dc2b_act = 1;		// start PWM
				break;
			case 0b01:
				P1OUT |= 0b00010000;	// on
				pwm.dc2a_act = 1;		// start PWM
				break;
		}

		// restart with new direction
		P2OUT |= 0b00000010;	// enable 3,4EN
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

	CCTL0 = CCIE;                             // CCR0 interrupt enabled

	// 16MHz = 16.000.000 cycles / sec.
	// 16.000.000 / 1.000 = 16000 cycles / sec. = 16000 Hz = 16kHz
	CCR0  = 1000;
	TACTL = TASSEL_2 + MC_2;                  // SMCLK, contmode

	act();

#ifdef SERIAL_DEBUG
	cio_printf("%s\n\r", __func__);
#endif
}

void cmd_get_fwtype(i2c_cmd_args *args)
{
	act();

	// FIXME: add to global defines
	// 0x02 = DCM
	i2cslave_cmdproc_clrres();
	i2cslave_cmdproc_addres(0x02);

#ifdef SERIAL_DEBUG
	cio_printf("%s\n\r", __func__);
#endif
}

void cmd_get_fwversion(i2c_cmd_args *args)
{
	act();

	// FIXME: add to global defines
	// current fw-version
	i2cslave_cmdproc_clrres();
	i2cslave_cmdproc_addres(0x01);

#ifdef SERIAL_DEBUG
	cio_printf("%s\n\r", __func__);
#endif
}

interrupt(TIMER0_A0_VECTOR) isr_timer_a(void)
{
	/**
	 * PWM for M1A
	 */
	if(pwm.dc1a_act > 0)
	{
		pwm.dc1a_cur += 1000;

		if(pwm.dc1a_act == 1 && pwm.dc1a_cur >= pwm.dc1a)
		{
			P1OUT &= 0b11111110;	// off
			pwm.dc1a_act = 2;
		}
		else if(pwm.dc1a_act == 2 && pwm.dc1a_cur >= 16000)
		{
			P1OUT |= 0b00000001;	// on
			pwm.dc1a_act = 1;
			pwm.dc1a_cur = 0;
		}
	}

	/**
	 * PWM for M1B
	 */
	if(pwm.dc1b_act > 0)
	{
		pwm.dc1b_cur += 1000;

		if(pwm.dc1b_act == 1 && pwm.dc1b_cur >= pwm.dc1b)
		{
			P1OUT &= 0b11110111;	// off
			pwm.dc1b_act = 2;
		}
		else if(pwm.dc1b_act == 2 && pwm.dc1b_cur >= 16000)
		{
			P1OUT |= 0b00001000;	// on
			pwm.dc1b_act = 1;
			pwm.dc1b_cur = 0;
		}
	}


	/**
	 * PWM for M2A
	 */
	if(pwm.dc2a_act > 0)
	{
		pwm.dc2a_cur += 1000;

		if(pwm.dc2a_act == 1 && pwm.dc2a_cur >= pwm.dc2a)
		{
			P1OUT &= 0b11101111;	// off
			pwm.dc2a_act = 2;
		}
		else if(pwm.dc2a_act == 2 && pwm.dc2a_cur >= 16000)
		{
			P1OUT |= 0b00010000;	// on
			pwm.dc2a_act = 1;
			pwm.dc2a_cur = 0;
		}
	}

	/**
	 * PWM for M2B
	 */
	if(pwm.dc2b_act > 0)
	{
		pwm.dc2b_cur += 1000;

		if(pwm.dc2b_act == 1 && pwm.dc2b_cur >= pwm.dc2b)
		{
			P1OUT &= 0b11011111;	// off
			pwm.dc2b_act = 2;
		}
		else if(pwm.dc2b_act == 2 && pwm.dc2b_cur >= 16000)
		{
			P1OUT |= 0b00100000;	// on
			pwm.dc2b_act = 1;
			pwm.dc2b_cur = 0;
		}
	}

	CCR0 += 1000;  // Add Offset to CCR0
}

