/*
* Varex 4343CT Driver
* 
* Software written for controlling the 2 Varex 4343 CT detectors at the HED beamline at the
* European X-ray free electron laser (EuXFEL)
* 
* This software uses the dlls provided by the Varex company and wraps a Socket server around it.
* Currently the server is started on port 3490.
* 
* Commands are send with the following syntax:
* 
* generalized to: varex:<det_number>:<verb>(:<param>:(<value>));
* 
* All commands have to end with ";". Det_number starts with 1. 
* Possible verbs are: get, set, start_acquisition, stop_acquisition.
* get and set require <param>. Possible params are: gain, exposure_time, streaming_target
* set_requires a value for the parameter to be set to.
* 
* example commands:
* 
* varex:1:set:gain:3;
* varex:1:get:gain;
* varex:2:start_acquisition;
*/

#include "server.h"
#include "handle.h"
#include <vector>
#include <thread>

int main() {
	SOCKET entry_socket = start_server("127.0.0.1", "3490");

	while (true) {
		SOCKET client_socket = accept_client(entry_socket);
		std::thread(handle_connection, client_socket).detach();
	}
}