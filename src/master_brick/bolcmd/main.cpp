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
#include "brick.hpp"

using namespace std;
using namespace bol;

int main(void) 
{

	cout << "DIO" << endl;

	try
	{
		// get brick bus instance
		BrickBus bb; 

		// ask for DIO brick on address 0x48
		// if no brick at 0x48 or brick of different type, exception is thrown 
		Brick *b = bb.getBrickByAddress(0x48, BrickType::DIO);

		// describe brick by JSON meta data
		cout << endl << "Brick description" << endl << b->describe() << endl << endl;

		// reset MCU
		cout << "Performe MCU reset" << endl;
		b->reset();

		// switch all outputs on
		b->getPortByName("DO1")->setValue(1);
		b->getPortByName("DO2")->setValue(1);
		b->getPortByName("DO3")->setValue(1);
		b->getPortByName("DO4")->setValue(1);
		b->sync();

		// map state of input to output for ever
		while(1) 
		{
			b->sync();	

			b->getPortByName("DO1")->setValue(b->getPortByName("DI1")->getValue());
			b->getPortByName("DO2")->setValue(b->getPortByName("DI2")->getValue());
			b->getPortByName("DO3")->setValue(b->getPortByName("DI3")->getValue());
			b->getPortByName("DO4")->setValue(b->getPortByName("DI4")->getValue());

			usleep(20000);
		}
	}
	catch (exception& e)
	{
		cerr << "exception caught: " << e.what() << endl;
		return 1;
	}

	return 0;
}
