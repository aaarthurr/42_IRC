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
		bool					topic_changement;
		std::string				pass;
		int						_operator;
		bool					is_on_invite;
		std::map<int , User *>	client_list;
		int						user_limit;

	public:
		Channel(std::string name_);
		Channel(std::string name_, std::string topic_, int operator_);
		void					add_to_list(const User   *client);
		void					remove_from_list(User *client);
		void					set_topic(std::string topic);
		void					set_topic_changement(bool changement);
		void					set_operator(int client_fd, std::string nickname);
		void					set_invite_only(bool _invite_only);
		void					set_password(int client_fd, std::string password);
		void					set_user_limit(int new_limit);
		bool					is_invite_only();
		int					get_user_limit();
		int						get_operator() const;
		std::string				get_pass() const;
		std::string				get_name() const;
		std::string				get_topic() const; //if no topic, return NO TOPIC or smth
		bool					get_topic_changement();
		
		std::string				get_client_str(std::string nickname);  //:server-name 353 nickname = #channel-name :@user1 +user2 user3
		std::map<int , User *>	get_client_list() const;
		void					send_to_all(int client_fd, std::string message);
		~Channel();

		/*-----Nucleocherry's functions-------*/

		void					kick_everyone();
		
};

void							send_msg(int client_fd, std::string message);//DONE

#include "User.hpp"
