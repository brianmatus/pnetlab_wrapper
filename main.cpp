#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>

const char* appDataPath = std::getenv("APPDATA");

std::string wiresharkProfilePath = std::string (appDataPath) + R"(\Wireshark\profiles\SSH\preferences)";
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

    std::fstream theFile(wiresharkProfilePath);
    if (!theFile) {
        std::cerr << "Error opening file: " << wiresharkProfilePath << std::endl;
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

    std::ofstream outputFile(wiresharkProfilePath);
    if (!outputFile) {
        std::cerr << "Error opening file for writing: " << wiresharkProfilePath << std::endl;
        sleep(2);
        return 1;
    }

    for (const auto& updatedLine : lines) {
        outputFile << updatedLine << '\n';
    }
    outputFile.close();

    const char* cmdCommand = R"(powershell -window hidden -command "&\"C:\Program Files\Wireshark\Wireshark.exe\" -C SSH -i sshdump.exe -k -platform windows:darkmode=2")";
    //const char* cmdCommand = R"(powershell -window hidden -command "&\"C:\Program Files\Wireshark\Wireshark.exe\" -C SSH -i sshdump.exe -k")";
    //std::cout << cmdCommand << std::endl;
    return system(cmdCommand);
}


int handleVNCProtocol(std::string input) {
    input = input.substr(6);
    if (input.back() == '/') {
        input.pop_back();
    }
    std::string cmdCommand = R"(powershell -window hidden -command "&\"C:\Program Files\uvnc bvba\UltraVNC\vncviewer.exe\" )" + input + "\"";
    //std::cout << cmdCommand << std::endl;
    return system(cmdCommand.c_str());
}

int handleTelnetProtocol(const std::string& input) {
    std::cout << input << std::endl;
    
    std::string cmdCommand = R"(powershell -command "&\"C:\Program Files (x86)\NetSarang\Xshell 7\Xshell.exe\" -url )" + input + "\"";
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

    if (input.find("telnet://") != std::string::npos) {
        std::cout << "Telnet handling" << std::endl;
        
        return handleTelnetProtocol(input);
    }

    std::cerr << "Invalid url handler. It should be in ['capture', 'vnc', 'telnet']" << std::endl;
    sleep(3);
    return 1;
}