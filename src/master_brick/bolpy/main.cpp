#include <boost/python.hpp>

#include <exception>
#include <brick.hpp>

using namespace boost::python;
using namespace bol;
using namespace std;

BOOST_PYTHON_MODULE(bol)
{
	class_<bol::Brick>("Brick", init<const char *>())
		.def("setPortValue",&bol::Brick::setPortValue)
		.def("getPortValue",&bol::Brick::getPortValue);
		def("sleep", &::sleep);
}

int main(int, char **)
{
	try
	{
		BrickBus::initialize();
	}
	catch (std::exception& e)
	{
		cerr << "exception caught: " << e.what() << endl;
		return 1;
	}

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

   		BrickBus::terminate();
    }
    catch (error_already_set) 
	{
        PyErr_Print();
    }

    Py_Finalize();

	try
	{
		BrickBus::terminate();
	}
	catch (exception& e)
	{
		cerr << "exception caught: " << e.what() << endl;
		return 1;
	}

    return 0;
}
