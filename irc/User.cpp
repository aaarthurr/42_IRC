#include "User.hpp"

User::User(int client_fd, struct sockaddr_in client_adress)
{
	this->client_fd = client_fd;
	this->client_adress = client_adress;
}

std::string	User::get_nickname() const
{
	return (nickname);
}

std::string	User::get_username() const
{
	return (username);
}

std::string	User::get_fullname() const
{
	return (fullname);
}

int	User::get_client_fd() const
{
	return (client_fd);
}

void	User::set_username(std::vector<char *> _username)
{
	this->username = _username[0];
	this->fullname = _username[2];
	this->fullname.push_back(' ');
	this->fullname.append(_username[3]);
}

void	User::set_nickname(std::string _nickname)
{
	this->nickname = _nickname;
}
