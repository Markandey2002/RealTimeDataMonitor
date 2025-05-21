#include "serial_utils.h"
#include <iostream>

HANDLE hSerial;

bool openSerialPort(const std::string& portName) {
    hSerial = CreateFile(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open port.\n";
        return false;
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    GetCommState(hSerial, &dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;

    SetCommState(hSerial, &dcbSerialParams);
    return true;
}

void sendCommand(const std::string& cmd) {
    std::string fullCmd = cmd + "\r";
    DWORD written;
    WriteFile(hSerial, fullCmd.c_str(), fullCmd.length(), &written, NULL);
    Sleep(100);
}

std::string readResponse() {
    char buffer[256];
    DWORD bytesRead;
    ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
    buffer[bytesRead] = '\0';
    return std::string(buffer);
}

void closeSerialPort() {
    CloseHandle(hSerial);
}