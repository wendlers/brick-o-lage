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

#include "bolcontroller.hpp"

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

void bol::srv::BolController::setup()
{
	{
		using namespace Mongoose;
		addRoute("POST", "/scriptengine/run", BolController, postRun);
		addRoute("GET" , "/scriptengine/run", BolController, getRun);
		addRoute("GET" , "/scriptengine/stop", BolController, getStop);
	}
}

bol::BrickScript *bol::srv::BolController::getScriptEngine()
{
	return &bs;
}
