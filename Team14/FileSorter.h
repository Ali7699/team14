// FileSorter.h
#ifndef FILESORTER_H
#define FILESORTER_H

#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

class FileSorter {
public:
    // Sorts the file by VT values in ascending order, ignoring first 4 lines
    static bool sortFileByVT(const std::string& filename) {
        std::vector<LineData> lines;

        // Read all lines from the file
        if (!readLines(filename, lines)) {
            return false;
        }

        // Sort lines by VT
        std::sort(lines.begin(), lines.end(), [](const LineData& a, const LineData& b) {
            return a.vt < b.vt;
            });

        // Write sorted lines back to the file (including original first 4 lines)
        return writeLines(filename, lines);
    }

private:
    struct LineData {
        int vt;
        std::string line;

        LineData(const std::string& inputLine) : line(inputLine) {
            std::istringstream iss(inputLine);
            char type;
            int x, y, z;

            iss >> type >> x >> y >> z;
            vt = y;  // VT is the third number (second after type)
        }
    };

    static bool readLines(const std::string& filename, std::vector<LineData>& lines) {
        std::ifstream inFile(filename);
        if (!inFile) {
            return false;
        }

        std::string currentLine;
        int lineCount = 0;

        while (std::getline(inFile, currentLine)) {
            lineCount++;
            // Skip first 4 lines
            if (lineCount <= 4) continue;

            if (!currentLine.empty()) {
                lines.emplace_back(currentLine);
            }
        }
        return true;
    }

    static bool writeLines(const std::string& filename, const std::vector<LineData>& lines) {
        std::ifstream originalFile(filename);
        std::ofstream outFile(filename + ".tmp");
        if (!originalFile || !outFile) {
            return false;
        }

        // Copy first 4 lines unchanged
        std::string currentLine;
        for (int i = 0; i < 4 && std::getline(originalFile, currentLine); i++) {
            outFile << currentLine << '\n';
        }
        originalFile.close();

        // Write sorted lines
        for (const auto& data : lines) {
            outFile << data.line << '\n';
        }

        outFile.close();

        // Replace original file with the temporary one
        std::remove(filename.c_str());
        std::rename((filename + ".tmp").c_str(), filename.c_str());

        return true;
    }
};

#endif // FILESORTER_H