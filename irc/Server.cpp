#include "Server.hpp"

Server::Server(int _port, std::string _password) : port(_port), password(_password)
{
	
}

void	Server::start_server(void)
{
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1)
        throw std::runtime_error("Socket creation failed");

    pollfd server_pollfd;
    server_pollfd.fd = _socket;
    server_pollfd.events = POLLIN;  // Monitor the server socket for incoming connections
    server_pollfd.revents = 0;
    fds.push_back(server_pollfd);

	memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;  

    if (bind(_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        close(_socket);
        throw std::runtime_error("Binding failed");
    }

    if (listen(_socket, 5) == -1) {
        close(_socket);
        throw std::runtime_error("Listening failed");
    }

    std::cout << "Server is listening on port " << port << " with password " << password << "...\n";
}
/*
std::string	Server::parse_request(std::string request)
{

}
*/
void	Server::add_client(void)
{
    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);
    int new_client = accept(_socket, (struct sockaddr*)&client_address, &client_len);
    if (new_client == -1) {
        std::cerr << "Failed to accept client: " << strerror(errno) << std::endl;
        return;
    }
    User *user = new User(new_client, client_address);

    client_list[new_client] = user;
    pollfd  client_pollfd;
    client_pollfd.fd = new_client;
    client_pollfd.events = POLLIN;
    client_pollfd.revents = 0;
    fds.push_back(client_pollfd);
}

void	Server::remove_client(std::map<int, User *>::iterator it, int x)
{
    unavailable_nick.erase(it->second->get_nickname());
    std::vector <struct pollfd>::iterator it2 = fds.begin();
    std::advance(it2, x);
    fds.erase(it2);
    client_list.erase(it);
    std::cout << "Client requested to close the connection.\n";
}
/*
void	Server::auth_client(int client_fd, std::string password)
{
	if (password != this->password)
	{
		send_msg(client_fd, "464 :Password incorrect");
		return;
	}
	send_msg(client_fd, "Welcome !");
}*/

void	Server::send_msg(int client_fd, std::string message)
{
    if (send(client_fd, message.c_str(), message.length(), 0) == -1)
        std::cerr << "send() error" << std::endl;
    else
        std::cout << "message " << message << " sent" << std::endl;
}

int is_auth_character(const std::string& str)
{

    const char spe_character[] = {'`', '|', '^', '_', '-', '{', '}', '[', ']', '\\'};
    std::set<char> spe_character_set(spe_character, spe_character + sizeof(spe_character) / sizeof(char)); //  spe_character + sizeof(spe_character) / sizeof(char) is to initialize the pointer past the last array elements

    for (size_t x = 0; x < str.length(); x++)
    {
        if (!isalnum(str[x]) && spe_character_set.find(str[x]) == spe_character_set.end())
            return 0;
    }

    return 1;
}

void	Server::set_nickname(std::string nickname_str, int client_fd)
{
	//need to check if the nickname is already taken
    //need to check if the nickname has the correct length (9) and doesn't have forbidden characters
    //A name can only contain the following characters:

    //A through to Z. (Lowercase and uppercase.)
    //0 through to 9.
    /* `|^_-{}[] and \ */

    //And a name cannot start with a number or hyphen.
    //get the old username and erase it from the list of unavailable_nick list
    //assign the username
    std::vector<char *> buffer = parse_request((char *)nickname_str.c_str(), " ", 2);
    buffer.erase(buffer.begin());
    std::cout << "BUFFER: " << buffer[0] << std::endl;
    std::string str(buffer[0]);

    if (unavailable_nick.find(str) != unavailable_nick.end())
    {
        send_msg(client_list[client_fd]->get_client_fd(), "ERR_NICKNAMEINUSE");
        return ;
    }
    else if (str.length() > 9 || str.empty() )
    {
        std::string msg= "IRC 432 ";

        msg.append(str);
        msg.append(" :Erroneous nickname");
        send_msg(client_list[client_fd]->get_client_fd(), msg);
        return ;
    }
    unavailable_nick.insert(str);
    unavailable_nick.erase(client_list[client_fd]->get_nickname());
    client_list[client_fd]->set_nickname(str);
}

void	Server::set_username(std::string username_str, int client_fd)
{
    std::vector<char *> buffer = parse_request((char*)(username_str.c_str()), " :*", 5);
    buffer.erase(buffer.begin());
    client_list[client_fd]->set_username(buffer);

    std::cout << "USER has been setup with:" << std::endl << "Nickname: " 
                << client_list[client_fd]->get_nickname()
                << "Username : " << client_list[client_fd]->get_username() << std::endl
                << "Fullname : " << client_list[client_fd]->get_fullname();

}
/*
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
*/



int	Server::get_socket() const
{
	return (_socket);

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

std::set<std::string>	Server::get_unavilable_nick() const
{
	return (unavailable_nick);
}

Server::~Server()
{

}