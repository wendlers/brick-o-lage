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
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <iostream>

#include "brickexception.hpp"
#include "brickbus.hpp"

// #define TRACE	std::cout << __func__ << std::endl;
#define TRACE

bol::BrickBus::BrickBus(const char *device)
{
	TRACE

	fd = -1;

	open(device);
}

bol::BrickBus::~BrickBus()
{
	TRACE

	close();
}

void bol::BrickBus::write(int slaveAddress, std::vector<unsigned char> data) 
{
	TRACE

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

