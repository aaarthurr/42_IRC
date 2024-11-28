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
int hashCommand(const char* command) {
    if (strncmp(command, "KICK", 4) == 0) return 1;
    if (strncmp(command, "INVITE", 6) == 0) return 2;
    if (strncmp(command, "TOPIC", 5) == 0) return 3;
    if (strncmp(command, "MODE", 4) == 0) return 4;
	if (strncmp(command, "NICK", 4) == 0) return 5;
	if (strncmp(command, "USER", 4) == 0) return 6;
	if (strncmp(command, "PASS", 4) == 0) return 7;
	if (strncmp(command, "PRIVMSG", 7) == 0) return 8;
	if (strncmp(command, "CAP LS 302", 10) == 0) return 9;
	if (strncmp(command, "QUIT", 4) == 0) return 10;
    return 11;
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
					ssize_t bytes_received = recv(it->first, buffer, 1024, 0);
					it->second->user_buffer(buffer);
					if (it->second->get_userBuffer().find('\n') == std::string::npos)
						continue;
					else if (it->second->get_userBuffer().size())
					{
						size_t totalSize = it->second->get_userBuffer().size() + std::strlen(buffer);
						if (totalSize >= 1024)
						{
							it->second->get_userBuffer().clear();
							continue;
						}
						std::strncpy(buffer, it->second->get_userBuffer().c_str(), it->second->get_userBuffer().size());
						std::strncpy(buffer + it->second->get_userBuffer().size(), buffer + it->second->get_userBuffer().size(), 1024 - it->second->get_userBuffer().size());
						it->second->get_userBuffer().clear();
					}
					if (bytes_received == -1)
					{
						std::cerr << "recv() error" << std::endl;
						return ; //throw error
					}
					int command = hashCommand(buffer);
					std::cout << "client :" << buffer << std::endl;
					switch (command) {
						case 1:
							std::cout << "KICK USER " << it->first << "\n";
							break ;
						case 2:
							std::cout << "INVITE USER " << it->first << "\n";
							break ;
						case 3:
							std::cout << "SET TOPIC " << it->first << "\n";
							break ;
						case 4:
							std::cout << "MODE SETUP " << it->first << "\n";
							break ;
						case 5:
							set_nickname(buffer, it->first);
							break ;
						case 6:
							set_username(buffer, it->first);
							break ;
						case 7:
							auth_client(it->first, buffer);
							break ;
						case 8:
							std::cout << "PRIVMSG" << it->first << "\n";
							break ;
						case 9:
							send_msg(it->first, "CAP * LS :");
							break ;
						case 10:
							remove_client(it, x);
							break ;
						case 11:
						{
							if (strncmp("CAP END", buffer, 7))
							{
								std::cout << "IRC Invalid Command: " << buffer << std::endl;
								send_msg(it->first, "IRC Invalid Command ");
							}
							break ;
						}
					}
				}
			}
		}
	}
}
