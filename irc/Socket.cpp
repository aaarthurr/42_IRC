#include "Socket.hpp"
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <unistd.h>

Socket::Socket() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        throw std::runtime_error("Socket creation failed");

    pollfd server_pollfd;
    server_pollfd.fd = sock;
    server_pollfd.events = POLLIN;  // Monitor the server socket for incoming connections
    fds.push_back(server_pollfd);
}

/*-----------------------------------------------------------------------------------------*/

void Socket::connectSocket(int port) {
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;  

    if (bind(sock, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        close(sock);
        throw std::runtime_error("Binding failed");
    }

    if (listen(sock, 5) == -1) {
        close(sock);
        throw std::runtime_error("Listening failed");
    }

    std::cout << "Server is listening on port " << port << "...\n";
}

/*-----------------------------------------------------------------------------------------*/

void Socket::handleNewConnection() {
    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);
    int new_client_sock = accept(sock, (struct sockaddr*)&client_address, &client_len);

    if (new_client_sock == -1)
        throw std::runtime_error("Accept failed");

    char text[2][1024];
    memset(text, 0, sizeof(text));

    User::sendText("Enter your username:", new_client_sock, fds);
    recv(new_client_sock, text[0], 1023, 0);
    text[0][1023] = '\0';

    User::sendText("Enter your nickname:", new_client_sock, fds);
    recv(new_client_sock, text[1], 1023, 0);
    text[1][1023] = '\0';

    std::string username(text[0]);
    std::string nickname(text[1]);

    User* newUser = new User(username, nickname, new_client_sock);
    client_sockets[newUser->getUsername()] = newUser;
    std::cout << "New client connected: " << newUser->getUsername() << std::endl;

    struct pollfd client_pollfd = {new_client_sock, POLLIN | POLLOUT, 0};
    fds.push_back(client_pollfd);  // Add client socket to pollfd list
}

/*-----------------------------------------------------------------------------------------*/

void Socket::handleMess() {
    char buff[1024];

    for (size_t i = 1; i < fds.size(); ++i) {  // Skip the first entry (server socket)
        if (fds[i].revents & POLLIN) {  // Check if the client has sent data
            int client_sock = fds[i].fd;
            int bytes_read = recv(client_sock, buff, sizeof(buff) - 1, 0);
            if (bytes_read <= 0 || !strncmp("exit", buff, 4)) {
                // Handle disconnection
                std::cout << "Client disconnected: " << client_sock << std::endl;
                close(client_sock);

                // Remove the client from pollfd and client_sockets
                std::map<std::string, User*>::iterator it = client_sockets.begin();
               for(  ; it->second->getClientSock() != client_sock;)
                it++;;
                if (it != client_sockets.end()) {
                    client_sockets.erase(it);
                }

                fds.erase(fds.begin() + i);
                --i;  // Adjust index after erasing to ensure no client is skipped
            } else {
                buff[bytes_read] = '\0';  // Null-terminate the received data
                std::cout << "Message from client " << client_sock << ": " << buff << std::endl;
            }
        }
    }
}

/*-----------------------------------------------------------------------------------------*/

int Socket::wait_for_act() {
    int activity = poll(&fds[0], fds.size(), -1);  // Wait indefinitely for events
    if (activity < 0) {
        throw std::runtime_error("poll() error");
    }
    return activity;
}

/*-----------------------------------------------------------------------------------------*/

void Socket::handleRequest() {
    char buff[1024];
    for (size_t i = 0; i < fds.size(); ++i) {
        if (fds[i].revents & POLLIN) {
            int client_sock = fds[i].fd;
            int bytes_received = recv(client_sock, buff, sizeof(buff) - 1, 0);
            if (bytes_received > 0) {
                buff[bytes_received] = '\0';
                std::cout << "Message received: " << buff << std::endl;
            } else {
                std::cerr << "Failed to receive data from socket " << client_sock << std::endl;
            }
        }
    }
}

/*-----------------------------------------------------------------------------------------*/

int Socket::addClient() {
    if (fds[0].revents & POLLIN) {  // Server socket is always the first element
        try {
            handleNewConnection();
        } catch (const std::exception& e) {
            std::cerr << "Error adding client: " << e.what() << std::endl;
        }
    }
    return 0;
}

/*-----------------------------------------------------------------------------------------*/

int Socket::getSock() {
    return sock;
}

/*-----------------------------------------------------------------------------------------*/

Socket::~Socket() {
    for (std::map<std::string, User*>::iterator it = client_sockets.begin(); it != client_sockets.end(); ++it) {
        close(it->second->getClientSock());
        delete it->second;
    }
    client_sockets.clear();

    for (size_t i = 0; i < fds.size(); ++i) {
        close(fds[i].fd);
    }
    fds.clear();
}
