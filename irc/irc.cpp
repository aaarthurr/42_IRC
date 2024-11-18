#include "irc.hpp"

// Constructor: Create a server socket
Socket::Socket() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        throw std::runtime_error("Socket creation failed");
    }
}

// Bind and listen on the server socket
void Socket::connectSocket(int port) {
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;  // Listen on all interfaces

    // Bind the socket to the address
    if (bind(sock, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        close(sock);
        throw std::runtime_error("Binding failed");
    }

    // Listen for incoming connections
    if (listen(sock, 5) == -1) {
        close(sock);
        throw std::runtime_error("Listening failed");
    }

    std::cout << "Server is listening on port " << port << "...\n";
}

// Handle new incoming connections
void Socket::handleNewConnection() {
    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);
    int new_client_sock = accept(sock, (struct sockaddr*)&client_address, &client_len);

    if (new_client_sock == -1) {
        std::cerr << "Accept failed\n";
        return;
    }

    std::cout << "New client connected!\n";
    client_sockets.push_back(new_client_sock);  // Add the new client socket to the list
}

// Receive messages from all connected clients
void Socket::receiveMessages() {
    fd_set readfds;  // File descriptor set for select()
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);  // Add the server socket to the set

    int max_fd = sock;

    // Add all client sockets to the set
    std::list<int>::iterator it;

    for (it = client_sockets.begin(); it != client_sockets.end(); ++it) {
        FD_SET(*it, &readfds);
        if (*it > max_fd) {
            max_fd = *it;  // Keep track of the largest file descriptor
        }
    }

    // Wait for activity on any of the sockets (blocking call)
    int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
    if (activity < 0) {
        std::cerr << "select() error\n";
        return;
    }

    // Check if the server socket is ready to accept a new connection
    if (FD_ISSET(sock, &readfds)) {
        handleNewConnection();
    }
    int x = 0;
    // Check all client sockets for incoming data
    for (it = client_sockets.begin(); it != client_sockets.end(); ) {
        int client_sock = *it;
        if (FD_ISSET(client_sock, &readfds)) {
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            ssize_t bytes_received = recv(client_sock, buffer, sizeof(buffer) - 1, 0);

            if (bytes_received == -1) {
                std::cerr << "Failed to receive message\n";
                close(client_sock);
                it = client_sockets.erase(it);  // Remove client socket
                continue;
            } else if (bytes_received == 0) {
                std::cout << "Client disconnected\n";
                close(client_sock);
                it = client_sockets.erase(it);  // Remove client socket
                continue;
            }

            buffer[bytes_received] = '\0';  // Null-terminate the buffer
            std::cout << "Client " << x <<" says: " << buffer << "\n";

            // Handle client message or exit condition
            if (strcmp(buffer, "exit") == 0) {
                std::cout << "Client " << x << " requested to close the connection.\n";
                close(client_sock);
                it = client_sockets.erase(it);  // Remove client socket
                continue;
            }
        }
        ++it;
        x++;
    }
}

// Get server socket
int Socket::getSock() {
    return sock;
}

// Destructor: Close sockets
Socket::~Socket() {
    std::list<int>::iterator it;
    for (it = client_sockets.begin(); it != client_sockets.end(); ++it) {
        close(*it);
    }
    close(sock);
}