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
// handles the communication between client and the Varex control server
// it is awaiting commands from the client and acts accordingly
{
	string ip = get_client_ip(s);

	std::cout << ip << " connected. Waiting for Message." << std::endl;

	char recv_buffer[c_recv_buffer_size];
	while (recv(s, recv_buffer, c_recv_buffer_size - 1, NULL) > 0) {
		string command = read_command(recv_buffer);
		try {
			handle_command(command);
			send_success(s);
		}
		catch(const std::exception& e) {
			std::cout << "Unable to understand command: " << command << std::endl;
			std::cout << "Exception: " << e.what() << std::endl;
			send_failed(s);
		};
	}
	std::cout << "Connection was closed by the client" << std::endl;
}

void handle_command(const string& command) {
	vector<string> command_parts = split_command(command);
	int det_index = stoi(command_parts.at(1));
	string verb = command_parts.at(2);

	if (verb == "set") {
		set_param(det_index, command_parts);
	}
	else if (verb == "get") {
		get_param(det_index, command_parts);
	}
	else if (verb == "start_acquisition") {
		varex::start_collection(det_index);
	}
	else if (verb == "stop_acquisition") {
		varex::stop_collection(det_index);
	}
	else {
		std::cout << "Unable to understand command: " << command << std::endl;
	}
}

void set_param(int det_index, const vector<string>& command_parts) {
	string parameter = command_parts.at(3);
	if (parameter == "gain") {
		int value = stoi(command_parts.at(4));
		varex::set_gain(det_index, value);
	}
	else if (parameter == "exposure_time") {
		double value = stod(command_parts.at(4));
		varex::set_exposure_time(det_index, value);
	}
	else if (parameter == "streaming_target") {
		varex::set_streaming_target(
			det_index,
			command_parts.at(4).c_str(),
			command_parts.at(5).c_str()
		);
	}
}

void get_param(int det_index, const vector<string>& command_parts) {
	string parameter = command_parts.at(3);
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
	// still needs to send it back...
}

string read_command(const char* recv_buffer) {
	std::istringstream recv_stream(recv_buffer);
	string recv_str;
	std::getline(recv_stream, recv_str, ';');
	return recv_str;
}

vector<string> split_command(const string& command) {
	return split(command, ':');
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

void send_success(SOCKET s) {
	send(s, "1", 1, NULL);
}

void send_failed(SOCKET s) {
	send(s, "0", 1, NULL);
}