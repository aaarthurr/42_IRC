#include "Socket.hpp"

// Constructor: Create a server socket
Socket::Socket() {

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        throw std::runtime_error("Socket creation failed");
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds); 
}

// Bind and listen on the server socket
void Socket::connectSocket(int port)
{
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;  // Listen on all interfaces

    // Bind the socket to the address
    if (bind(sock, (struct sockaddr*)&server_address, sizeof(server_address)) == -1)
    {
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
int Socket::handleNewConnection()
{
    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);
    int new_client_sock = accept(sock, (struct sockaddr*)&client_address, &client_len);
    for (std::map<std::string, User*>::iterator it = client_sockets.begin(); it != client_sockets.end(); ++it) {
        if (new_client_sock == it->second->getClientSock())
            return (1);
    }
    if (new_client_sock == -1)
        throw std::runtime_error("Accept failed");
    char text[3][1024];
    User temp("temp", "guest", "temp", new_client_sock);
    memset(text, 0, sizeof(text)); // Initialize the entire array to zero

    temp.sendText("Enter your username:", new_client_sock);
    recv(sock, text[0], sizeof(text[0]) - 1, 0); // Reserve space for null-terminator
    text[0][sizeof(text[0]) - 1] = '\0'; // Manually null-terminate

 

    temp.sendText("Enter your nickname:", new_client_sock);
    recv(sock, text[1], sizeof(text[1]) - 1, 0); // Reserve space for null-terminator
    text[1][sizeof(text[1]) - 1] = '\0'; // Manually null-terminate


 

    temp.sendText("Enter your password:", new_client_sock);
    recv(sock, text[2], sizeof(text[2]) - 1, 0); // Reserve space for null-terminator
    text[2][sizeof(text[2]) - 1] = '\0'; // Manually null-terminate

        std::string username(text[0]);
    std::string nickname(text[1]);
    std::string password(text[2]);

 
    User *newUser = new User(username, nickname, password, new_client_sock);
    client_sockets[newUser->getUsername()] = newUser;  // Add the new client socket to the list
    std::cout << "New client connected:" << std::endl;
    std::cout << "Username : " << newUser->getUsername() << std::endl;
    std::cout << "Nickname : " << newUser->getNickname() << std::endl;
    std::cout << "Mdp : " << newUser->getMdp() << std::endl;
    FD_SET(new_client_sock, &readfds);
    return (0);
}

void    Socket::wait_for_act()
{
      // File descriptor set for select()
   // Add the server socket to the set

    int max_fd = sock;

    // Add all client sockets to the set
    for (std::map<std::string, User*>::iterator it = client_sockets.begin(); it != client_sockets.end(); ++it) {
        
        if (it->second->getClientSock() > max_fd) {
            max_fd = it->second->getClientSock();  // Keep track of the largest file descriptor
        }
    }

    // Wait for activity on any of the sockets (blocking call)
    int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
    if (activity < 0) {
        throw std::runtime_error("select() error");
    }
}
int Socket::AddClient() {

    int x = 0;
    // Check if the server socket is ready to accept a new connection
    if (FD_ISSET(sock, &readfds))
    {
        try
        {
           x = handleNewConnection();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    return (x);
}


// Get server socket
int Socket::getSock() {
    return sock;
}

// Destructor: Close sockets
Socket::~Socket() {
    std::map<std::string, User>::iterator it;
    for (std::map<std::string, User*>::iterator it = client_sockets.begin(); it != client_sockets.end(); ++it) {
        delete it->second;  // Free memory
    }
    close(sock);
}