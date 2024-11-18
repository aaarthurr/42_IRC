#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <iostream>
#include <cstring>
#include <netinet/in.h>

class Socket{

    private:
        int sock;
        struct sockaddr_in server_address;
    public:
        Socket();
        void connectToServer(const std::string& ip, int port);
        void connectToClient(const std::string& ip, int port);
     ~Socket();   
};