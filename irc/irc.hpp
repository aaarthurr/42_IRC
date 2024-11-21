#pragma once

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
#include "Channel.hpp"

class Channel;

class Socket {
private:
    int sock;
    struct sockaddr_in server_address;
    std::list<int> client_sockets;  // Use list to track client sockets
	Channel *channel;
public:
    Socket();
    void connectSocket(int port);
    void receiveMessages();
    void handleNewConnection();
	int message_processing(int client_sock, const char* buffer);
	void mode_command(const char* buffer);
    int getSock();
    ~Socket();
};