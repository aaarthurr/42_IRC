#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <list>  // C++98 does not support dynamic array resizing in vector
#include <vector>

class Socket {
private:
    int sock;
    struct sockaddr_in server_address;
    std::list<int> client_sockets;  // Use list to track client sockets

public:
    Socket();
    void connectSocket(int port);
    void receiveMessages();
    void handleNewConnection();
    int getSock();
    ~Socket();
};