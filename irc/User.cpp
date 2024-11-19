#include "User.hpp"

User::User() : username("temp"), nickname("guest"), mdp("temp"), sock(-1)
{}

User::User(const User &usr): username(usr.username), nickname(usr.nickname), mdp(usr.mdp), sock(usr.sock)
{}

User::User(std::string username, std::string nickname, std::string mdp, int sock)
{
    this->username = username;
    this->nickname = nickname;
    this->mdp = mdp;
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

std::string User::getMdp()
{
    return (this->mdp);
}

int User::getClientSock()
{
    return (sock);
}

void User::sendText(std::string text, int dest_sock)
{
    ssize_t bytes_sent = send(dest_sock, text.c_str(), text.length(), MSG_CONFIRM);
    if (bytes_sent == -1) {
        std::cerr << "Failed to send message" << std::endl;
    } 
}

User::~User()
{
    if (sock != -1)
        close(sock);
    std::cout << "User " << this->nickname << " just disconnected" << std::endl;
}