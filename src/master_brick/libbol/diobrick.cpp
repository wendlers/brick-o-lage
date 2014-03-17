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

	addPort(new BrickPort("DI1", BrickPortType::INPUT));
	addPort(new BrickPort("DI2", BrickPortType::INPUT));
	addPort(new BrickPort("DI3", BrickPortType::INPUT));
	addPort(new BrickPort("DI4", BrickPortType::INPUT));

	addPort(new BrickPort("DO1", BrickPortType::OUTPUT));
	addPort(new BrickPort("DO2", BrickPortType::OUTPUT));
	addPort(new BrickPort("DO3", BrickPortType::OUTPUT));
	addPort(new BrickPort("DO4", BrickPortType::OUTPUT));
}

bol::DioBrick::~DioBrick()
{
	try
	{
		getPortByName("DI1")->setValue(0);
		getPortByName("DI2")->setValue(0);
		getPortByName("DI3")->setValue(0);
		getPortByName("DI4")->setValue(0);

		sync(true, false);
	}
	catch(...)
	{
		// so then we don't reset the ports ...
	}
}

void bol::DioBrick::sync(bool out, bool in)
{
	if(out)
	{
		unsigned char _pout = 0x00;

		_pout |= (getPortByName("DO1")->getValue() << 0);
		_pout |= (getPortByName("DO2")->getValue() << 1);
		_pout |= (getPortByName("DO3")->getValue() << 2);
		_pout |= (getPortByName("DO4")->getValue() << 3);

		// only sync outputs when they changed
		if(_pout != pout)
		{
			std::vector<unsigned char> msg1 = {CMD_SET_POUT, _pout};
			bbus->write(address, msg1);
			pout = _pout;
		}
	}

	if(in)
	{
		// always sync inputs
		std::vector<unsigned char> res = bbus->read(address, CMD_GET_PIN, 1);

		getPortByName("DI1")->setValue((res[0] >> 0) & 1);
		getPortByName("DI2")->setValue((res[0] >> 1) & 1);
		getPortByName("DI3")->setValue((res[0] >> 2) & 1);
		getPortByName("DI4")->setValue((res[0] >> 3) & 1);
	}
}
