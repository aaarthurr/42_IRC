#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <map>  // C++98 does not support dynamic array resizing in vector
#include <vector>
#include <poll.h>
#include "User.hpp"

class Socket {
private:
    int sock;
    struct sockaddr_in server_address;
    std::map<std::string, User*> client_sockets;  // Use list to track client sockets
    std::vector<pollfd> fds;
    std::vector<std::string> buffer;
public:
    Socket();
    void    connectSocket(int port);
    int    addClient();
    void    handleNewConnection();
    int     getSock();
    User    setUser(int sock);
    int    wait_for_act();
    void    handleMess();
    void    handleRequest();
	int trigger;

    ~Socket();
};