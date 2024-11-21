#include "Socket.hpp"
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <unistd.h>
#include <cerrno>
Socket::Socket() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        throw std::runtime_error("Socket creation failed");

    pollfd server_pollfd;
    server_pollfd.fd = sock;
    server_pollfd.events = POLLIN;  // Monitor the server socket for incoming connections
    fds.push_back(server_pollfd);
	trigger = 0;
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
/*
void Socket::handleNewConnection() {
    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);
    int new_client_sock = accept(sock, (struct sockaddr*)&client_address, &client_len);

    if (new_client_sock == -1) {
        std::cerr << "Accept failed: " << strerror(errno) << std::endl;
        return; // Safely return if accept fails
    }

    // Add new client to the pollfd list, initially monitoring only for POLLIN
    struct pollfd client_pollfd = {new_client_sock, POLLIN, 0};
    fds.push_back(client_pollfd);

    std::cout << "New client connected on socket: " << new_client_sock << std::endl;

    // Send initial prompts
    User::sendText("Enter your username:\n", new_client_sock, client_pollfd);
        // Dynamically allocate strings to avoid fixed buffer sizes
        char username_buff[1024] = {0};
        ssize_t bytes_received = recv(new_client_sock, username_buff, sizeof(username_buff) - 1, 0);

        if (bytes_received <= 0) {
            std::cerr << "Failed to receive username from client: " << strerror(errno) << std::endl;
            close(new_client_sock);
            fds.pop_back();
            return;
        }
        username_buff[bytes_received] = '\0'; // Null-terminate received data
        std::string username(username_buff);

        // Ask for nickname
        User::sendText("Enter your nickname:\n", new_client_sock, client_pollfd);
            char nickname_buff[1024] = {0};
            bytes_received = recv(new_client_sock, nickname_buff, sizeof(nickname_buff) - 1, 0);

            if (bytes_received <= 0) {
                std::cerr << "Failed to receive nickname from client: " << strerror(errno) << std::endl;
                close(new_client_sock);
                fds.pop_back();
                return;
            }
            nickname_buff[bytes_received] = '\0'; // Null-terminate received data
            std::string nickname(nickname_buff);

            // Create a new User object
            User* newUser = new User(username, nickname, new_client_sock);
            client_sockets[username] = newUser;
            std::cout << "User connected: " << username << " (Nickname: " << nickname << ")\n";
			if (trigger == 0)
       			User::sendText("Enter your text:\n", new_client_sock, client_pollfd);

}
*/

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
   static char buff[1024];
	if (trigger == 0)
	{
		trigger = 1;
            int client_sock = fds[1].fd;
            int bytes_received = recv(client_sock, buff, sizeof(buff) - 1, 0);
            if (bytes_received > 0) {
                buff[bytes_received] = '\0';
                std::cout << "Message received: " << buff << std::endl;
			}
	}
			else if (trigger == 1)
			{
       			User::sendText(buff, fds[2].fd, fds[2]);
				trigger = 2;
			}

}

/*-----------------------------------------------------------------------------------------*/

int Socket::addClient() {
    if (fds[0].revents & POLLIN) {  // Server socket is always the first element
        try {
            char buff[1024];
            memset(buff, 0, 1024);
            std::string text = "USER";
           // handleNewConnection();
            struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);
    int new_client_sock = accept(sock, (struct sockaddr*)&client_address, &client_len);

    if (new_client_sock == -1) {
        std::cerr << "Accept failed: " << strerror(errno) << std::endl;
        return 1; // Safely return if accept fails
    }

    // Add new client to the pollfd list, initially monitoring only for POLLIN
    struct pollfd client_pollfd = {new_client_sock, POLLIN, 0};
    fds.push_back(client_pollfd);

    int bytes_received = recv(new_client_sock, buff, sizeof(buff) - 1, 0);
if (bytes_received > 0) {
    buff[bytes_received] = '\0'; // Safely null-terminate
} else {
    std::cerr << "Receive failed or client disconnected." << std::endl;
    
}
    std::cout << "Client says: " << buff << std::endl;
        const char* cap_reply = ":server CAP * LS :multi-prefix sasl";
        send(new_client_sock, cap_reply, strlen(cap_reply), 0);

     bytes_received = recv(new_client_sock, buff, sizeof(buff) - 1, 0);
if (bytes_received > 0) {
    buff[bytes_received] = '\0'; // Safely null-terminate
} else {
    std::cerr << "Receive failed or client disconnected." << std::endl;
    
}
    std::cout << "Client says: " << buff << std::endl;
         int bytesent =  send(new_client_sock, text.c_str(), 4, 0);
    std::cout << "bytesent: " << bytesent << std::endl;
         bytes_received = recv(new_client_sock, buff, sizeof(buff) - 1, 0);
if (bytes_received > 0) {
    buff[bytes_received] = '\0'; // Safely null-terminate
    std::cout << "Client says: " << buff << std::endl;

} else {
    std::cerr << "Receive failed or client disconnected." << std::endl;
    
}

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
