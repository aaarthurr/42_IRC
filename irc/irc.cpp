#include "irc.hpp" 

Socket::Socket() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        throw std::runtime_error("Socket creation failed");
    }
}

void Socket::connectToServer(const std::string& ip, int port) {
        
        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(port);

        if (inet_pton(AF_INET, ip.c_str(), &server_address.sin_addr) <= 0) {
            throw std::runtime_error("Invalid address/Address not supported");
        }

        if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) == -1)
        {
            throw std::runtime_error("Connection failed");
        }
}

void Socket::connectToClient(const std::string& ip, int port) {

    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        close(sock);
        throw std::runtime_error("Invalid address/Address not supported");

    }

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) == -1)
    {
        close(sock);
        throw std::runtime_error("Connection failed");
    }
}

Socket::~Socket() {
        close(sock);
}