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
		Channel();
		void					add_to_list(const User   *client, int i);
		void					remove_from_list(int client);
		void					set_topic(std::string topic);
		void					set_invite_only(bool invite_only);
		bool					is_invite_only();
		int						get_operator(); const
		std::string				get_name(); const
		std::string				get_topic(); const
		std::map<int , User *>	get_client_list(); const
		void					send_to_all(std::string message);
		~Channel();
};