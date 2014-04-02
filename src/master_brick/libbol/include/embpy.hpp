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

#ifndef __EMBPY_HPP__
#define __EMBPY_HPP__

#include <string>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

namespace bol {

class EmbeddedPython
{
private:

	bool pause;

	boost::mutex pauseMutex;
	boost::condition_variable pauseChanged;

	boost::thread *execThread;
	std::string currentProg;

public:

	EmbeddedPython();

	~EmbeddedPython();

	void exec(std::string prog);

	void stop();

	bool isRunning();

private:

	void execThreadFunction();

	void blockOnPause();

	void setPause(bool doPause);
};

}

#endif
