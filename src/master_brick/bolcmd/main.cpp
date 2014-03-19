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

	Brick *b;

public:

	PortUpdateHandler(Brick *brick)
	{
		b = brick;
	}

	void onUpdate(BrickPort *p)
	{
		cout << "PortUpate: name=" << p->getName() << ", value=" << p->getValue() << endl;

		if(p->getName() == DioBrick::DI1)
		{
			cout << "change DO1" << endl;
			b->getPortByName(DioBrick::DO1)->setValue(p->getValue());
		}
		else if(p->getName() == DioBrick::DI2)
		{
			cout << "change DO2" << endl;
			b->getPortByName(DioBrick::DO2)->setValue(p->getValue());
		}
		else if(p->getName() == DioBrick::DI3)
		{
			cout << "change DO3" << endl;
			b->getPortByName(DioBrick::DO3)->setValue(p->getValue());
		}
		else if(p->getName() == DioBrick::DI4)
		{
			cout << "change DO4" << endl;
			b->getPortByName(DioBrick::DO4)->setValue(p->getValue());
		}
	}
};

int main(void) 
{
	try
	{
		// handle sigint
		signal(SIGINT, sigHandler);

		// get brick bus instance
		BrickBus bb; 
		
		// ask for DIO brick on address 0x48
		// if no brick at 0x48 or brick of different type, exception is thrown 
		Brick *b = bb.getBrickByAddress(0x48, BrickType::DIO);

		// lower bricks priority for syncing (0 = highest)
		b->setSyncPriority(3);

		// describe bus and bricks by JSON meta data
		cout << endl << bb.describe() << endl << endl;

		// reset brick 
		b->reset();

		// set all outputs to HIGH 
		b->getPortByName(DioBrick::DO1)->setValue(DioBrick::HIGH);
		b->getPortByName(DioBrick::DO2)->setValue(DioBrick::HIGH);
		b->getPortByName(DioBrick::DO3)->setValue(DioBrick::HIGH);
		b->getPortByName(DioBrick::DO4)->setValue(DioBrick::HIGH);

		// connect signals for input port updates
		PortUpdateHandler h(b);

		b->getPortByName(DioBrick::DI1)->connect(boost::bind(&PortUpdateHandler::onUpdate, h, _1));
		b->getPortByName(DioBrick::DI2)->connect(boost::bind(&PortUpdateHandler::onUpdate, h, _1));
		b->getPortByName(DioBrick::DI3)->connect(boost::bind(&PortUpdateHandler::onUpdate, h, _1));
		b->getPortByName(DioBrick::DI4)->connect(boost::bind(&PortUpdateHandler::onUpdate, h, _1));

		// loop until SIGINT received
		while(!terminated) 
		{
			usleep(10000);
		}
	}
	catch (exception& e)
	{
		cerr << "exception caught: " << e.what() << endl;
		return 1;
	}

	cout << "Terminated" << endl;

	return 0;
}
