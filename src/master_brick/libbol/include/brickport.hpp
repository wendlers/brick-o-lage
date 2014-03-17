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

#ifndef __BRICKPORT_HPP__
#define __BRICKPORT_HPP__

#include <map>
#include <string>

namespace bol {

enum class BrickPortType {
	INPUT = 0, 
	OUTPUT = 1,
};

class BrickPort
{
private:
	
	std::string	name;

	BrickPortType type;

	int from;
	int to;
	int	step;
	int current;

public:

	BrickPort(std::string portName, BrickPortType portType, int valueFrom = 0, int valueTo = 1, int valueStep = 1);

	std::string getName();

	BrickPortType getType();

	void setValue(int value);

	int getValue();

	int getValueFrom();

	int getValueTo();

	int getValueStep();

	std::string describe();
};

typedef std::map<std::string, BrickPort *> BrickPortMap;
typedef std::pair<std::string, BrickPort *> BrickPortMapPair;

}
#endif
