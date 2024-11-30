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

    if (listen(_socket, 5) == -1) { // need to be revised (number 5)
        close(_socket);
        throw std::runtime_error("Listening failed");
    }

    std::cout << "Server is listening on port " << port << " with password " << password << "...\n";
}

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
    std::cout << "Client " << it->second->get_nickname() << " requested to close the connection." << std::endl; // move that to the user destructor
    unavailable_nick.erase(it->second->get_nickname());
    std::vector <struct pollfd>::iterator it2 = fds.begin();
    std::advance(it2, x);
    fds.erase(it2);
    client_list.erase(it);
    delete it->second;
}

void	Server::auth_client(int client_fd, std::string _password)
{
    std::vector<char *> buffer = parse_request((char *)_password.c_str(), " \r\n", 2);

    buffer.erase(buffer.begin());
    _password.erase(_password.begin());
    _password = buffer[0];
    std::cout << "PASSWORD: " << _password << std::endl;
    if (_password.empty())
    {
        send_msg(client_fd, "IRC ERR_NEEDMOREPARAMS");
		return;
    }
	else if (strcmp(_password.c_str(), password.c_str()))
	{
		send_msg(client_fd, "IRC ERR_BADPASSWORD");
		return;
	}
	send_msg(client_fd, "IRC Welcome !");
    client_list[client_fd]->set_auth(true);
}

void	Server::send_msg(int client_fd, std::string message)//maybe have it so that we can put infinite parameters
{
    int temp = 0;

    message.append("\r\n");
    for (size_t bytes_sent = 0; bytes_sent < message.length();)
    {
        if ((temp = send(client_fd, message.c_str(), message.length(), 0) ) == -1)
        {
            std::cerr << "send() error" << std::endl;
            return ;
        }
        else
            bytes_sent += temp;
    }
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
    std::vector<char *> buffer = parse_request((char *)nickname_str.c_str(), " \r\n", 2);
    buffer.erase(buffer.begin());
    std::cout << "BUFFER: " << buffer[0] << std::endl;
    std::string str(buffer[0]);

    if (unavailable_nick.find(str) != unavailable_nick.end())
    {
        send_msg(client_list[client_fd]->get_client_fd(), "IRC ERR_NICKNAMEINUSE");
        return ;
    }
    else if (str.length() > 9 || str.empty())
    {
        std::string msg= "IRC 432 ";

        msg.append(str);
        msg.append(" :Erroneous nickname");
        send_msg(client_list[client_fd]->get_client_fd(), msg);
        return ;
    }
    unavailable_nick.insert(str);
    unavailable_nick.erase(client_list[client_fd]->get_nickname());
    if (!client_list[client_fd]->get_nickname().empty())
    {
        std::string msg = "IRC Nickname successfully changed from ";
        msg.append(client_list[client_fd]->get_nickname());
        msg.append(" to ");
        msg.append(str);
        send_msg(client_fd, msg);
    }
    client_list[client_fd]->set_nickname(str);
}

void	Server::set_username(std::string username_str, int client_fd)
{
    std::vector<char *> buffer = parse_request((char*)(username_str.c_str()), " :*\r\n", 5);
    
    buffer.erase(buffer.begin());
    if (username_str.empty())
    {
        send_msg(client_list[client_fd]->get_client_fd(), "IRC ERR_NEEDMOREPARAMS");
        return ;
    }
    else if (!client_list[client_fd]->get_username().empty())
    {
        send_msg(client_list[client_fd]->get_client_fd(), "IRC ERR_ALREADYREGISTRED");
        return ;
    }
    client_list[client_fd]->set_username(buffer);
    std::cout << "USER has been setup with:" << std::endl << "Nickname: " 
                << client_list[client_fd]->get_nickname() << std::endl
                << "Username : " << client_list[client_fd]->get_username() << std::endl
                << "Fullname : " << client_list[client_fd]->get_fullname() << std::endl;
}
/*
void	Server::join_channel(std::string channel_name, int client)
{

}

void	Server::quit_channel(std::string channel_name, int client)
{
	}//if there is no more client in this channel, delete it. To be used with remove_from_list() -see channel.hpp- . Can be used to kick people too. What happen when the operator leave the channel?

*/
void	Server::privmsg(int client_fd, std::string demand)
{
	std::cout << "PRIVMSG to " << client_fd << " with the message " << demand << std::endl;
}
/*
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



/*-------------Nucleocherry's functions-------------*/

int					Server::get_client_fd_by_nickname(std::string _nickname)
{
	for (std::map<int, User>::iterator it; it != client_list.end(); it++)
	{
		if (it->second.get_nickname == _nickname)
		{
			return (it->second.get_client_fd());
		}
	}
	return (-1);
}

