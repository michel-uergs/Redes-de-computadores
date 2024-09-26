#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    const char *server_ip = "127.0.0.1"; // Server IP address (localhost)
    const int PORT = 8080;
    char buffer[1024] = {0};

    // Step 1: Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    // Step 2: Define server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert server IP address from text to binary
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address or Address not supported" << std::endl;
        return -1;
    }

    // Step 3: Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    std::cout << "Connected to server!" << std::endl;

    while (true) {
        // Step 4: Send a message to the server
        std::string client_message;
        std::cout << "Client: ";
        std::getline(std::cin, client_message);
        send(sock, client_message.c_str(), client_message.length(), 0);

        // Step 5: Receive message from server
        memset(buffer, 0, sizeof(buffer)); // Clear buffer
        int valread = read(sock, buffer, sizeof(buffer));
        if (valread == 0) {
            std::cout << "Server disconnected!" << std::endl;
            break;
        }
        std::cout << "Server: " << buffer << std::endl;
    }

    close(sock);
    return 0;
}
//
//
//
