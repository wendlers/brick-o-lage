#include <boost/python.hpp>

#include "brick.hpp"

using namespace boost::python;

namespace bol
{
class PyBrickWrap
{
private:

	static BrickBus *brickBus; 
	Brick *brick;

public:
	
	static void setup(BrickBus *bus)
	{
		brickBus = bus;
	}

	PyBrickWrap(const std::string &brickName)
	{
		int address = 0;

		if(brickName == "DIO1")
		{
			address = 0x48;
		}
		else
		{
			return;
		}

		brick = PyBrickWrap::brickBus->getBrickByAddress(address);
	}

	void set(const char *name, int value)
	{
		if(brick != NULL)
		{
			brick->setPortValue(name, value);
		}
	}

	int get(const char *name)
	{
		if(brick != NULL)
		{
			return brick->getPortValue(name);
		}

		return -1;
	}
};

BrickBus *PyBrickWrap::brickBus = NULL; 

}


BOOST_PYTHON_MODULE(bol)
{
	class_<bol::PyBrickWrap>("Brick", init<std::string>())
		.def("set",&bol::PyBrickWrap::set)
		.def("get",&bol::PyBrickWrap::get);
		def("sleep", &::sleep);
}

int main(int, char **) {

	bol::BrickBus bb;
	bb.getBrickByAddress(0x48);

	bol::PyBrickWrap::setup(&bb);

    Py_Initialize();

    try 
	{
        initbol();

		object main_module = import("__main__");
		object main_namespace = main_module.attr("__dict__");

   		object ignored = exec_file(
			"bolprog.py",
			main_namespace);

        // PyRun_SimpleString("import bol");
        // PyRun_SimpleString("import bolprog");
        // PyRun_SimpleString("bolprog.main()");
     }
    catch (error_already_set) 
	{
        PyErr_Print();
    }

    Py_Finalize();

    return 0;
}
