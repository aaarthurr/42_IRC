#include "Server.cpp"

Server::Server(int port, std::string _password) : password(_password)
{

}

void	Server::start_server(void)
{

}

std::string	Server::parse_request(std::string request)
{

}

void	Server::handle_request(void)
{

}

void	Server::add_fd(int client_fd)
{

}

void	Server::remove_fd(int client)
{

}

void	Server::auth_client(int client_fd, std::string password)
{

}

void	Server::set_nickname(std::string nickname, int client)
{

}

void	Server::set_username(std::string username_str, int client)
{

}

void	Server::join_channel(std::string channel_name, int client)
{

}

void	Server::quit_channel(std::string channel_name, int client)
{
	}//if there is no more client in this channel, delete it. To be used with remove_from_list() -see channel.hpp- . Can be used to kick people too. What happen when the operator leave the channel?

void	Server::terminate_ses(int client)
{

}

void	Server::privmsg(std::string username, std::string messages)
{

}

void	Server::handle_mod(std::vector<std::string> mod_request)
{

}

int	Server::get_socket()
{

}

struct sockaddr_in	Server::get_server_adress()
{
	
}

std::string	Server::get_password()
{

}

std::map<int, User*>	Server::get_client_list()
{

}

std::map<std::string, Channel *>	Server::get_channel_list()
{

}

std::vector<struct pollfd>	Server::get_fds()
{

}

std::vector<std::string>	Server::get_unavilable_nick()
{

}

Server::~Server()
{

}