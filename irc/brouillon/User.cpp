#include "User.hpp"

User::User() : username("temp"), nickname("guest"), sock(-1)
{}

User::User(const User &usr): username(usr.username), nickname(usr.nickname), sock(usr.sock)
{}

User::User(std::string username, std::string nickname, int sock)
{
    this->username = username;
    this->nickname = nickname;
    this->sock = sock;
}

std::string User:: getUsername()
{
    return (this->username);
}

std::string User:: getNickname()
{
    return (this->nickname);
}

int User::getClientSock()
{
    return (sock);
}


void User::sendText(std::string text, int dest_sock,  struct pollfd fds)
{
	text.push_back('\n');
	if (fds.revents & POLLERR)
	{
			std::cout << "ok" << std::endl;
	}
		ssize_t bytes_sent = send(dest_sock, text.c_str(), text.length(), 0); // Removed MSG_CONFIRM
		if (bytes_sent == -1) 
			std::cerr << "Failed to send message: " <<  std::endl;
	//}
}



User::~User()
{
    if (sock != -1)
        close(sock);
}