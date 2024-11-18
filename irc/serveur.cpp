#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    // Create a socket
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    // Define the server address
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address)); // Zero out the structure
    server_address.sin_family = AF_INET;               // IPv4
    server_address.sin_port = htons(8080);             // Port 8080
    server_address.sin_addr.s_addr = INADDR_ANY;       // Listen on all interfaces

    // Bind the socket to the address
    if (bind(server_sock, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Binding failed\n";
        close(server_sock);
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_sock, 5) == -1) {
        std::cerr << "Listening failed\n";
        close(server_sock);
        return 1;
    }

    std::cout << "Server is listening on port 8080...\n";

    // Accept a connection
    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);
    int client_sock = accept(server_sock, (struct sockaddr*)&client_address, &client_len);
    if (client_sock == -1) {
        std::cerr << "Accept failed\n";
        close(server_sock);
        return 1;
    }

    std::cout << "Connection accepted!\n";

    // Close sockets
   char buffer[1024];
    std::string mess;

    while (true) {
        // Receive data from the client
        memset(buffer, 0, sizeof(buffer)); // Clear the buffer
        ssize_t bytes_received = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received == -1) {
            std::cerr << "Failed to receive message\n";
            break;
        } else if (bytes_received == 0) {
            std::cout << "Client disconnected\n";
            break;
        }

        // Convert received buffer to string and print it
        buffer[bytes_received] = '\0'; // Null-terminate the buffer
        mess = buffer;

        std::cout << "Client says: " << mess << "\n";

        // Check if the client wants to exit
        if (mess == "exit") {
            std::cout << "Client requested to close the connection.\n";
            break;
        }
    }
        close(client_sock);
        close(server_sock);
    return 0;
}
