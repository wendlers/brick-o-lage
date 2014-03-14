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
		BrickBus bb; 
		DioBrick db(&bb, 0x48);

		// reset MCU
		cout << "Performe MCU reset" << endl;
		db.reset();

		// read HW type
		cout << "Type = " << (int)(db.getType()) << endl;
		
		// read FW version
		cout << "FW-Version = " << (int)(db.getFirmwareVersion()) << endl;
	
		db.writeOut(BrickPin::P1, BrickLogVal::HIGH);
		db.writeOut(BrickPin::P2, BrickLogVal::HIGH);
		db.writeOut(BrickPin::P3, BrickLogVal::HIGH);
		db.writeOut(BrickPin::P4, BrickLogVal::HIGH);

		while(1) 
		{
			// copy state of input to output
			for(int pin = 0; pin < 4; pin++)
			{			
				db.writeOut(static_cast<BrickPin>(1 << pin), db.readIn(static_cast<BrickPin>(1 << pin)));
			}
		}
	}
	catch (exception& e)
	{
		cerr << "exception caught: " << e.what() << '\n';
	}

	return 0;
}
