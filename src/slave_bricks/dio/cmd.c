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
 * RX - P1.1
 * TX - P1.2
 *
 * DI1 - P1.0
 * DI2 - P1.3
 * DI3 - P1.4
 * DI4 - P1.5
 *
 * DO1 - P2.0
 * DO2 - P2.1
 * DO3 - P2.2
 * DO4 - P2.3
 *
 * PWR_LED - P2.5
 * ACT_LED - P2.4
 */

#define SERIAL_DEBUG

#ifdef SERIAL_DEBUG
#include "serial.h"
#include "conio.h"
#endif

#include "cmd.h"

/* Commands */
#define CMD_SET_POUT    0x01
#define CMD_GET_PIN     0x02
#define CMD_RESET       0xA0
#define CMD_FW_TYPE		0xF0
#define CMD_FW_VERSION	0xF1

static i2c_cmds cmds = {
     .count = 5,
     .cmds	= {
          {
               .cmd		= CMD_SET_POUT,
			   .args	= 1,
               .func 	= cmd_set_pout,
          },
          {
               .cmd		= CMD_GET_PIN,
			   .args	= 0,
               .func 	= cmd_get_pin,
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

void cmd_set_pout(i2c_cmd_args *args)
{
	unsigned char p2 = (args->args[0] & 0b00001111);

	i2cslave_cmdproc_clrres();

	P2OUT = (0b11110000 & P2OUT) | p2;

#ifdef SERIAL_DEBUG
    cio_printf("%s::P2OUT:", __func__);
	cio_printb(P2OUT, 8);
    cio_printf("\n\r");
#endif
}

void cmd_get_pin(i2c_cmd_args *args)
{
	unsigned char p1 =  0b00111001 & P1IN; 
	unsigned char h  = 0;

	act();

	if((p1 & 0b00000001) == 0b00000001)
	{
		h |= 0b00000001;
	}

	if((p1 & 0b00001000) == 0b00001000)
	{
		h |= 0b00000010;
	}

	if((p1 & 0b00010000) == 0b00010000)
	{
		h |= 0b00000100;
	}

	if((p1 & 0b00100000) == 0b00100000)
	{
		h |= 0b00001000;
	}

	i2cslave_cmdproc_clrres();
	i2cslave_cmdproc_addres(h);

#ifdef SERIAL_DEBUG
    cio_printf("%s::P1IN:", __func__);
	cio_printb(h, 8);
    cio_printf("\n\r");
#endif
}

void cmd_reset(i2c_cmd_args *args)
{
	i2cslave_cmdproc_clrres();

	P1DIR &= ~0b00111001; // inputs
	P1REN |=  0b00111001; // enable int. res for all inputs
	P1OUT |=  0b00111001; // make res. pullup

	P2DIR |=  0b00111111; // outputs
	P2OUT &=  0b00100000; // only enable power-led

	act();

#ifdef SERIAL_DEBUG
	cio_printf("%s\n\r", __func__);
#endif
}

void cmd_get_fwtype(i2c_cmd_args *args)
{
	act();

	// FIXME: add to global defines
	// 0x01 = DIO
	i2cslave_cmdproc_clrres();
	i2cslave_cmdproc_addres(0x01);

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

