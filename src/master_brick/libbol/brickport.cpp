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

#include <sstream>
#include "brickport.hpp"

bol::BrickPort::BrickPort(std::string portName, BrickPortType portType, int valueFrom, int valueTo, int valueStep)
{
	name = portName;
	type = portType;
	from = valueFrom;
	to   = valueTo;
	step = valueStep;

	current = from;
}

std::string bol::BrickPort::getName()
{
	return name;
}

bol::BrickPortType bol::BrickPort::getType()
{
	return type;
}

void bol::BrickPort::setValue(int value)
{
	bool doSig = (current != value); 

	current = value;
	
	if(doSig)
	{
		sig(this);
	}
}

int bol::BrickPort::getValue()
{
	return current;
}

int bol::BrickPort::getValueFrom()
{
	return from;
}

int bol::BrickPort::getValueTo()
{
	return to;
}

int bol::BrickPort::getValueStep()
{
	return step;
}

std::string bol::BrickPort::describe()
{
	std::stringstream d;

	d << "\"BrickPort\": {\"name\"=" << name << "\", \"type\"=";

	if(type == BrickPortType::INPUT)
	{
		d << "\"INPUT\"";
	}
	else
	{
		d << "\"OUTPUT\"";
	}

	d << ", \"from\"=" << from;
	d << ", \"to\"=" << to;
	d << ", \"step\"=" << step;
	d << ", \"current\"=" << current;

	d << "}";

	return d.str();
}    

bol::BrickPort::BrickPortSigCon bol::BrickPort::connect(bol::BrickPort::BrickPortSig::slot_function_type subscriber)
{
	return sig.connect(subscriber);
}

void bol::BrickPort::disconnect(bol::BrickPort::BrickPortSigCon subscriber)
{
	subscriber.disconnect();
}

