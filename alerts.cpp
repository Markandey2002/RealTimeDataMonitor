#include <iostream>

void alertRPM(int rpm) {
    if (rpm > 4000) {
        std::cout << "⚠️ ALERT: High RPM: " << rpm << "\n";
    }
}

void alertSpeed(int speed) {
    if (speed > 120) {
        std::cout << "⚠️ ALERT: Over Speeding: " << speed << " km/h\n";
    }
}