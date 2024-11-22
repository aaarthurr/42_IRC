#include "Server.hpp"

Server::Server(int port, std::string _password) : password(_password)
{
	socket = socket(AF_INET, SOCK_STREAM, 0);
    if (socket == -1)
        throw std::runtime_error("Socket creation failed");

    pollfd server_pollfd;
    server_pollfd.fd = socket;
    server_pollfd.events = POLLIN;  // Monitor the server socket for incoming connections
    fds.push_back(server_pollfd);

	memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;  

    if (bind(socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        close(socket);
        throw std::runtime_error("Binding failed");
    }

    if (listen(socket, 5) == -1) {
        close(socket);
        throw std::runtime_error("Listening failed");
    }

    std::cout << "Server is listening on port " << port << "...\n";
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
	if (password != this->password)
	{
		send_msg(client_fd, "464 :Password incorrect");
		return;
	}
	send_msg(client_fd, "Welcome !");
}

void	send_msg(int client_fd, std::string message)
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

int	Server::get_socket() const
{
	return (socket);

}

struct sockaddr_in	Server::get_server_adress() const
{
	return (server_address);
	
}

std::string	Server::get_password() const
{
	return (password);

}

std::map<int, User*>	Server::get_client_list() const
{
	return (client_list);

}

std::map<std::string, Channel *>	Server::get_channel_list() const
{
	return (channel_list);

}

std::vector<struct pollfd>	Server::get_fds() const
{
	return (fds);

}

std::vector<std::string>	Server::get_unavilable_nick() const
{
	return (unavailable_nick);
}

Server::~Server()
{

}