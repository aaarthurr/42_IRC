#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <iostream>
#include <cstring>

int main() {
    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    // Define the server address
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address)); // Zero out the structure
    server_address.sin_family = AF_INET;               // IPv4
    server_address.sin_port = htons(8080);             // Port 8080

    // Convert IP address to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        std::cerr << "Invalid address/Address not supported\n";
        close(sock);
        return 1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Connection failed\n";
        close(sock);
        return 1;
    }

    std::cout << "Connected to the server successfully!\n";

    std::string mess;

  while (std::cin >> mess && mess != "exit") {
        ssize_t bytes_sent = send(sock, mess.c_str(), mess.length(), MSG_CONFIRM);
        if (bytes_sent == -1) {
            std::cerr << "Failed to send message\n";
            break;
        }
  }
    // Close the socket
    close(sock);

    return 0;
}
