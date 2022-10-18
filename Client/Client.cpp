#include <iostream>
#include <WinSock2.h>
#include <string>
#include <chrono>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

SOCKET connection;

std::string getCurrentTime() {
    auto rTime = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(rTime);

    return std::ctime(&end_time);
}

// Handler for receiving messages from other clients
void ClientHandler() {
    char msg[256];
    while (true) {
        recv(connection, msg, sizeof(msg), NULL);
        std::cout << msg << std::endl;
    }
    closesocket(connection);
}

int main(int argc, char* argv[]) {
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);

    char* ip = 0;
    int port = 0;

    if (argc < 1) {
        std::cout << "No server IP provided" << std::endl;
        exit(1);
    } else if (argc < 2) {
        std::cout << "No server port provided" << std::endl;
        exit(1);
    } else {
        ip = argv[1];
        port = atoi(argv[2]); // getting port number from the arguments
        std::cout << "Server ip is: " << ip << std::endl;
        std::cout << "Server port number is: " << port << std::endl;
    }

    /* loading library and checking if it was loaded
    first paremater is the requeted library
    second paratemet is the reference to wsaData */
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "Error" << std::endl;
        exit(1);
    }

    //if the library is loaded, we can add info for scoket address
    SOCKADDR_IN address;
    int addrSize = sizeof(address);
    address.sin_addr.s_addr = inet_addr(ip); //specifying IP
    address.sin_port = htons(port); //specifying port
    address.sin_family = AF_INET;

    ////asking client for a username
    //std::cout << "Enter your username:" << std::endl;
    //std::string username;
    //std::cin >> username;

    //connecting to the server
    connection = socket(AF_INET, SOCK_STREAM, NULL); // TCP connection
    if (connect(connection, (SOCKADDR*)&address, sizeof(address)) != 0) {
        std::cout << "Error: Connnection to the server failed" << std::endl;
        return 1;
    }
    std::cout << "Connection success" << std::endl;
    std::cout << "Connected to the server on " << getCurrentTime().c_str() << std::endl;

    //receiving initial message from the server
    char msg[256];
    recv(connection, msg, sizeof(msg), NULL);
    std::cout << "Initial message from Server received on " << getCurrentTime().c_str() << ": " << msg << std::endl;

    // creating thread
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

    std::string msg1;

    //create infinite cycle afer msg received from the server
    while (true) {
        std::getline(std::cin, msg1);
        send(connection, msg1.c_str(), sizeof(msg1), NULL);
        std::cout << "Sending message to the server and other clients on " << getCurrentTime().c_str() << std::endl;
        Sleep(10);
    }
    
    system("pause");
    return 0;
}
