#include "../../../include/pin/network.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
int main() {
    // Define the server's details
    const char* server_ip = "127.0.0.1";
    int server_port = 8080;

    // Create a socket and connect to the server
    int sock = 0;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);

    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address / Address not supported" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    // Continuously receive data
    FuncBranchInfo info;
    std::ofstream outfile("branch_function_info.txt");
    while (read(sock, &info, sizeof(info)) > 0) {
        switch (info.type) {
            case MessageType::FUNC_CALL:
                std::cout << "Function: " << info.message.funcInfo.funcName << " Address : 0x" << 
                  info.message.funcInfo.funcAddr<< std::endl;
                outfile<< "Function: " << info.message.funcInfo.funcName <<  " Address : 0x" << 
                  info.message.funcInfo.funcAddr<< std::endl; 
                break;
            case MessageType::BRANCH_INFO:
                std::cout << " Source Address: 0x" << info.message.branchInfo.sourceAddr
                          << ", Target Address: 0x" << info.message.branchInfo.targetAddr
                          << ", Taken: " << (info.message.branchInfo.taken ? "Yes" : "No")
                          << std::endl;
                outfile <<  " Source Address: 0x" << info.message.branchInfo.sourceAddr
                            << ", Target Address: 0x" << info.message.branchInfo.targetAddr
                            << ", Taken: " << (info.message.branchInfo.taken ? "Yes" : "No")
                            << std::endl; 
                break;
             case MessageType::END_OF_DATA:
                std::cout << "End of data received, closing the connection" << std::endl;
                outfile << "End of data received, closing the connection" << std::endl;
                outfile.close();
                close(sock);
                break;
            default:
                std::cerr << "Unknown message type received" << std::endl;
                break;
        }
    }

    std::cout << "Connection closed by server" << std::endl;

    return 0;
}
