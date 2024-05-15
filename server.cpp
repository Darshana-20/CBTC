
//SERVER CODE...............


#include <iostream>
#include <winsock2.h>   //library for socket.......

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define MAX_CLIENTS 2

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
    std::cout << "________________________________________________________________________"<<std::endl;
}

bool checkWin(char board[3][3], char symbol) {
    // Check rows, columns, and diagonals for winning condition
    for (int i = 0; i < 3; ++i) {
        if ((board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol) || // Rows
            (board[0][i] == symbol && board[1][i] == symbol && board[2][i] == symbol) || // Columns
            (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) || // Diagonal 1
            (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol)) { // Diagonal 2
            return true;
        }
    }
    return false;
}

bool checkDraw(char board[3][3]) {
    // Check if the board is full (draw condition)
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ') {
                return false; // If any empty cell found, the game is not a draw
            }
        }
    }
    return true; // All cells are filled, indicating a draw
}

int main() {
    WSADATA wsaData;
    int iResult;

    SOCKET server_fd, client_sockets[MAX_CLIENTS];
    struct sockaddr_in server, client;
    int opt = 1;
    int addrlen = sizeof(client);
    char buffer[1024] = {0};
    char board[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
    char symbol = 'X'; // Player 1 starts with symbol X
    bool gameOver = false;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt)) == SOCKET_ERROR) {
        std::cerr << "setsockopt failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    if (listen(server_fd, MAX_CLIENTS) == SOCKET_ERROR) {
        std::cerr << "listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "Waiting for players to join..." << std::endl;

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if ((client_sockets[i] = accept(server_fd, (struct sockaddr *)&client, &addrlen)) == INVALID_SOCKET) {
            std::cerr << "accept failed with error: " << WSAGetLastError() << std::endl;
            closesocket(server_fd);
            WSACleanup();
            return 1;
        }
        std::cout << "Player " << i + 1 << " connected." << std::endl;
    }

    while (!gameOver)   // Iteration loop for taking the moves of the players one after the other......
     {
        // Iterate through each player
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            // Print the current state of the board
            printBoard(board);

            // Send the board state to the current player
            send(client_sockets[i], (char *)board, sizeof(board), 0);

            // Receive move from the current player
            recv(client_sockets[i], buffer, 1024, 0);

            // Process the received move
            int row = buffer[0] - '0';
            int col = buffer[2] - '0';
            if (board[row][col] == ' ') {
                board[row][col] = symbol;
                // Check for win condition
                if (checkWin(board, symbol)) {
                    std::cout << "Player " << symbol << " wins!" << std::endl;
                    gameOver = true;
                    break;
                } else if (checkDraw(board)) {
                    std::cout << "It's a draw!" << std::endl;
                    gameOver = true;
                    break;
                } else {
                    // Switch player symbol
                    symbol = (symbol == 'X') ? 'O' : 'X';
                }
                // Send the updated board state back to the client
                send(client_sockets[i], (char *)board, sizeof(board), 0);
            } else {
                std::cout << "Invalid move. Try again." << std::endl;
                i--; // Re-try the current player's move
            }

            // Clear buffer
            memset(buffer, 0, sizeof(buffer));
        }
    }

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        closesocket(client_sockets[i]);
    }
    closesocket(server_fd);
    WSACleanup();
    return 0;
}


//g++ -o server server.cpp -lws2_32   to  run server code in terminal....