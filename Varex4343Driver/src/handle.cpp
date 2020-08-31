#include "server.h"
#include "varex.h"
#include "handle.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

using std::vector;
using std::string;

constexpr int c_recv_buffer_size = 100;

void handle_connection(SOCKET s) 
// handles the communication between client and our server
// it is awaiting commands from the client and acts accordingly
{
	string ip = get_client_ip(s);

	std::cout << ip << " connected. Waiting for Message." << std::endl;
	send(s, "Welcome! You have connected to BANANA SERVER!", 46, NULL);

	char recv_buffer[c_recv_buffer_size];
	while (recv(s, recv_buffer, c_recv_buffer_size - 1, NULL) > 0) {

		vector<string> recv_parts = read_command(recv_buffer);
		int det_index = stoi(recv_parts[1]);
		string command = recv_parts[2];

		if (command == "set") {
			string parameter = recv_parts[3];
			if (parameter == "gain") {
				int value = stoi(recv_parts[4]);
				varex::set_gain(det_index, value);
			}
			else if (parameter == "exposure_time") {
				double value = stod(recv_parts[4]);
				varex::set_exposure_time(det_index, value);
			}
			else if (parameter == "streaming_target") {
				varex::set_streaming_target(
					det_index,
					recv_parts[4].c_str(),
					recv_parts[5].c_str()
				);
			}
		}
		else if (command == "get") {
			string parameter = recv_parts[3];
			if (parameter == "status") {
				int status = varex::get_status(det_index);
			}
			else if (parameter == "gain") {
				int gain = varex::get_gain(det_index);
			}
			else if (parameter == "exposure_time") {
				double exposure_time = varex::get_exposure_time(det_index);
			}
			else if (parameter == "streaming_target") {
				string target = varex::get_streaming_target(det_index); 
			}
		}
		else if (command == "start_acquisition") {
			varex::start_collection(det_index);
		}
		else if (command == "stop_acquisition") {
			varex::stop_collection(det_index);
		}
		else {

		}

	}
	std::cout << "Connection was closed by the client" << std::endl;
}

vector<string> read_command(const char* recv_buffer) {
	std::istringstream recv_stream(recv_buffer);
	string recv_str;
	std::getline(recv_stream, recv_str, ';');
	return split(recv_str, ':');
}


vector<string> split(const string& str, char delimiter)
{
	vector<string> tokens;
	string token;
	std::istringstream tokenStream(str);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}