#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>

const char* appDataPath = std::getenv("APPDATA");

std::string filepath = std::string (appDataPath) + R"(\Wireshark\profiles\SSH\preferences)";
int prefs_host_line = 457;
int prefs_interface = 485;

int handleCaptureProtocol(std::string input) {
    input = input.substr(10);

    size_t slashPosition = input.find('/');

    if (slashPosition == std::string::npos) {
        std::cerr << "Invalid input format. Missing '/' after IP address" << std::endl;
        sleep(2);
        return 1;
    }

    std::string host = input.substr(0, slashPosition);
    std::string interface = input.substr(slashPosition + 1);

    std::fstream theFile(filepath);
    if (!theFile) {
        std::cerr << "Error opening file: " << filepath << std::endl;
        sleep(2);
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
        sleep(2);
        return 1;
    }

    for (const auto& updatedLine : lines) {
        outputFile << updatedLine << '\n';
    }
    outputFile.close();

    const char* cmdCommand = R"("C:\Program Files\Wireshark\Wireshark.exe" -C SSH -i sshdump.exe -k)";
    return system(cmdCommand);
}


int handleVNCProtocol(std::string input) {
    input = input.substr(6);
    if (input.back() == '/') {
        input.pop_back();
    }
    std::string cmdCommand = R"("C:\Program Files\uvnc bvba\UltraVNC\vncviewer.exe" )" + input;
    std::cout << cmdCommand << std::endl;
    return system(cmdCommand.c_str());
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Insufficient arguments..." << std::endl;
        sleep(2);
        return 1;
    }
    std::string input(argv[1]);
    if (input.find("capture://") != std::string::npos) {
        return handleCaptureProtocol(input);
    }
    if (input.find("vnc://") != std::string::npos) {
        return handleVNCProtocol(input);
    }

    std::cerr << "Invalid url handler. It should be in ['capture','vnc']" << std::endl;
    sleep(3);
    return 1;
}