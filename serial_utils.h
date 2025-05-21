#ifndef SERIAL_UTILS_H
#define SERIAL_UTILS_H
#include <windows.h>
#include <string>

bool openSerialPort(const std::string& portName);
void sendCommand(const std::string& cmd);
std::string readResponse();
void closeSerialPort();

#endif