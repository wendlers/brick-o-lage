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

#include <dirent.h>
#include <fstream>
#include "bolcontroller.hpp"

bol::srv::BolController::BolController(std::string userDataLocation)
{
	userData = userDataLocation;
}

bool bol::srv::BolController::runScriptFromFile(std::string scriptFile)
{
	bool stat = false;

	std::string code = "";

	// if name is given, try loading data from file
	std::ifstream infile(scriptFile, std::ifstream::binary);

	if(infile.is_open())
	{
		std::string line;

		while(std::getline(infile, line))
		{
			code += line;
			code += "\n";
		}

		stat = true;

		bs.run(code);
	}

	infile.close();

	return stat;
}

void bol::srv::BolController::postRun(Mongoose::Request& request, Mongoose::StreamResponse& response)
{
	std::string code = request.get("pythonCode", "(unknown)");

	// run script in background ...
	bs.run(code);

	response << "{\"isRunning\": " << bs.isRunning() << "}";
}

void bol::srv::BolController::getRun(Mongoose::Request& request, Mongoose::StreamResponse& response)
{
	response << "{\"isRunning\": " << bs.isRunning() << "}";
}

void bol::srv::BolController::getStop(Mongoose::Request& request, Mongoose::StreamResponse& response)
{
	bs.stop();

	response << "{\"isRunning\": " << bs.isRunning() << "}";
}

void bol::srv::BolController::postUserData(Mongoose::Request &request, Mongoose::StreamResponse &response)
{
	std::string name 	= request.get("userDataName", "(unknown)");
	std::string content = request.get("userDataContent", "(unknown)");

	std::string  filePath = userData + "/" + name;

	std::ofstream outfile(filePath, std::ofstream::binary);
	outfile << content;
	outfile.close();
}

void bol::srv::BolController::getUserData(Mongoose::Request &request, Mongoose::StreamResponse &response)
{
	std::string name 	 = request.get("userDataName", "(unknown)");
	std::string filePath = userData + "/" + name;

	// if no name is given, send back list of available files
	if(name == "(unknown)")
	{
		DIR *dir;
		struct dirent *ent;
		bool first = true;

		if ((dir = opendir(userData.c_str())) != NULL)
		{
			response << "[";

			while ((ent = readdir(dir)) != NULL)
			{
				if (ent->d_name[0] != '.')
				{
					if(!first)
					{
						response << ", ";
					}
					response << "\"" << ent->d_name << "\"";
					first = false;
				}
			}

			response << "]";

			closedir(dir);
		}

		return;
	}

	// if name is given, try loading data from file
	std::ifstream infile(filePath, std::ifstream::binary);

	if(infile.is_open())
	{
		std::string line;
		while(std::getline(infile, line))
		{
			response << line;
		}
	}

	infile.close();
}

void bol::srv::BolController::setup()
{
	{
		using namespace Mongoose;
		addRoute("POST", "/scriptengine/run", BolController, postRun);
		addRoute("GET" , "/scriptengine/run", BolController, getRun);
		addRoute("GET" , "/scriptengine/stop", BolController, getStop);
		addRoute("POST", "/userdata", BolController, postUserData);
		addRoute("GET", "/userdata", BolController, getUserData);
	}
}

bol::BrickScript *bol::srv::BolController::getScriptEngine()
{
	return &bs;
}
