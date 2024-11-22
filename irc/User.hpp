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

class Channel;

class User{
	private:
		std::string							nickname;
		std::string							username;
		std::string							fullname;
		bool								authentification;
		bool								_operator;
		int									client_fd;
		struct sockaddr_in 					client_adress;
		std::map<std::string, Channel *>	channel_joined;
	public:
		std::string				get_nickname() const;
		std::string				get_username() const;
		std::string				get_fullname() const;
		void					set_nickname();
		void					set_username();
		void					set_fullname();
		bool					get_auth() const;
		bool					get_operator() const;
		int						get_client_fd() const;
		struct sockaddr_in		get_client_adress() const;
		std::vector<Channel *>	get_channel_list() const;
		void					remove_channel(std::string channel_name);
		void					add_channel(const Channel *channel);
		bool					is_joined(std::string channel);
};