
   //CLIENT CODE...........



#include <iostream>
#include<sstream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

void printBoard(char board[3][3]) {
    std::cout << "   |   |   " << std::endl;
    std::cout << " " << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << std::endl;
    std::cout << "___|___|___" << std::endl;
    std::cout << "   |   |   " << std::endl;
    std::cout << " " << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << std::endl;
    std::cout << "___|___|___" << std::endl;
    std::cout << "   |   |   " << std::endl;
    std::cout << " " << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << std::endl;
    std::cout << "   |   |   " << std::endl;
    std::cout << "________________________________________________________________" << std::endl;
}

int main() {
    WSADATA wsaData;
    int iResult;

    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char board[3][3];

    // Initializing Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Socket creation error" << std::endl;
        WSACleanup();
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection Failed" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Game loop
    while (true) {
        // Receive board state from server
        recv(sock, (char *)board, sizeof(board), 0);

        // Print the current state of the board
        printBoard(board);

        // Player input
        std::cout << "Enter your move (row col): ";
        int row, col;
        std::cin >> row >> col;

        // Convert row and col to string
        std::string move = std::to_string(row) + " " + std::to_string(col);

        // Send move to server
        send(sock, move.c_str(), move.length(), 0);

        // Clear buffer
        memset(buffer, 0, sizeof(buffer));

        // Receive updated game state from server
        recv(sock, (char *)board, sizeof(board), 0);
    }

    closesocket(sock);
    WSACleanup();         
    return 0;
}


//g++ -o client client.cpp -lws2_32    to run client code in terminal.....
