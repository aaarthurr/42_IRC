#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <map> 
#include <vector>
#include <poll.h>
#include <unistd.h>
#include <set>
#include <sstream>
#include <exception>
#include <cerrno>
#include "User.hpp"

class Channel;

class Server
{
	private:
		int									_socket;
		int									port;
   		struct sockaddr_in 					server_address;
		std::string							password;
		std::map<int, User*>				client_list; 
		std::map<std::string, Channel *>	channel_list;
		std::vector<struct pollfd>			fds;
		std::set<std::string>				unavailable_nick;
		
	public:
		Server(int port, std::string _password);
		void								start_server(void);//DONE
		std::vector<char *>					parse_request(char *buffer, const char *delim, int words);//DONE
		void								handle_request(void);//DONE
		void								add_client(void);//DONE
		void								remove_client(std::map<int, User *>::iterator it, int x);//DONE
		void								auth_client(int client_fd, std::string _password);
		void								set_nickname(std::string nickname_str, int client);//DONE
		void								set_username(std::string username_str, int client);//DONE
		void								join_channel(std::string channel_name, int client);
		void								quit_channel(std::string channel_name, int client);//if there is no more client in this channel, delete it. To be used with remove_from_list() -see channel.hpp- . Can be used to kick people too. What happen when the operator leave the channel?
		void								privmsg(int client_fd, std::string demand);
		void								handle_mod(std::vector<std::string> mod_request);
		void								send_msg(int client_fd, std::string message);//DONE
		int									get_socket() const;//DONE
		struct sockaddr_in					get_server_adress() const;//DONE
		std::string							get_password() const;//DONE
		std::map<int, User*>				get_client_list() const;//DONE
		std::map<std::string, Channel *>	get_channel_list() const;//DONE
		std::vector<struct pollfd>			get_fds() const;//DONE
		std::set<std::string>				get_unavilable_nick() const;//DONE
		~Server();
};