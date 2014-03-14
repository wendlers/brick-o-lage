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

#ifndef __BRICK_HPP__
#define __BRICK_HPP__

#include <exception>

#include "brickbus.hpp"

namespace bol {

enum class BrickType 
{ 
	UNKNOWN = 0x00,
	DIO = 0x01,
};

enum class BrickPin
{ 
	P1 = 0b00000001,
	P2 = 0b00000010,
	P3 = 0b00000100,
	P4 = 0b00001000,
	P5 = 0b00010000,
	P6 = 0b00100000,
	P7 = 0b01000000,
	P8 = 0b10000000,
};

enum class BrickLogVal
{ 
	LOW  = 0x00,
	HIGH = 0x01,
};

class Brick
{
protected:

	BrickBus *mbus;
	
	int address;
	
public:

	Brick(BrickBus *bus, int slaveAddress);

	~Brick();

	BrickType getType();

	unsigned char getFirmwareVersion(); 

	void reset();
};

}

#endif
