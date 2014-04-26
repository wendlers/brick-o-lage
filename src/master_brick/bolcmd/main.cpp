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

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <signal.h>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <brick.hpp>
#include <diobrick.hpp>
#include <dcmbrick.hpp>
#include <brickscript.hpp>

using namespace boost::program_options;

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
	}

	void onUpdate(BrickPort &p)
	{
		cout << "PortUpate: name=" << p.getName() << ", value=" << (int)p << endl;
	}
};

int main(int argc, const char* argv[])
{
	BrickScript bs;

	// handle sigint
	signal(SIGINT, sigHandler);

	options_description description("Bric(k)-o-Lage command line utility");

	description.add_options()
	    ("help,h", "show help message")
	    ("script,s", value<string>(), "run bolscript")
	    ("brick,b", value<vector<string>>(), "brick identifier")
	    ("port,p", value<vector<string>>(), "port identifier")
	    ("value,v", value<vector<int>>(), "port value")
	;

    variables_map vm;
    store(parse_command_line(argc, argv, description), vm);
    notify(vm);

	if(vm.count("help"))
	{
		cout << description;
		return 0;
	}

	if(vm.count("script"))
	{
		string script = vm["script"].as<string>();

		ifstream infile(script, ifstream::binary);

		std::string code = "";

		if(infile.is_open())
		{
			std::string line;
			while(std::getline(infile, line))
			{
				code += line;
				code += "\n";
			}

			bs.run(code);
		}
		else
		{
			cerr << "ERROR: Unable to load script: " << script << endl;
			return 1;
		}

		infile.close();
	}
	else if(vm.count("brick") && vm.count("port") && vm.count("value"))
	{
		try
		{
			vector<string> bricks = vm["brick"].as<vector<string>>();
			vector<string> ports  = vm["port"].as<vector<string>>();
			vector<int> val = vm["value"].as<vector<int>>();

			for(unsigned int i = 0; i < bricks.size(); i++)
			{
				Brick b(bricks[i].c_str());

				// set port value
				b[ports[i].c_str()] = val[i];
			}
		}
		catch (exception& e)
		{
			cerr << "ERROR: " << e.what() << endl;
			return 1;
		}
	}
	else if(vm.count("brick") && vm.count("port"))
	{
		try
		{
			vector<string> bricks = vm["brick"].as<vector<string>>();
			vector<string> ports  = vm["port"].as<vector<string>>();

			for(unsigned int i = 0; i < bricks.size(); i++)
			{
				Brick b(bricks[i].c_str());

				PortUpdateHandler h(b);

				// register handler
				b[ports[i].c_str()].connect(boost::bind(&PortUpdateHandler::onUpdate, h, _1));
			}
		}
		catch (exception& e)
		{
			cerr << "ERROR: " << e.what() << endl;
			return 1;
		}
	}

	cout << "Press Ctrl+C to end" << endl;

	// loop until SIGINT received
	while(!terminated)
	{
		usleep(10000);
	}

	cout << "Terminated" << endl;

	return 0;
}
