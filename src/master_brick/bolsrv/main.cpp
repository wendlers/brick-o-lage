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

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <mongoose/Server.h>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include "daemon.hpp"
#include "bolcontroller.hpp"

namespace po = boost::program_options;

static bool terminated = false;

// catch sigterm
void sigHandler(int)
{
    terminated = true;
}

int main(int argc, const char* argv[])
{
	po::options_description description("Bric(k)-o-Lage server");

	description.add_options()
	    ("help,h", "show help message")
	    ("foreground,f", "run in foreground (don't fork)")
	    ("verbose,v", "be verbose, even if run as daemon")
	    ("lock,l", po::value<string>()->default_value("/var/run/bolsrv.lock"), "lockfile to use for daemon")
	    ("root,r", po::value<string>()->default_value("/opt/bol/html"), "HTTP server document root")
	    ("port,p", po::value<int>()->default_value(80), "HTTP server port")
	    ("userdata,u", po::value<string>()->default_value("/opt/bol/html/userdata"), "location to store userdata")
	    ("script,s", po::value<string>(), "bol script to run on startup")
	;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, description), vm);
    po::notify(vm);

	if(vm.count("help"))
	{
		std::cout << description;
		return 0;
	}

	// run in foreground
	if(vm.count("foreground"))
	{
		// handle sigint
		signal(SIGINT, sigHandler);
	}
	// run in background (fork as daemon)
	else
	{
		bool verbose = !vm.count("verbose");

		if(!daemonize(verbose, verbose, vm["root"].as<string>().c_str(), vm["lock"].as<string>().c_str()))
		{
			std::cerr << "Failed to start bolsrv as daemon" << std::endl;
			return 1;
		}

		// handle sigterm
		signal(SIGTERM, sigHandler);
	}

	bol::srv::BolController bc(vm["userdata"].as<string>());

	// run startup script if given
	if(vm.count("script"))
	{
		bc.runScriptFromFile(vm["script"].as<string>());
	}

    Mongoose::Server server(vm["port"].as<int>());
    server.registerController(&bc);
    server.setOption("document_root", vm["root"].as<string>());

    server.start();

	std::cout << std::endl;
	std::cout << "**********************************" << std::endl;
	std::cout << "** BOL server started             " << std::endl;
	std::cout << "**********************************" << std::endl;
	std::cout << std::endl;

//	int cnt = 0;

    while (!terminated)
    {
        sleep(1);

//        if(cnt++ % 15 == 0)
//        {
//        	std::cout << std::endl;
//        	std::cout << "**********************************" << std::endl;
//        	std::cout << "** BOL server alive               " << std::endl;
//        	std::cout << "**********************************" << std::endl;
//        	std::cout << std::endl;
//        }
    }

    server.stop();

	std::cout << std::endl;
	std::cout << "**********************************" << std::endl;
	std::cout << "** BOL server ended               " << std::endl;
	std::cout << "**********************************" << std::endl;
	std::cout << std::endl;
}
