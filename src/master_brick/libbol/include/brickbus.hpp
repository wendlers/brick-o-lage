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

#ifndef __BRICKBUS_HPP__
#define __BRICKBUS_HPP__

#include <string>
#include <vector>
#include <map>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

namespace bol {

class Brick;
enum class BrickType; 

typedef std::map<int, Brick *> BrickMap;

class BrickBus
{
private:
	
	int address;

	int fd; 
	
	BrickMap bmap;

	boost::thread *syncThread;

	boost::mutex busMutex;

public:

	BrickBus(int busAddress = 0, bool startSync = true);

	~BrickBus();

	void write(int slaveAddress, std::vector<unsigned char> data); 

	std::vector<unsigned char> read(int slaveAddress, unsigned char reg, int expectedLength);

	std::vector<unsigned char> xfer(int slaveAddress, std::vector<unsigned char> data, int expectedLength);

	Brick *getBrickByAddress(int slaveAddress);

	Brick *getBrickByAddress(int slaveAddress, BrickType type);

	void sync(bool out= true, bool in = true);

	void startSyncThread();

	void stopSyncThread();

	std::string describe();

private:

	void discover();

	void syncThreadFunction();

	void open(const char *device);

	void close();
};

}

#endif
