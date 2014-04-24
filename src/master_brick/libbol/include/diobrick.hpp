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

#ifndef __DIOBRICK_HPP__
#define __DIOBRICK_HPP__

#include "brick.hpp"

namespace bol {

class DioBrick : public GenericBrick
{
private:
	unsigned char pout;

	DioBrick(const GenericBrick &brick);

	virtual ~DioBrick();

public:

	static const int LOW;
	static const int HIGH;

	static const char* DI1;
	static const char* DI2;
	static const char* DI3;
	static const char* DI4;

	static const char* DO1;
	static const char* DO2;
	static const char* DO3;
	static const char* DO4;

protected:

	void sync(bool out = true, bool in = true);

	friend class BrickBus;
};

}

#endif
