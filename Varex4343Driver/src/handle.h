#pragma once

#include <WinSock2.h>
#include <vector>
#include <string>

void handle_connection(SOCKET s);

std::vector<std::string> read_command(const char* recv_buffer);
std::vector<std::string> split(const std::string& str, char delimiter);