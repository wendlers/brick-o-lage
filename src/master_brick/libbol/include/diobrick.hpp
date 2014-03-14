/*
 * This file is part of the Brick-o-Lage project.
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

#ifndef __DIOBRICK_HPP__
#define __DIOBRICK_HPP__

#include "brick.hpp"

namespace bol {

class DioBrick : public Brick
{
private:
	unsigned char pout;

public:

	DioBrick(BrickBus *bus, int slaveAddress);

	void writeOut(BrickPin pin, BrickLogVal value);

	BrickLogVal readIn(BrickPin pin);	
};

}

#endif
