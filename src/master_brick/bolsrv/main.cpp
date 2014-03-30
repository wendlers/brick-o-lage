#include <boost/python.hpp>
#include <brick.hpp>

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <mongoose/Server.h>
#include <mongoose/WebController.h>

using namespace std;
using namespace Mongoose;

using namespace boost::python;

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
		def("set_port", &bol::Brick::set_port);
		def("sleep", &::sleep);
		def("usleep", &::usleep);

	boost::python::to_python_converter<bol::BrickType, BrickType_to_python_str>();
}

class MyController : public WebController
{
    public:

		void runPost(Request &request, StreamResponse &response)
		{
			response << "<pre>" << htmlEntities(request.get("pythonCode", "(unknown)")) << "</pre>";

		    try
			{
		    	object main_module = import("__main__");
		    	object main_namespace = main_module.attr("__dict__");

		    	std::string code = request.get("pythonCode", "(unknown)");
		   		object ignored = exec(
		   			code.c_str(),
					main_namespace);
			}
		    catch (error_already_set)
			{
		        PyErr_Print();
		    }
		}

        void hello(Request &request, StreamResponse &response)
        {
            response << "Hello " << htmlEntities(request.get("name", "... what's your name ?")) << endl;
        }

        void setup()
        {
            addRoute("GET", "/hello", MyController, hello);
            addRoute("POST", "/run", MyController, runPost);
        }
};

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

    Py_Initialize();

    try
	{
        initbol();

		object main_module = import("__main__");
		object main_namespace = main_module.attr("__dict__");

        PyRun_SimpleString("import bol");
    }
    catch (error_already_set)
	{
        PyErr_Print();
    }

    MyController myController;
    Server server(8080);
    server.registerController(&myController);
    server.setOption("document_root", "html");

    server.start();

    while (!terminated) {
        sleep(10);
    }

    Py_Finalize();
}
