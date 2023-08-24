#include <iostream>
#include <fstream>
#include <vector>
#include <string>



const char* appDataPath = std::getenv("APPDATA");

std::string filepath = std::string (appDataPath) + R"(\Wireshark\profiles\SSH\preferences)";
int prefs_host_line = 457;
int prefs_interface = 485;


int main(int argc, char* argv[]) {
    std::string input(argv[1]);
    if (input.substr(0, 10) != "capture://") {
        std::cerr << "Invalid input format. It should start with 'capture://'" << std::endl;
        return 1;
    }
    input = input.substr(10);

    size_t slashPosition = input.find('/');

    if (slashPosition == std::string::npos) {
        std::cerr << "Invalid input format. Missing '/' after IP address" << std::endl;
        return 1;
    }

    std::string host = input.substr(0, slashPosition);
    std::string interface = input.substr(slashPosition + 1);



    std::fstream theFile(filepath);
    if (!theFile) {
        std::cerr << "Error opening file: " << filepath << std::endl;
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(theFile, line)) {
        lines.push_back(line);
    }
    theFile.close();


    lines[prefs_host_line - 1] = "extcap.sshdump_exe.remotehost: " + host;
    lines[prefs_interface - 1] = "extcap.sshdump_exe.remoteinterface: " + interface;

    std::ofstream outputFile(filepath);
    if (!outputFile) {
        std::cerr << "Error opening file for writing: " << filepath << std::endl;
        return 1;
    }

    for (const auto& updatedLine : lines) {
        outputFile << updatedLine << '\n';
    }
    outputFile.close();

    const char* cmdCommand = R"("C:\Program Files\Wireshark\Wireshark.exe" -C SSH -i sshdump.exe -k)";
    return system(cmdCommand);
}
