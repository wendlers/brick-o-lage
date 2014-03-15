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

#include <stdio.h>
#include <unistd.h>

#include "diobrick.hpp"

/* Commands */
#define CMD_SET_POUT    0x01
#define CMD_GET_PIN     0x02

bol::DioBrick::DioBrick(Brick *brick) : bol::Brick(brick)
{
	pout = 0x00;
}

bol::DioBrick::~DioBrick()
{
}

void bol::DioBrick::writeOut(bol::BrickPin pin, bol::BrickLogVal value)
{
	if(value == bol::BrickLogVal::HIGH) {
		pout |= (unsigned char)pin;
	}
	else {
		pout &= ~(unsigned char)pin;
	}

	std::vector<unsigned char> msg1 = {CMD_SET_POUT, pout};
	bbus->write(address, msg1);
}

bol::BrickLogVal bol::DioBrick::readIn(bol::BrickPin pin)	
{
	std::vector<unsigned char> res = bbus->read(address, CMD_GET_PIN, 1);

	if(res[0] & (unsigned char)pin) {
		return bol::BrickLogVal::HIGH;
	}

	return bol::BrickLogVal::LOW;
}
