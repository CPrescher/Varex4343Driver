#pragma once

#include <WinSock2.h>
#include <vector>
#include <string>

void handle_connection(SOCKET s);

void handle_command(const std::string& command);
void set_param(int det_index, const std::vector<std::string>& command_parts);
void get_param(int det_index, const std::vector<std::string>& command_parts);


std::string read_command(const char* recv_buffer);
std::vector<std::string> split_command(const std::string& command);
std::vector<std::string> split(const std::string& str, char delimiter);

void send_success(SOCKET s);
void send_failed(SOCKET s);