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

class User;

class Channel;

class Server
{
	private:
		int									socket; 
   		struct sockaddr_in 					server_address;
		std::string							password;
		std::map<int, User*>				client_list; 
		std::map<std::string, Channel *>	channel_list;
		std::vector<struct pollfd>			fds;
		std::vector<std::string>			unavailable_nick;
		
	public:
		Server(int port, std::string _password);
		void								start_server(void);
		std::string							parse_request(std::string request); //go with handle_request
		void								handle_request(void);//go with parse request
		void								add_fd(int client_fd);
		void								remove_fd(int client);
		void								auth_client(int client_fd, std::string password);
		void								set_nickname(std::string nickname, int client);
		void								set_username(std::string username_str, int client);
		void								join_channel(std::string channel_name, int client);
		void								quit_channel(std::string channel_name, int client);//if there is no more client in this channel, delete it. To be used with remove_from_list() -see channel.hpp- . Can be used to kick people too. What happen when the operator leave the channel?
		void								terminate_ses(int client);
		void								privmsg(std::string username, std::string messages);
		void								handle_mod(std::vector<std::string> mod_request);
		int									get_socket(); const
		struct sockaddr_in					get_server_adress(); const
		std::string							get_password(); const
		std::map<int, User*>				get_client_list(); const
		std::map<std::string, Channel *>	get_channel_list(); const
		std::vector<struct pollfd>			get_fds(); const
		std::vector<std::string>			get_unavilable_nick(); const
		~Server();
};