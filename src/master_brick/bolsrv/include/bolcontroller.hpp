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

#ifndef __BOLCONTROLLER_HPP__
#define __BOLCONTROLLER_HPP__

#include <brickscript.hpp>
#include <mongoose/WebController.h>

namespace bol
{
namespace srv
{

class BolController : public Mongoose::WebController
{
private:

	bol::BrickScript bs;

	std::string userData;

public:

	BolController(std::string userDataLocation);

	bool runScriptFromFile(std::string scriptFile);

	void postRun(Mongoose::Request &request, Mongoose::StreamResponse &response);

	void getRun(Mongoose::Request &request, Mongoose::StreamResponse &response);

	void getStop(Mongoose::Request &request, Mongoose::StreamResponse &response);

	void postUserData(Mongoose::Request &request, Mongoose::StreamResponse &response);

	void getUserData(Mongoose::Request &request, Mongoose::StreamResponse &response);

	void setup();

	bol::BrickScript *getScriptEngine();
};

}
}

#endif
