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
		void								start_server(void);
		std::vector<char *>					parse_request(char *buffer, const char *delim, int words);
		void								handle_request(void);//go with parse request
		void								add_client(void);//DONE
		void								remove_client(std::map<int, User *>::iterator it, int x);
		void								auth_client(int client_fd, std::string password);
		void								set_nickname(std::string nickname_str, int client);//DONE
		void								set_username(std::string username_str, int client);//DONE
		void								join_channel(std::string channel_name, int client);
		void								quit_channel(std::string channel_name, int client);//if there is no more client in this channel, delete it. To be used with remove_from_list() -see channel.hpp- . Can be used to kick people too. What happen when the operator leave the channel?
		void								terminate_ses(int client_fd);
		void								privmsg(std::string username, std::string messages);
		void								handle_mod(std::vector<std::string> mod_request);
		void								send_msg(int client_fd, std::string message);
		int									get_socket() const;
		struct sockaddr_in					get_server_adress() const;
		std::string							get_password() const;
		std::map<int, User*>				get_client_list() const;
		std::map<std::string, Channel *>	get_channel_list() const;
		std::vector<struct pollfd>			get_fds() const;
		std::set<std::string>				get_unavilable_nick() const;
		~Server();
};