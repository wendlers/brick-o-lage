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
#include "diobrick.hpp"

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
		DioBrick *b = (DioBrick *)bb.getBrickByAddress(0x48, BrickType::DIO);

		// reset MCU
		cout << "Performe MCU reset" << endl;
		b->reset();

		// read HW type
		cout << "Type = " << (int)(b->getType()) << endl;
		
		// read FW version
		cout << "FW-Version = " << (int)(b->getFirmwareVersion()) << endl;
	
		// switch all outputs on
		b->writeOut(BrickPin::P1, BrickLogVal::HIGH);
		b->writeOut(BrickPin::P2, BrickLogVal::HIGH);
		b->writeOut(BrickPin::P3, BrickLogVal::HIGH);
		b->writeOut(BrickPin::P4, BrickLogVal::HIGH);

		// map state of input to output for ever
		while(1) 
		{
			// copy state of input to output
			for(int pin = 0; pin < 4; pin++)
			{			
				b->writeOut(static_cast<BrickPin>(1 << pin), b->readIn(static_cast<BrickPin>(1 << pin)));
			}
		}
	}
	catch (exception& e)
	{
		cerr << "exception caught: " << e.what() << endl;
		return 1;
	}

	return 0;
}
