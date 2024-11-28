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
#include <algorithm>
#include "Channel.hpp"
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
		void								kick_user(std::string command, int client_fd);
		void								invite_user(std::string command, int client_fd);
		void								set_topic(std::string command, int client_fd);
		void								remove_client(std::map<int, User *>::iterator it, int x);//DONE
		void								auth_client(int client_fd, std::string _password);
		void								set_nickname(std::string nickname_str, int client_fd);//DONE
		void								set_username(std::string username_str, int client_fd);//DONE
		void								join_channel(std::string command, int client_fd);//NEED TO BE TESTED
		void								quit_channel(std::string channel_name, int client_fd);//NEED TO BE TESTED
		void								privmsg(int client_fd, std::string demand); //DONE
		void								channel_msg(int client_fd, std::string channel_name, std::string msg);
		void								handle_mod(std::vector<std::string> mod_request);
		void								send_msg(int client_fd, std::string message);//DONE
		int									get_socket() const;//DONE
		void								hashCommand(char* _buffer, std::map<int, User *>::iterator it, int x);//DONE
		struct sockaddr_in					get_server_adress() const;//DONE
		std::string							get_password() const;//DONE
		std::map<int, User*>				get_client_list() const;//DONE
		std::map<std::string, Channel *>	get_channel_list() const;//DONE
		std::vector<struct pollfd>			get_fds() const;//DONE
		std::set<std::string>				get_unavilable_nick() const;//DONE
		~Server();
};