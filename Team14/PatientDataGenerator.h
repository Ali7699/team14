#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>

class PatientDataGenerator {
private:
    // Helper function to flip a coin (returns true or false)
    bool flipCoin() {
        return rand() % 2 == 0;
    }

    // Helper function to get a random number in range [min, max]
    int getRandomNumber(int min, int max) {
        return min + (rand() % (max - min + 1));
    }

    // Helper function to track used appointment times
    bool isTimeUsed(int time, const std::vector<int>& usedTimes, int maxUses) {
        int count = 0;
        for (int usedTime : usedTimes) {
            if (usedTime == time) {
                count++;
                if (count >= maxUses) {
                    return true;
                }
            }
        }
        return false;
    }

    // Generate a random letter (U, X, or E)
    char getRandomLetter() {
        char letters[] = { 'U', 'X', 'E' };
        return letters[rand() % 3];
    }

public:
    PatientDataGenerator() {
        // Seed the random number generator
        srand(static_cast<unsigned int>(time(nullptr)));
    }

    // Main function to generate patient data and write to file
    void generatePatientData(int patientCount, const std::string& filename) {
        std::vector<int> usedAppointmentTimes;

        // Open output file
        std::ofstream outputFile("Input/" + filename);
        if (!outputFile.is_open()) {
            // Try creating without the "Input/" directory if it failed
            outputFile.open(filename);
            if (!outputFile.is_open()) {
                
                return;
            }
        }

        // Increase the range of possible appointment times when generating many records
        int maxAppointmentTime = 199;
        if (patientCount > 199) {
            maxAppointmentTime = patientCount;
        }

        for (int i = 1; i <= patientCount; i++) {
            // Flip a coin to determine if it's an N or R appointment
            char appointmentType = flipCoin() ? 'N' : 'R';

            // Generate appointment time (PT) with larger range for many patients
            // and relax the reuse constraint to avoid infinite loops
            int appointmentTime;
            int maxAttempts = 100; // Avoid infinite loop
            int attempts = 0;

            do {
                appointmentTime = getRandomNumber(1, maxAppointmentTime);
                attempts++;
                // Break out if we've tried too many times
                if (attempts > maxAttempts) {
                    break;
                }
            } while (isTimeUsed(appointmentTime, usedAppointmentTimes, 5)); // Allow more reuse

            usedAppointmentTimes.push_back(appointmentTime);

            // Generate arrival time (VT)
            int arrivalTime;

            // Flip a coin to determine if patient is late or early
            bool isLate = flipCoin();
            if (isLate) {
                // Patient is late: VT > PT
                arrivalTime = appointmentTime + getRandomNumber(1, 10);
            }
            else {
                // Patient is early: PT > VT
                // Make sure we don't get a negative or zero arrival time
                int maxEarly = (appointmentTime > 10) ? 10 : (appointmentTime - 1);
                if (maxEarly < 1) maxEarly = 1;
                arrivalTime = appointmentTime - getRandomNumber(1, maxEarly);
                if (arrivalTime < 1) arrivalTime = 1;
            }

            // Generate number of following letters (1-3)
            int numLetters = getRandomNumber(1, 3);

            // Start building the output line
            std::string line = "";
            line += appointmentType;
            line += " " + std::to_string(i) + "  " + std::to_string(appointmentTime) + " " + std::to_string(arrivalTime);

            // Add the letters and their associated numbers
            for (int j = 0; j < numLetters; j++) {
                char letter = getRandomLetter();
                int number = getRandomNumber(1, 9); // Using 1-9 instead of 1-20 as seen in the example
                line += " " + std::string(1, letter) + " " + std::to_string(number);
            }

            // Write the line to file
            outputFile << line << std::endl;

            // Periodically flush the file to avoid buffer issues
            if (i % 100 == 0) {
                outputFile.flush();
            }
        }

        // Add a period at the end as seen in the example
        outputFile << "." << std::endl;

        // Close the file
        outputFile.close();

    }
};

