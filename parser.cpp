#include "parser.h"
#include <sstream>

int hexToInt(const std::string& hex) {
    return std::stoi(hex, nullptr, 16);
}

int parseRPM(const std::string& raw) {
    size_t pos = raw.find("41 0C");
    if (pos == std::string::npos) return -1;
    std::istringstream iss(raw.substr(pos));
    std::string _, A, B;
    iss >> _ >> _ >> A >> B;
    return ((hexToInt(A) * 256) + hexToInt(B)) / 4;
}

int parseSpeed(const std::string& raw) {
    size_t pos = raw.find("41 0D");
    if (pos == std::string::npos) return -1;
    std::istringstream iss(raw.substr(pos));
    std::string _, speedHex;
    iss >> _ >> _ >> speedHex;
    return hexToInt(speedHex);
}