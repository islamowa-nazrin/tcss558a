#include <iostream>
#include <string>
#include <WinSock2.h>
#include <chrono>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996) 


SOCKET connections[100]; // defining socket that allows 100 connections
int connIndex = 0; // counter for the client connections

std::string getCurrentTime() {
    auto rTime = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(rTime);

    return std::ctime(&end_time);
}

// client handler that allows proper messaging between clients
void ClientHandler(int k) {
    char msg_from_c[256];

    while (true) {
        recv(connections[k], msg_from_c, sizeof(msg_from_c), NULL); // receive a message from client with index k
        std::cout << "Message from client " << k << " received on " << getCurrentTime().c_str() << std::endl;
        std::string msg = "From client " + std::to_string(k) + ": " + msg_from_c;
        // send this message to all clients except the sender client
        for (int i = 0; i < connIndex; i++) {
            if (i == k) {
                continue;
            };
            send(connections[i], msg.c_str(), sizeof(msg), NULL);
            std::cout << "Message to client " << i << " received on " << getCurrentTime().c_str() << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    int port;
    
    if (argc < 1) {
        std::cout << "No port provided" << std::endl;
    } else {
        port = atoi(argv[1]); // getting port number from the arguments
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
    address.sin_addr.s_addr = NULL; //specifying IP
    address.sin_port = htons(port); //specifying port
    address.sin_family = AF_INET;

    /* creating socket
    first parameter identifies that internet protocls will be used,
    second parameter identifies TCP will be used
    */
    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);

    //binding address with socket
    bind(sListen, (SOCKADDR*)&address, sizeof(address));

    //listening to the port for clients
    listen(sListen, SOMAXCONN);

    SOCKET newConnection;
    for (int i = 0; i < 100; i++) {
        newConnection = accept(sListen, (SOCKADDR*)&address, &addrSize);

        if (newConnection == 0) {
            std::cout << "Error";
        }
        else {
            std::cout << "Client " << i << " connected on " << getCurrentTime().c_str() << std::endl;
            char msg[256] = "You are connected to the server";
            send(newConnection, msg, sizeof(msg), NULL);
            std::cout << "Sending client " << i << " initial message on " << getCurrentTime().c_str() << std::endl;

            connections[i] = newConnection; // add newly connected client to the array of all clients
            connIndex++;

            // create a thread for this client
            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
        }
    }
    system("pause");
    return 0;
}
