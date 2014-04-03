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

#include "bolcontroller.hpp"

static bool terminated = false;

// catch sigterm
void sigHandler(int)
{
    terminated = true;
}

int main()
{
	// handle sigint
	signal(SIGINT, sigHandler);

	bol::srv::BolController bc;

    Mongoose::Server server(8080);
    server.registerController(&bc);
    server.setOption("document_root", "html");

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
