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

#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include <iostream>
#include <sstream>

#include "brickexception.hpp"
#include "brickbus.hpp"
#include "brick.hpp"
#include "diobrick.hpp"
#include "dcmbrick.hpp"

// #define TRACE	std::cout << __func__ << "::" << __LINE__ << std::endl;
#define TRACE

// #define DBG(...)	std::cout << __func__ << "::" << __VA_ARGS__ << std::endl;
#define DBG(...)

bol::BrickBus *bol::BrickBus::busInstance = NULL;

bol::BrickBus::BrickBus(int busAddress)
{
	TRACE

	address = busAddress;
	fd = -1;
	syncThread = NULL;

	char device[32];

	sprintf(device, "/dev/i2c-%d", address);

	open(device);
	discover();
	startSyncThread();
}

bol::BrickBus::~BrickBus()
{
	TRACE

	stopSyncThread();

	BrickMap::iterator it; 

	for(it = bmap.begin(); it != bmap.end(); ++it)
	{
		GenericBrick *b = it->second;
		delete b;
	}

	close();
}


void bol::BrickBus::initialize(int busAddress)
{
	TRACE

	static SingletonWatch watch;

	delete busInstance;

	busInstance = new BrickBus(busAddress);

	DBG("Bus initialised");
}

void bol::BrickBus::terminate()
{
	TRACE

	delete busInstance;
}

bol::BrickBus *bol::BrickBus::getInstance()
{
	TRACE

	if(busInstance == NULL)
	{
		initialize();
	}

	return busInstance;
}

void bol::BrickBus::write(int slaveAddress, std::vector<unsigned char> data) 
{
	TRACE

	boost::mutex::scoped_lock l(busMutex); 

	unsigned char buf;

    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    messages[0].addr  = slaveAddress;
    messages[0].flags = 0;
  	messages[0].len   = data.size();
    messages[0].buf   = &data[0];

    messages[1].addr  = slaveAddress;
    messages[1].flags = I2C_M_RD;
    messages[1].len   = 1;
    messages[1].buf   = &buf;

    packets.msgs  	= messages;
    packets.nmsgs 	= 2;

    if(ioctl(fd, I2C_RDWR, &packets) < 0) {
        throw BrickException("Unable to send data");
    }
}

std::vector<unsigned char> bol::BrickBus::read(int slaveAddress, unsigned char reg, int expectedLength)
{
	TRACE

	boost::mutex::scoped_lock l(busMutex); 

	DBG("Reading from address: " << slaveAddress)

	unsigned char *buf = new unsigned char[expectedLength];

    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    messages[0].addr  = slaveAddress;
    messages[0].flags = 0;
    messages[0].len   = 1;
    messages[0].buf   = &reg;

    messages[1].addr  = slaveAddress;
    messages[1].flags = I2C_M_RD;
    messages[1].len   = expectedLength;
    messages[1].buf   = buf;

    packets.msgs      = messages;
    packets.nmsgs     = 2;

    if(ioctl(fd, I2C_RDWR, &packets) < 0) {
		delete[] buf;
        throw BrickException("Unable to send data");
    }
	
	std::vector<unsigned char> res(buf, buf + expectedLength);

	delete[] buf;

	return res;
}

std::vector<unsigned char> bol::BrickBus::xfer(int slaveAddress, std::vector<unsigned char> data, int expectedLength)
{
	TRACE

	boost::mutex::scoped_lock l(busMutex); 

	unsigned char *buf = new unsigned char[expectedLength];

    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    messages[0].addr  = slaveAddress;
    messages[0].flags = 0;
  	messages[0].len   = data.size();
    messages[0].buf   = &data[0];

    messages[1].addr  = slaveAddress;
    messages[1].flags = I2C_M_RD;
    messages[1].len   = expectedLength;
    messages[1].buf   = buf;

    packets.msgs      = messages;
    packets.nmsgs     = 2;

    if(ioctl(fd, I2C_RDWR, &packets) < 0) {
		delete[] buf;
        throw BrickException("Unable to send data");
    }
	
	std::vector<unsigned char> res(buf, buf + expectedLength);

	delete[] buf;

	return res;
}	

bol::GenericBrick *bol::BrickBus::getBrickByAddress(int slaveAddress)
{
	return getBrickByAddress(slaveAddress, BrickType::ANY);
}

bol::GenericBrick *bol::BrickBus::getBrickByAddress(int slaveAddress, BrickType type)
{
	TRACE

	if(bmap[slaveAddress] != NULL)
	{
		DBG("Brick already in map")

		GenericBrick *b = bmap[slaveAddress];

		if(type != BrickType::ANY && b->getType() != type)
		{
	        throw BrickException("Brick has not expected type");
		}

		return b;
	}

	throw BrickException("Brick for address not found");
}

