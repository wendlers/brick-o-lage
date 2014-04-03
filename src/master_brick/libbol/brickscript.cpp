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

#include <boost/python.hpp>

#include "brick.hpp"
#include "brickscript.hpp"

struct BrickType_to_python_str
{
    static PyObject* convert(bol::BrickType const& t)
      {
    	if(t == bol::BrickType::DIO)
    	{
    		return boost::python::incref(boost::python::object("DIO").ptr());
    	}
		return boost::python::incref(boost::python::object("UNKNOWN").ptr());
      }
};

BOOST_PYTHON_MODULE(bol)
{
	boost::python::class_<bol::Brick>("Brick", boost::python::init<const char *>())
		.def("setPortValue",&bol::Brick::setPortValue)
		.def("getPortValue",&bol::Brick::getPortValue)
		.def("setSyncPriority", &bol::Brick::setSyncPriority)
		.def("getType", &bol::Brick::getType)
		.def("getFirmwareVersion", &bol::Brick::getFirmwareVersion)
		.def("reset", &bol::Brick::reset)
		.def("describe", &bol::Brick::describe);
		boost::python::def("set_port", &bol::Brick::set_port);
		boost::python::def("get_port", &bol::Brick::get_port);
		boost::python::def("sleep", &::sleep);
		boost::python::def("usleep", &::usleep);

		boost::python::to_python_converter<bol::BrickType, BrickType_to_python_str>();
}

bol::BrickScript::BrickScript()
{
	setPause(true);

	execThread = NULL;
}

bol::BrickScript::~BrickScript()
{
	if(execThread != NULL)
	{
		execThread->interrupt();

		// FIXME: strange work around needed to avoid segfault
		// on shutdown when engine is paused ...
		if(pause)
		{
			run("while True:\n\tpass\n");
		}

		stop();
		waitUntilStopped();

		execThread->join();

		delete execThread;
		execThread = NULL;
	}

	std::cout << std::endl;
	std::cout << "**********************************" << std::endl;
	std::cout << "** BOL scripting engine ended     " << std::endl;
	std::cout << "**********************************" << std::endl;
	std::cout << std::endl;
}

void bol::BrickScript::run(std::string prog)
{
	boost::mutex::scoped_lock l(runMutex);

	if(!pause)
	{
		stop();

		if(!waitUntilStopped())
		{
			std::cout << std::endl;
			std::cout << "**********************************" << std::endl;
			std::cout << "** BOL unable to stop script      " << std::endl;
			std::cout << "**********************************" << std::endl;
			std::cout << std::endl;
			return;
		}
	}

	currentProg = prog;

	if(execThread == NULL)
	{
		execThread = new boost::thread(boost::bind(&bol::BrickScript::execThreadFunction, this));
	}

	setPause(false);
}


void bol::BrickScript::stop()
{
	if(execThread != NULL && Py_IsInitialized())
	{
		PyErr_SetNone(PyExc_KeyboardInterrupt);
	}
}

bool bol::BrickScript::isRunning()
{
	return !pause;
}

void bol::BrickScript::execThreadFunction()
{
	Py_Initialize();

	PyEval_InitThreads();

	// Save a pointer to the main PyThreadState object
	PyThreadState *mainThreadState = PyThreadState_Get();

	// Get a reference to the PyInterpreterState
	PyInterpreterState *mainInterpreterState = mainThreadState->interp;

	// Create a thread state object for this thread
	PyThreadState *threadState = PyThreadState_New(mainInterpreterState);

	// Release global lock
	PyEval_ReleaseLock();

	// Acquire global lock
	PyEval_AcquireLock();

	// Swap in my thread state
	PyThreadState *tempState = PyThreadState_Swap(threadState);

	initbol();

	PyRun_SimpleString("import bol");

	// exec python
	// boost::python::object main_module = boost::python::import("__main__");
	// boost::python::object main_namespace = main_module.attr("__dict__");

	std::cout << std::endl;
	std::cout << "**********************************" << std::endl;
	std::cout << "** BOL scripting engine started   " << std::endl;
	std::cout << "**********************************" << std::endl;
	std::cout << std::endl;

	while(!boost::this_thread::interruption_requested())
	{
		blockOnPause();

		try
		{
			std::cout << std::endl;
			std::cout << "**********************************" << std::endl;
			std::cout << "** BOL script execution started   " << std::endl;
			std::cout << "**********************************" << std::endl;
			std::cout << std::endl;

			PyRun_SimpleString(currentProg.c_str());
			// boost::python::object ignored = boost::python::exec(currentProg.c_str(), main_namespace);

			std::cout << std::endl;
			std::cout << "**********************************" << std::endl;
			std::cout << "** BOL script execution ended     " << std::endl;
			std::cout << "**********************************" << std::endl;
			std::cout << std::endl;
		}
		catch(boost::python::error_already_set)
		{
			std::cout << std::endl;
			std::cout << "**********************************" << std::endl;
			std::cout << "** BOL script execution failed    " << std::endl;
			std::cout << "**********************************" << std::endl;
			std::cout << std::endl;

			// PyErr_Print();
		}

		setPause(true);
	}

	// Swap out the current thread
	PyThreadState_Swap(tempState);

	// Release global lock
	PyEval_ReleaseLock();

	// Clean up thread state
	PyThreadState_Clear(threadState);
	PyThreadState_Delete(threadState);

	Py_Finalize();
}

void bol::BrickScript::blockOnPause()
{
    boost::unique_lock<boost::mutex> lock(pauseMutex);

    while(pause)
    {
        pauseChanged.wait(lock);
    }
}

void bol::BrickScript::setPause(bool doPause)
{
    {
        boost::unique_lock<boost::mutex> lock(pauseMutex);
        pause = doPause;
    }

    pauseChanged.notify_all();
}

bool bol::BrickScript::waitUntilStopped(int timeOut)
{
	int tout = timeOut;

	while (!pause)
	{
		usleep(1000);

		if (tout-- < 0)
		{
			return false;
		}
	}

	return true;
}
