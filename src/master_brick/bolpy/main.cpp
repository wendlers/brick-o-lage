#include <boost/python.hpp>

#include <exception>
#include <brick.hpp>

using namespace boost::python;
using namespace bol;
using namespace std;

/** to-python convert to QStrings */
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
	class_<bol::Brick>("Brick", init<const char *>())
		.def("setPortValue",&bol::Brick::setPortValue)
		.def("getPortValue",&bol::Brick::getPortValue)
		.def("setSyncPriority", &bol::Brick::setSyncPriority)
		.def("getType", &bol::Brick::getType)
		.def("getFirmwareVersion", &bol::Brick::getFirmwareVersion)
		.def("reset", &bol::Brick::reset)
		.def("describe", &bol::Brick::describe);
		def("sleep", &::sleep);
		def("usleep", &::usleep);

	boost::python::to_python_converter<BrickType, BrickType_to_python_str>();
}

int main(int, char **)
{
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

    return 0;
}