bol::GenericBrick *bol::BrickBus::getBrickByName(const char* name)
{
	TRACE

	std::string n(name);

	int start = 0;

	BrickType type = BrickType::ANY;

	if(n.substr(0, 3) == "DIO")
	{
		type = BrickType::DIO;
		start = 0x48;
		DBG("Brick type for name is DIO")
	}
	else if(n.substr(0, 3) == "DCM")
	{
		type = BrickType::DCM;
		start = 0x4C;
		DBG("Brick type for name is DCM")
	}

	if(start == 0)
	{
		throw BrickException("Invalid brick name given");
	}

	int addr = atoi(n.substr(3).c_str()) + start - 1;

	DBG("Brick address from name is: " << addr)

	return getBrickByAddress(addr, type);
}

std::vector<bol::GenericBrick*> bol::BrickBus::getBricks()
{
	std::vector<GenericBrick *> bricks;

	BrickMap::iterator it;

	for(it = bmap.begin(); it != bmap.end(); ++it)
	{
		GenericBrick *b = it->second;

		if(b == NULL)
		{
			DBG("NULL brick in map!")
			break;
		}

		bricks.push_back(b);
	}

	return bricks;
}

#if 0
bol::GenericBrick *bol::BrickBus::operator [](const char *name)
{
	TRACE

	return getBrickByName(name);
}

bol::GenericBrick& bol::BrickBus::operator [](const char *name)
{
	TRACE

	return *getBrickByName(name);
}
#endif

void bol::BrickBus::discover()
{
	TRACE

	for(int slaveAddress = 0x40; slaveAddress <= 0x4F; slaveAddress++)
	{
		// Brick *genericBrick = new Brick(this, slaveAddress);
		GenericBrick genericBrick(this, slaveAddress);

		try
		{
			if(genericBrick.getType() == BrickType::DIO)
			{
				DBG("Found DIO brick at address " << slaveAddress)

				bmap[slaveAddress] = (GenericBrick *)new DioBrick(genericBrick);
			}
			else if(genericBrick.getType() == BrickType::DCM)
			{
				DBG("Found DCM brick at address " << slaveAddress)

				bmap[slaveAddress] = (GenericBrick *)new DcmBrick(genericBrick);
			}
			else
			{
				DBG("Found UNKNOWN brick at address " << slaveAddress << ", ignored")
			}
		}
		catch(...)
		{
			DBG("Exception while discovering brick at address " << slaveAddress << ", ignored")
		}

		// delete genericBrick;
	}
}

void bol::BrickBus::open(const char *device)
{
	TRACE

    if((fd = ::open(device, O_RDWR)) < 0) {
        throw BrickException("Unable to open i2c device");
    }
}


void bol::BrickBus::close()
{
	TRACE

	if(fd > -1) {
		::close(fd);
	}	
}

void bol::BrickBus::sync(bool out, bool in)
{

	TRACE

	BrickMap::iterator it; 

	for(it = bmap.begin(); it != bmap.end(); ++it)
	{
		GenericBrick *b = it->second;

		if(b == NULL)
		{
			DBG("NULL brick in map!")
			break;
		}

		b->sync(out, in);
	}
}

void bol::BrickBus::startSyncThread()
{
	TRACE

	if(syncThread != NULL)
	{
		return;
	}
	
	syncThread = new boost::thread(boost::bind(&bol::BrickBus::syncThreadFunction, this));	
}

void bol::BrickBus::stopSyncThread()
{
	TRACE

	if(syncThread != NULL)
	{
		syncThread->interrupt();
		syncThread->join();
		delete syncThread;
		syncThread = NULL;
	}
}

void bol::BrickBus::syncThreadFunction()
{
	TRACE

	while(!boost::this_thread::interruption_requested())
	{
		TRACE

		boost::this_thread::sleep_for(boost::chrono::milliseconds(25));

		TRACE

		sync();

		TRACE
	}

	DBG("Sync thread ended")
}

std::string bol::BrickBus::describe()
{
	TRACE

	if(busInstance == NULL)
	{
		throw BrickException("Brick bus not initialized");
	}

	std::stringstream d;
	
	d << "{\"BrickBus\": {";
	d << "\"address\"=" << busInstance->address;
	d << ", \"bricks\"=[";

	BrickMap::iterator it; 

	for(it = busInstance->bmap.begin(); it != busInstance->bmap.end(); ++it)
	{
		GenericBrick *b = it->second;
		d << b->describe();
	}


	d << "]}}";

	return d.str();
}

