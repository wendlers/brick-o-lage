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

#include <iostream>
#include <unistd.h>
#include <signal.h>

#include "brick.hpp"
#include "diobrick.hpp"

using namespace std;
using namespace bol;

static bool terminated = false;

// catch sigterm
void sigHandler(int) 
{
    terminated = true;
}

// port update handler for changing inputs
class PortUpdateHandler
{
private:

	Brick b;

public:

	PortUpdateHandler(Brick &brick) : b(brick)
	{
		cout << "con" << endl;
	}

	void onUpdate(BrickPort &p)
	{
		cout << "PortUpate: name=" << p.getName() << ", value=" << (int)p << endl;

		if(p.getName() == DioBrick::DI1)
		{
			cout << "change DO1" << endl;
			b[DioBrick::DO1] = p;
		}
		else if(p.getName() == DioBrick::DI2)
		{
			cout << "change DO2" << endl;
			b[DioBrick::DO2] = p;
		}
		else if(p.getName() == DioBrick::DI3)
		{
			cout << "change DO3" << endl;
			b[DioBrick::DO3] = p;
		}
	}
};

int main(void) 
{
	try
	{
		// handle sigint
		signal(SIGINT, sigHandler);

		// start brick bus
		BrickBus::initialize();

		// ask for DIO brick on address 0x48
		// Brick b(0x48);
		Brick b(Brick::DIO1);

		// set sync priority
		b.setSyncPriority(3);

		// describe bus and bricks by JSON meta data
		cout << endl << BrickBus::describe() << endl << endl;

		b[DioBrick::DO1] = DioBrick::HIGH;
		b[DioBrick::DO2] = DioBrick::HIGH;
		b[DioBrick::DO3] = DioBrick::HIGH;
		b[DioBrick::DO4] = DioBrick::HIGH;

		PortUpdateHandler h(b);

		b[DioBrick::DI1].connect(boost::bind(&PortUpdateHandler::onUpdate, h, _1));
		b[DioBrick::DI2].connect(boost::bind(&PortUpdateHandler::onUpdate, h, _1));
		b[DioBrick::DI3].connect(boost::bind(&PortUpdateHandler::onUpdate, h, _1));

		// loop until SIGINT received
		while(!terminated) 
		{
			b[DioBrick::DO4] = DioBrick::HIGH;
			usleep(10000);
			b[DioBrick::DO4] = DioBrick::LOW;
			usleep(10000);
		}

		// stop brick bus
		BrickBus::terminate();
	}
	catch (exception& e)
	{
		cerr << "exception caught: " << e.what() << endl;
		return 1;
	}

	cout << "Terminated" << endl;

	return 0;
}
