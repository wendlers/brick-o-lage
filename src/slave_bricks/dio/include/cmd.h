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

#ifndef __CMD_H__
#define __CMD_H__

#include "i2c.h"

/* I2C slave address (7-bit) */
#ifndef I2C_ADDR
#define I2C_ADDR	0x48
#endif

/* Brick information */
#define FW_TYPE		0x01		// FW-Type (DIO)
#define FW_VERSION	0x01		// FW-Version

void i2c_cmd_init();

void cmd_reset(i2c_cmd_args *args);

void cmd_set_pout(i2c_cmd_args *args);

void cmd_get_pin(i2c_cmd_args *args);

void cmd_get_fwtype(i2c_cmd_args *args);

void cmd_get_fwversion(i2c_cmd_args *args);

#endif
