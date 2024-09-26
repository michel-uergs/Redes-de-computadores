#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    const int PORT = 8080;
    char buffer[1024] = {0};

    // Step 1: Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        return -1;
    }

    // Step 2: Bind socket to IP/Port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return -1;
    }

    // Step 3: Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        return -1;
    }

    std::cout << "Waiting for a connection..." << std::endl;

    // Step 4: Accept the connection
    if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        return -1;
    }

    std::cout << "Connection established!" << std::endl;

    while (true) {
        // Step 5: Receive message from client
        memset(buffer, 0, sizeof(buffer)); // Clear buffer
        int valread = read(client_fd, buffer, sizeof(buffer));
        if (valread == 0) {
            std::cout << "Client disconnected!" << std::endl;
            break;
        }
        std::cout << "Client: " << buffer << std::endl;

        // Step 6: Send a message to the client
        std::string server_message;
        std::cout << "Server: ";
        std::getline(std::cin, server_message);
        send(client_fd, server_message.c_str(), server_message.length(), 0);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
