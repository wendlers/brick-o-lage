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

#include "brick.hpp"
#include "brickexception.hpp"

#define CMD_RESET       0xA0
#define CMD_FW_TYPE		0xF0
#define CMD_FW_VERSION	0xF1

bol::Brick::Brick(bol::BrickBus *bus, int slaveAddress)
{
	bbus = bus;

	address = slaveAddress;
}

void bol::Brick::addPort(BrickPort *port)
{
	pmap.insert(BrickPortMapPair(port->getName(), port));
}

bol::Brick::Brick(bol::Brick *brick)
{
	bbus = brick->bbus;

	address = brick->address;
}


bol::Brick::~Brick()
{
	BrickPortMap::iterator it; 

	for(it = pmap.begin(); it != pmap.end(); ++it)
	{
		BrickPort *p = it->second;
		delete p;
	}
}

bol::BrickType bol::Brick::getType()
{
	std::vector<unsigned char> res = bbus->read(address, CMD_FW_TYPE, 1);

	switch(res[0]) {
		case 0x01: return bol::BrickType::DIO;
	}

	return bol::BrickType::UNKNOWN;
}

unsigned char bol::Brick::getFirmwareVersion() 
{
	std::vector<unsigned char> res = bbus->read(address, CMD_FW_VERSION, 1);

	return res[0];
}

void bol::Brick::reset()
{
	std::vector<unsigned char> msg = {CMD_RESET};
	bbus->write(address, msg);
}

bol::BrickPort *bol::Brick::getPortByName(const char *name)
{
	if(pmap[name] == NULL)
	{
		throw BrickException("Invalid port name");
	}

	return pmap[name];
}

bol::BrickPortMap *bol::Brick::getPorts()
{
	return &pmap;
}

std::string bol::Brick::describe()
{
	std::stringstream d;

	d << "{\"Brick\": " << std::endl << "{\"type\"=";

	BrickType type = getType();

	if(type == BrickType::DIO)
	{
		d << "\"DIO\"";
	}
	else if(type == BrickType::DCM)
	{
		d << "\"DCM\"";
	}
	else if(type == BrickType::SER)
	{
		d << "\"SER\"";
	}
	else if(type == BrickType::SEN)
	{
		d << "\"SEN\"";
	}
	else
	{
		d << "\"UNKNOWN\"";
	}

	d << ", \"fw-version\"=" << (int)getFirmwareVersion();
	d << ", " << std::endl << "\"ports\"=[" << std::endl;
	
	BrickPortMap::iterator it; 

	for(it = pmap.begin(); it != pmap.end(); ++it)
	{
		BrickPort *p = it->second;
		d << p->describe();
		d << ", " << std::endl;
	}

	d << "]";

	d << "}}";

	return d.str();
}

void bol::Brick::sync(bool out, bool in)
{
}
