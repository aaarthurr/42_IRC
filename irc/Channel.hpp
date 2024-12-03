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

class Channel
{
	private:
		std::string				name;
		std::string				topic;
		int						_operator;
		bool					is_on_invite;
		std::map<int , User *>	client_list;
	public:
		Channel(std::string name_);
		Channel(std::string name_, std::string topic_, int operator_);
		void					add_to_list(const User   *client);
		void					remove_from_list(User *client);
		void					set_topic(std::string topic);
		void					set_invite_only(bool invite_only);
		bool					is_invite_only();
		int						get_operator() const;
		std::string				get_name() const;
		std::string				get_topic() const; //if no topic, return NO TOPIC or smth
		std::string				get_client_str(std::string nickname);  //:server-name 353 nickname = #channel-name :@user1 +user2 user3
		std::map<int , User *>	get_client_list() const;
		void					send_to_all(int client_fd, std::string message);
		~Channel();

		/*-----Nucleocherry's functions-------*/

		void					kick_everyone();
		
};

void							send_msg(int client_fd, std::string message);//DONE

#include "User.hpp"
