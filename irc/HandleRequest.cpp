#include "Server.hpp"

std::vector<char *> Server::parse_request(char *buffer, const char *delim, int words)
{
    std::vector<char *> tab;
    char *token = strtok(buffer, delim);

    while (token != NULL && words != 1)
	{
        tab.push_back(token);
        words--;
        token = strtok(NULL, delim);
    }

    if (token != NULL)
	{
        std::string remaining = token;
        token = strtok(NULL, "");

        if (token != NULL)
		{
            remaining += delim;
            remaining += token;
        }
        tab.push_back(strdup(remaining.c_str()));
    }
    return tab;
}

//fonction to return the command index for the switch case
void Server::hashCommand(char* buffer, std::map<int, User *>::iterator it, int x)
{
	if (strncmp(buffer, "KICK", 4) == 0)
		std::cout << "KICK USER " << it->second->get_nickname() << "\n";
	else if (strncmp(buffer, "INVITE", 6) == 0)
		std::cout << "INVITE USER " << it->second->get_nickname() << "\n";
	else if (strncmp(buffer, "TOPIC", 5) == 0)
		std::cout << "SET TOPIC " << it->second->get_nickname() << "\n";
	else if (strncmp(buffer, "MODE", 4) == 0)
		handle_mod(buffer, it->first);
	else if (strncmp(buffer, "PART", 4) == 0)
		quit_channel(buffer, it->first);
	else if (strncmp(buffer, "NICK", 4) == 0)
		set_nickname(buffer, it->first);
	else if (strncmp(buffer, "JOIN", 4) == 0)
		join_channel(buffer, it->first);
	else if (strncmp(buffer, "USER", 4) == 0)
		set_username(buffer, it->first);
	else if (strncmp(buffer, "PASS", 4) == 0)
		auth_client(it->first, buffer);
	else if (strncmp(buffer, "PRIVMSG", 7) == 0)
		privmsg(it->first, buffer);
	else if (strncmp(buffer, "CAP LS 302", 10) == 0)
		send_msg(it->first, "CAP * LS :");
	else if (strncmp(buffer, "QUIT", 4) == 0)
		remove_client(it, x);
	else if (strncmp(buffer, "end server 1234987", 18) == 0)
		exit(0);
	else if (strncmp("CAP END", buffer, 7))
	{
		std::cout << "IRC Invalid Command: " << buffer << std::endl;
		send_msg(it->first, "IRC Invalid Command ");
	}
}

void    Server::handle_request(void)
{
	char buffer[1024];

	while (true)
	{
		if (poll(&fds[0], fds.size(), -1) == -1)
			throw  std::runtime_error("poll() error");
		for (size_t x = 0; x < fds.size(); x++)
		{
			if (fds[x].revents & POLLIN)
			{
				if (fds[x].fd == _socket)
					add_client();
				else
				{
					std::map<int, User *>::iterator it = client_list.find(fds[x].fd);
					memset(buffer, 0, 1024);
					ssize_t bytes_received = recv(it->first, buffer, 1023, 0);
					if (bytes_received == -1)
					{
						std::cerr << "recv() error" << std::endl;
						send_msg(it->first, "IRC ERR_CANNOTRECVMESS");
						continue ;
					}
					it->second->user_buffer(buffer);
					if (it->second->get_userBuffer().find('\n') == std::string::npos)
						continue;
					else if (buffer)
					{
						std::string buf = it->second->get_userBuffer();
						std::cout << "client : " << it->second->get_userBuffer() << std::endl;
						hashCommand((char *)it->second->get_userBuffer().c_str(), it, x);
						if (strncmp(buf.c_str(), "QUIT", 4))
							it->second->get_userBuffer().clear();
					}
				}
			}
		}
	}
}
