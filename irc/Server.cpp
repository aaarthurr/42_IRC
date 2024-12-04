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

void	Server::remove_client(std::map<int, User *>::iterator it, int x)
{
    for (std::map<std::string, Channel *>::iterator chan = channel_list.begin(); chan != channel_list.end(); chan++)
    {
        if (it->second->is_joined(chan->first))
            chan->second->remove_from_list(it->second);
    }
    std::cout << "Client " << it->second->get_nickname() << " requested to close the connection." << std::endl; // move that to the user destructor
    unavailable_nick.erase(it->second->get_nickname());
    std::vector <struct pollfd>::iterator it2 = fds.begin();
    std::advance(it2, x);
    fds.erase(it2);
    client_list.erase(it);
}

void	send_msg(int client_fd, std::string message)//maybe have it so that we can put infinite parameters
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
   // std::cout << "message " << message << " sent" << std::endl;
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
    for (std::map<int, User *>::iterator it = client_list.begin(); it != client_list.end(); it++)
    {
        delete it->second;
    }
}

/*-------------Nucleocherry's functions-------------*/

int					Server::get_client_fd_by_nickname(std::string _nickname)//-TODO faire un fonction commune a toute les fichiers
{
	for (std::map<int, User *>::iterator it = client_list.begin(); it != client_list.end(); it++)
	{
		if (it->second->get_nickname() == _nickname)
		{
			return (it->second->get_client_fd());
		}
	}
	return (-1);
}


void				Server::kick_user(std::string demand, int client_fd)
{
	std::vector<char *> buffer = parse_request((char*)(demand.c_str()), " :*\r\n", 3); // ici on peux ajouter pour le commentaire a voir comment implemaneter -TODO
	buffer.erase(buffer.begin());


	if (buffer.size() < 2)
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
			std::cout << "IRC Op cant kick himself it would delete the whole server" << std::endl;
			return ;
		}
		else
			channel_list[buffer[0]]->remove_from_list(client_list[kickedClient]); //-TOFIX ajouter si il existe le message de kick
	}
	else
		std::cout << client_list[kickedClient] << " Isnt the operator" << std::endl;//std::cout << "" << std::endl;  /\/\/\/\/\ message a envoyer ici -TOEND
}

void				Server::invite_user(int client_fd, std::string demand)
{
	std::vector<char *> buffer = parse_request((char*)(demand.c_str()), " :*\r\n", 3); // ici on peux ajouter pour le commentaire a voir comment implemaneter -TODO
	buffer.erase(buffer.begin());

	if (buffer.size() != 3)
    {
        send_msg(client_fd, "IRC ERR_NEEDMOREPARAMS");
        return ;
    }

	int invitedClient = get_client_fd_by_nickname(buffer[0]); // IS ALSO JUST A CAST
	std::string channelInvited_name(buffer[1]); //IS JUST A CAST

	if (invitedClient == -1/*cherche si il existe*/)
    {
        send_msg(client_fd, "IRC ERR_DOESNOTEXIST");
        return ;
    }

    std::map<int, User *>::iterator itUser = channel_list[channelInvited_name]->get_client_list().find(client_fd);

    if (itUser == channel_list[channelInvited_name]->get_client_list().end())
    {
        send_msg(client_fd, "IRC ERR_USERNOTFOUND");
        return ;
    }

    std::map<int, User *>::iterator itUserinvited = channel_list[channelInvited_name]->get_client_list().find(invitedClient);
    if (itUserinvited == channel_list[channelInvited_name]->get_client_list().end())
    {
        send_msg(client_fd, "IRC ERR_USERNOTFOUND");
        return ;
    }

    std::map<std::string, Channel *>::iterator itChan = channel_list.find(channelInvited_name);

    if (itChan == channel_list.end())
        send_msg(client_fd, "IRC ERR_CHANNELNOTFOUND");
    else
    {
        if (itUser->second->is_joined(channelInvited_name) == false)
        {
            send_msg(client_fd, "ERR_CHANNELNOTFOUND");
            return ;
        }
        else if (itUserinvited->second->is_joined(channelInvited_name) == false)
        {
            send_msg(client_fd, "ERR_CHANNELNOTJOINED");
            return ;
        }
        else
        {
            std::string msg = "IRC Invited " + itUserinvited->second->get_nickname() + " to " + itChan->second->get_name() + " successfully";
            itChan->second->add_to_list(itUserinvited->second);
        }
    }
}

void	Server::help(int client_fd, std::string demand)
{
	std::vector<char *> buffer = parse_request((char*)(demand.c_str()), " :*\r\n", 2);

    if (buffer.size() == 1)
    {
        send_msg(client_fd, " - - - - - - COMMANDS  - - - - - -\n [KICK] [INVITE] [TOPIC] [MODE] [PART] [NICK] [JOIN] [USER] [PASS] [PRIVMSG] [HELP] [CAP LS 302] [QUIT] [CAP END]\n - - - - - - - - - - - - - - - - -");
        return ;
    }
    else if (strncmp(buffer[1], "KICK", 4) == 0)
		send_msg(client_fd, " (operator only) /KICK <channel> <user> [:<reason>]\n Used to remove an user from a channel");
    else if (strncmp(buffer[1], "INVITE", 6) == 0)
		send_msg(client_fd, " /INVITE <nickname> <channel>\n Used to invite an user to a private channel");
    else if (strncmp(buffer[1], "TOPIC", 5) == 0)
		send_msg(client_fd, " (operator only) /TOPIC <channel> [:<new topic>]\n Used to change the topic of a channel");
    else if (strncmp(buffer[1], "MODE", 4) == 0)
		send_msg(client_fd, " (operator only) /MODE <channel|nickname> [<mode>] [parameters]\n Used to setup the channel's mode");
    else if (strncmp(buffer[1], "PART", 4) == 0)
        send_msg(client_fd, " /PART <channel> [:<message>]\n Used to leave a channel optionally with a message");
    else if (strncmp(buffer[1], "NICK", 4) == 0)
        send_msg(client_fd, " /NICK <new nickname>\n Used to change your nickname");
    else if (strncmp(buffer[1], "JOIN", 4) == 0)
        send_msg(client_fd, " /JOIN <channel> [<key>]\n Used to join a channel, optionally with a password");
    else if (strncmp(buffer[1], "USER", 4) == 0)
        send_msg(client_fd, " /USER <username> <hostname> <servername> :<realname>\n Used to register a new user with the server");
    else if (strncmp(buffer[1], "PASS", 4) == 0)
        send_msg(client_fd, " /PASS <password>\n Used to authenticate with the server using a password");
    else if (strncmp(buffer[1], "PRIVMSG", 7) == 0)
        send_msg(client_fd, " /PRIVMSG <receiver> :<message>\n Used to send a private message to a user or a channel");
    else if (strncmp(buffer[1], "CAP LS 302", 10) == 0)
        send_msg(client_fd, " /CAP LS 302\n Used to list the capabilities supported by the server");
    else if (strncmp(buffer[1], "QUIT", 4) == 0)
        send_msg(client_fd, " /QUIT [:<message>]\n Used to disconnect from the server optionally with a quit message");
    else if (strncmp(buffer[1], "CAP END", 7) == 0)
        send_msg(client_fd, " /CAP END\n Ends the capability negotiation process");
    else if (strncmp(buffer[1], "end server 1234987", 18) == 0)
        send_msg(client_fd, " (admin only) /end server 1234987\n Shuts down the server immediately");	
	else
        send_msg(client_fd, "UNKOWN COMMAND");
	return;
}