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


void User::sendText(std::string text, int dest_sock, std::vector<pollfd> const& fds)
{
    bool can_send = false;

    // Loop through the pollfd vector to find the correct fd for the destination socket
    for (size_t i = 0; i < fds.size(); ++i) {
        if (fds[i].fd == dest_sock) {
            // Check if the socket is ready for writing (POLLOUT event)
            if (fds[i].revents & POLLOUT) {
                can_send = true;
                break;
            }
            // Optionally, handle POLLERR or POLLHUP here if needed
            else if (fds[i].revents & (POLLERR | POLLHUP)) {
                std::cerr << "Socket error or hang-up on socket " << dest_sock << std::endl;
                return;
            }
        }
    }

    // If the socket is ready for writing, send the message
    if (can_send) {
        ssize_t bytes_sent = send(dest_sock, text.c_str(), text.length(), 0); // Removed MSG_CONFIRM
        if (bytes_sent == -1) {
            std::cerr << "Failed to send message: " <<  std::endl;
        } else {
            std::cout << "Sent " << bytes_sent << " bytes." << std::endl;
        }
    } else {
        std::cerr << "Error: socket " << dest_sock << " is not ready for writing" << std::endl;
    }
}



User::~User()
{
    if (sock != -1)
        close(sock);
}