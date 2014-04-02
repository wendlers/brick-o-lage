#include <brick.hpp>
#include <embpy.hpp>

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <mongoose/Server.h>
#include <mongoose/WebController.h>

using namespace std;
using namespace Mongoose;


class BolController : public WebController
{
private:

	bol::EmbeddedPython	py;

public:

		void run(Request &request, StreamResponse &response)
		{
			response << "{}";

			std::string code = request.get("pythonCode", "(unknown)");

			// run in background ...
			py.exec(code);
		}

		void isRunning(Request &request, StreamResponse &response)
		{
			response << "<pre>" << py.isRunning() << "</pre>";
		}

		void stop(Request &request, StreamResponse &response)
		{
			response << "{}";

			py.stop();
		}

        void setup()
        {
            addRoute("POST", "/run", BolController, run);
            addRoute("GET", "/isRunning", BolController, isRunning);
            addRoute("GET", "/stop", BolController, stop);
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

    BolController *bc = new BolController();

    Server server(8080);
    server.registerController(bc);
    server.setOption("document_root", "html");

    server.start();

    while (!terminated) {
        sleep(1);
    }

    std::cout << "Terminating bolsrv" << std::endl;

    server.stop();

    delete bc;
}
