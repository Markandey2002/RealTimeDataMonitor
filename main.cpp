#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include "serial_utils.h"
#include "parser.h"

// Alert functions (you can customize thresholds)
void alertRPM(int rpm) {
    if (rpm > 6000) {
        std::cout << "[ALERT] High RPM detected: " << rpm << std::endl;
    }
}

void alertSpeed(int speed) {
    if (speed > 120) {
        std::cout << "[ALERT] Speed limit exceeded: " << speed << " km/h" << std::endl;
    }
}

int main() {
    //std::string port = "COM1"; // Adjust this to your actual port
    bool elmConnected = false;

    std::cout << "Attempting to connect to ELM327 on port " << port << "...\n";

    elmConnected = openSerialPort(port);
    if (!elmConnected) {
        std::cout << "⚠️ Could not open port " << port << ". Falling back to log file mode.\n";
    } else {
        // Give the device a moment
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        // Send reset command and read response
        sendCommand("ATZ");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::string resp = readResponse();

        if (resp.find("ELM327") == std::string::npos) {
            std::cout << "⚠️ Connected device is not ELM327. Falling back to log file mode.\n";
            closeSerialPort();
            elmConnected = false;
        }
    }

    if (elmConnected) {
        std::cout << "✅ ELM327 connected successfully on " << port << "\n";

        sendCommand("ATE0"); // Echo off
        sendCommand("ATL0"); // Linefeeds off
        sendCommand("ATS0"); // Spaces off

        while (true) {
            sendCommand("010C");  // Request RPM
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::string rpmResp = readResponse();
            int rpm = parseRPM(rpmResp);
            std::cout << "RPM: " << rpm << std::endl;
            alertRPM(rpm);

            sendCommand("010D");  // Request Speed
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::string speedResp = readResponse();
            int speed = parseSpeed(speedResp);
            std::cout << "Speed: " << speed << " km/h\n";
            alertSpeed(speed);

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        closeSerialPort();

    } else {
        std::cout << "⚠️  Running in log file mode...\n";

        std::ifstream logFile("obd_log.txt");
        if (!logFile.is_open()) {
            std::cerr << "❌ Error: Could not open obd_log.txt\n";
            return 1;
        }

        std::string line;
        while (std::getline(logFile, line)) {
            if (line.find("41 0C") != std::string::npos) {
                int rpm = parseRPM(line);
                std::cout << "[LOG] RPM: " << rpm << std::endl;
                alertRPM(rpm);
            } else if (line.find("41 0D") != std::string::npos) {
                int speed = parseSpeed(line);
                std::cout << "[LOG] Speed: " << speed << " km/h" << std::endl;
                alertSpeed(speed);
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        logFile.close();
    }

    return 0;
}