int					Server::get_client_fd_by_nickname(char *tempNickname)
{
	std::string _nickname(tempNickname);
	for (std::map<int, User>::iterator it; it != client_list.end(); it++)
	{
		if (it->second.get_nickname == tempNickname)
		{
			return (it->second.get_client_fd());
		}
		
	}
	return (-1);
}

void				Server::kick_user(int client_fd, std::string demand)
{
	std::vector<char *> buffer = parse_request((char*)(demand.c_str()), " :*\r\n", 3); // ici on peux ajouter pour le commentaire a voir comment implemaneter -TODO
	buffer.erase(buffer.begin());


	if (username_str.empty())
    {
        send_msg(client_list[client_fd]->get_client_fd(), "IRC ERR_NEEDMOREPARAMS");
        return ;
    }


	int kickedClient = get_client_fd_by_nickname(buffer[1]);
	if (kickedClient == -1/*cherche si il existe*/)
    {
        send_msg(client_list[client_fd]->get_client_fd(), "IRC ERR_DOESNOTEXIST");
        return ;
    }

	if (channel_list[buffer[0]]->get_operator() == client_list[client_fd]->get_client_fd())
	{
		if (client_list[client_fd]->get_client_fd() == kickedClient)
		{
			std::cout << "Op cant kick himself it would delete the whole server" std::endl;
			return ;
		}
		else
			channel_list[buffer[0]]->remove_from_list(kickedClient);
	}
	else
		std::cout << kickedCLient << " Isnt the operator" << std::endl;//std::cout << "" << std::endl;  /\/\/\/\/\ message a envoyer ici -TOEND
}

void				Server::invite_user(int client_fd, std::string demand)
{
	std::vector<char *> buffer = parse_request((char*)(demand.c_str()), " :*\r\n", 3); // ici on peux ajouter pour le commentaire a voir comment implemaneter -TODO
	buffer.erase(buffer.begin());

	if (username_str.empty())
    {
        send_msg(client_list[client_fd]->get_client_fd(), "IRC ERR_NEEDMOREPARAMS");
        return ;
    }

	int invitedClient = get_client_fd_by_nickname(buffer[0]); // IS ALSO JUST A CAST
	std::string channelInvited_name(buffer[1]); //IS JUST A CAST

	if (InvitedClient == -1/*cherche si il existe*/)
    {
        send_msg(client_list[client_fd]->get_client_fd(), "IRC ERR_DOESNOTEXIST");
        return ;
    }

	for (std::map<int, User>::iterator itUser = channel_list[channelInvited_name].client_list.begin(); itUser != channel_list[channelInvited_name].client_list.end(); itUser++)
	{
		if (itUser->second.get_client_fd() == invitedClient)
			break;
	}

	for (std::map<int, User>::iterator itUserinvited = channel_list[channelInvited_name].client_list.begin(); itUserinvited != channel_list[channelInvited_name].client_list.end(); itUserinvited++)
	{
		if (itUserinvited->second.get_client_fd() == invitedClient)
			break;
	}

	for (std::map<std::string, Channel>::iterator itChan; itChan != channel_list.end(); itChan++)
	{
		if (itChan->second.get_name() == channelInvited_name)
		{
			if (itUser->second.is_joined(channelInvited_name) == false)
			{
				std::cout <<  "No right to invite people in channel " << buffer[1] << std::endl;
				return ;
			}
			else if (itUserinvited->second.is_joined(channelInvited_name) == false)
			{
				std::cout << buffer[0] << " is already in the channel " << buffer[1] << std::endl;
				return ;
			}
			else
			{
				itChan->second.add_to_list(itUserinvited->second, 0);
				std::cout << "invited succesfully !" < std::endl;
			}
			return ;
		}
	}
	std::cout << "Channel not found." << std::endl;
}

void	Server::change_topic(int client_fd, std::string demand)
{
	std::vector<char *> buffer = parse_request((char*)(demand.c_str()), " :*\r\n", 3); // ici on peux ajouter pour le commentaire a voir comment implemaneter -TODO
	buffer.erase(buffer.begin());

	std::string chan_name(buffer[1]);

	for (std::map<std::string, Channel>::iterator itChan; itChan != channel_list.end(); itChan++)
	{
		if (itChan->second.get_name() == chan_name)
		{
			if (client_fd == itChan->second.get_operator())
			{
				set_topic(buffer[0]);
			}
			else
			{
				std::cout << "Doesnt have the right to change the topic" << std::endl; // -TODO
			}
			return ;
		}
		std::cout << "this channel doest exist try another" << std::endl; // -TODO
	}
}