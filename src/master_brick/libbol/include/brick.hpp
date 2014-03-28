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
#include "brickport.hpp"

namespace bol {

enum class BrickType 
{ 
	UNKNOWN = 0x00,
	DIO 	= 0x01,
	DCM 	= 0x02,
	SER 	= 0x03,
	SEN 	= 0x04,
	ANY 	= 0xFF,
};

class Brick
{
private:

	GenericBrick *brick;

public:

	static const char* DIO1;
	static const char* DIO2;
	static const char* DIO3;
	static const char* DIO4;

	Brick(int slaveAddress);

	Brick(int slaveAddress, BrickType type);

	Brick(const char *name);

	BrickType getType();

	unsigned char getFirmwareVersion();

	void reset();

	BrickPort *getPortByName(const char *name);

	BrickPortMap *getPorts();

	void setPortValue(const char *name, int value);

	int  getPortValue(const char *name);

	void setSyncPriority(int syncPriority);

	int getSyncPriority();

	std::string describe();

	BrickPort &operator[](const char *name);
};

class GenericBrick
{
protected:

	BrickBus *bbus;

	int address;

	int priority;

	int currentPriority;

	int	fwVersion;

	BrickType type;

	BrickPortMap pmap;

	GenericBrick(BrickBus *brickBus, int slaveAddress);

protected:

	GenericBrick(const GenericBrick &brick);

public:

	virtual ~GenericBrick();

	BrickType getType();

	unsigned char getFirmwareVersion();

	void reset();

	BrickPort *getPortByName(const char *name);

	BrickPortMap *getPorts();

	void setPortValue(const char *name, int value);

	int  getPortValue(const char *name);

	void setSyncPriority(int syncPriority);

	int getSyncPriority();

	std::string describe();

	BrickPort &operator[](const char *name);

protected:

	virtual void sync(bool out = true, bool in = true);

	bool shouldSync();

	void addPort(BrickPort *port);

	friend class BrickBus;
};

}

#endif
