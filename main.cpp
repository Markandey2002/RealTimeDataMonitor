#include <iostream> 
#include <fstream>  
#include <string>   
#include <vector>   
#include <chrono>   
#include <random>   
#include <iomanip>  
#include <thread>   


void writeCsvHeader(std::ofstream& outputFile);
// Writes a data row to the CSV file.
void writeCsvData(std::ofstream& outputFile, long long timestamp, double speed, double rpm, double fuelLevel, double engineTemp);

int main() {
    
    // This file will contain dummy data, simulating the output from an OBD-II system.
    const std::string filename = "obd_dummy_data.csv";
    // Create an output file stream object.
    std::ofstream outputFile(filename);

    // Check if the file was opened successfully.
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return 1; // Indicate an error.
    }

    
    std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    // Define distributions for generating random values within plausible ranges.
    std::uniform_real_distribution<> dist_speed(0.0, 120.0); // Speed in km/h (0 to 120)
    std::uniform_real_distribution<> dist_rpm(600.0, 5500.0); // RPM (600 to 5500)
    std::uniform_real_distribution<> dist_fuel(5.0, 95.0); // Fuel level in % (5% to 95%)
    std::uniform_real_distribution<> dist_temp(80.0, 100.0); // Engine temperature in Celsius (80 to 100)

    
    writeCsvHeader(outputFile);

    // Simulate data acquisition for a certain number of iterations.
    const int numReadings = 200; // Generate 200 data points for analysis.
    std::cout << "Generating " << numReadings << " dummy data readings and saving to CSV..." << std::endl;

    for (int i = 0; i < numReadings; ++i) {
        // Get the current timestamp (in milliseconds since epoch).
        long long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();

        // Generate simulated sensor values using the defined distributions.
        double speed = dist_speed(rng);
        double rpm = dist_rpm(rng);
        double fuelLevel = dist_fuel(rng);
        double engineTemp = dist_temp(rng);

        // Write the simulated data to the CSV file.
        writeCsvData(outputFile, timestamp, speed, rpm, fuelLevel, engineTemp);

        // Print progress to console every 20 readings.
        if ((i + 1) % 20 == 0) {
            std::cout << "  " << (i + 1) << " readings written." << std::endl;
        }

        // Simulate a small delay between readings to mimic real-time acquisition.
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 50 milliseconds delay
    }

    // Close the output file.
    outputFile.close();
    std::cout << "Dummy data generation complete. Data saved to " << filename << std::endl;

    return 0; // Indicate successful execution.
}

/**
 * @brief Writes the header row to the CSV file.
 * @param outputFile The output file stream.
 */
void writeCsvHeader(std::ofstream& outputFile) {
    outputFile << "Timestamp_ms,Speed_kmh,RPM,Fuel_Percent,Engine_Temp_C\n";
}


void writeCsvData(std::ofstream& outputFile, long long timestamp, double speed, double rpm, double fuelLevel, double engineTemp) {
    // Use std::fixed and std::setprecision to format floating-point numbers
    // for consistent output in the CSV file.
    outputFile << timestamp << ","
               << std::fixed << std::setprecision(2) << speed << ","
               << std::fixed << std::setprecision(0) << rpm << "," // RPM typically displayed as integer
               << std::fixed << std::setprecision(1) << fuelLevel << ","
               << std::fixed << std::setprecision(1) << engineTemp << "\n";
}
