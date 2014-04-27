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
#include "brickexception.hpp"
#include "brickscript.hpp"

namespace bol
{

class BrickPortEventCallback;

class PyCallbackHandle
{
private:

	bol::BrickPort::BrickPortSigCon con;
	std::string brick;
	std::string port;

	PyCallbackHandle(bol::BrickPort::BrickPortSigCon sigCon, const char *brickName, const char *portName)
	{
		con 	= sigCon;
		brick   = std::string(brickName);
		port 	= std::string(portName);
	}

public:

	PyCallbackHandle()
	{
	}

	friend class BrickPortEventCallback;
};

typedef std::map<boost::python::object, bol::PyCallbackHandle> PyObjBrickPortMap;

/**
 * Wrapper for BrickPortEvents
 */
class BrickPortEventCallback
{
private:

	static PyObjBrickPortMap callbacks;

public:

	static void clearCallbacks()
	{
		PyObjBrickPortMap::iterator it;

		for(it = callbacks.begin(); it != callbacks.end(); ++it)
		{
			BrickPort::BrickPortSigCon c = it->second.con;
			c.disconnect();
		}

		callbacks.clear();
	}

	static void addCallback(const char *brickName, const char *portName, boost::python::object callback)
	{
		BrickPort *p = bol::Brick::get_port(brickName, portName);

		if(strcmp(bol::Brick::DIO1, brickName) == 0)
		{
			callbacks[callback] = PyCallbackHandle(p->connect(&BrickPortEventCallback::onUpdateDIO1), brickName, portName);
		}
		else if(strcmp(bol::Brick::DIO2, brickName) == 0)
		{
			callbacks[callback] = PyCallbackHandle(p->connect(&BrickPortEventCallback::onUpdateDIO2), brickName, portName);
		}
		else if(strcmp(bol::Brick::DIO3, brickName) == 0)
		{
			callbacks[callback] = PyCallbackHandle(p->connect(&BrickPortEventCallback::onUpdateDIO3), brickName, portName);
		}
		else if(strcmp(bol::Brick::DIO4, brickName) == 0)
		{
			callbacks[callback] = PyCallbackHandle(p->connect(&BrickPortEventCallback::onUpdateDIO4), brickName, portName);
		}
	}

	static void onUpdateDIO1(BrickPort &p)
	{
		onUpdate(bol::Brick::DIO1, p);
	}

	static void onUpdateDIO2(BrickPort &p)
	{
		onUpdate(bol::Brick::DIO2, p);
	}

	static void onUpdateDIO3(BrickPort &p)
	{
		onUpdate(bol::Brick::DIO3, p);
	}

	static void onUpdateDIO4(BrickPort &p)
	{
		onUpdate(bol::Brick::DIO4, p);
	}

	static void onUpdate(std::string brickName, BrickPort &p)
	{
		PyObjBrickPortMap::iterator it;

		for(it = callbacks.begin(); it != callbacks.end(); ++it)
		{
			boost::python::object callback = it->first;

			try
			{
				if(brickName == it->second.brick && p.getName() == it->second.port)
				{
					callback(p.getValue());
				}
			}
			catch(boost::python::error_already_set)
			{
				std::cout << std::endl;
				std::cout << "**********************************" << std::endl;
				std::cout << "** BOL callback interrupted       " << std::endl;
				std::cout << "**********************************" << std::endl;
				std::cout << std::endl;

				PyErr_Print();
			}
		}
	}
};

PyObjBrickPortMap BrickPortEventCallback::callbacks;

void msleep(int value)
{
	int sec = value / 1000;
	int usec = (value - (sec * 1000)) * 1000;

	if(sec > 0)
	{
		sleep(value);
	}
	usleep(usec);
}

}

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

bool isCbReged = false;
boost::python::object pycb;

void register_callback(boost::python::object cb)
{
      pycb = cb;
      isCbReged = true;
}

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
		boost::python::def("set_port", &bol::Brick::set_port_value);
		boost::python::def("get_port", &bol::Brick::get_port_value);
		boost::python::def("sleep", &::sleep);
		boost::python::def("usleep", &::usleep);
		boost::python::def("msleep", &bol::msleep);
		boost::python::def("addCallback", &bol::BrickPortEventCallback::addCallback);

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
		if(isCbReged)
		{
			isCbReged = false;
		}

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

	// PyRun_SimpleString("import bol");

	boost::python::object main_module = boost::python::import("__main__");
	boost::python::object main_namespace = main_module.attr("__dict__");
	boost::python::dict local_namespace;
	boost::python::object result;

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

			local_namespace["__builtins__"] = main_namespace["__builtins__"];

			result = boost::python::exec("import bol\n", local_namespace);
			result = boost::python::exec(currentProg.c_str(), local_namespace);

			local_namespace.clear();

			std::cout << std::endl;
			std::cout << "**********************************" << std::endl;
			std::cout << "** BOL script ended normally      " << std::endl;
			std::cout << "**********************************" << std::endl;
			std::cout << std::endl;
		}
		catch(boost::python::error_already_set)
		{
			std::cout << std::endl;
			std::cout << "**********************************" << std::endl;
			std::cout << "** BOL script interrupted         " << std::endl;
			std::cout << "**********************************" << std::endl;
			std::cout << std::endl;

			PyErr_Print();
		}
		catch(BrickException &e)
		{
			std::cout << std::endl;
			std::cout << "**********************************" << std::endl;
			std::cout << "** BOL script BrickException      " << std::endl;
			std::cout << "**********************************" << std::endl;
			std::cout << std::endl;

			std::cerr << e.what() << std::endl;
		}

		BrickPortEventCallback::clearCallbacks();

		// try to reset all bricks on the bus
		try
		{
			BrickBus::getInstance()->reset();
		}
		catch(...)
		{
			// ignore whatever happens here ...
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

void bol::BrickScript::callback()
{
	if(isCbReged)
	{
		pycb();
	}
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
