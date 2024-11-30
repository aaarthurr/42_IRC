#include "Channel.hpp"

Channel::Channel(std::string name_, std::string topic_, int operator_)
{

}

void Channel::set_topic(std::string _topic)
{
	topic =_topic;
}

std::string		Channel::get_name() const
{
	return (name);
}
std::string		Channel::get_topic() const
{
	return (topic);
}
std::string		Channel::get_operator() const
{
	return (_operator);
}
std::map<int , User *>		Channel::get_client_list() const
{
	return (client_list);
}

void					Channel::add_to_list(const User   *client, int i)
{
	client_list[client->get_name()] = client;
	client.add_channel(this);
	(void)i;
}
void					Channel::remove_from_list(User *client)
{
	client_list.erase(client->get_client_fd());
	client->remove_channel(channel_name);

	if (client->get_op() == true)
	{
		client->remove_op_channel(channel_name);	
	}
}


void				Channel::kick_everyone()
{
	for (std::map<int, User>::iterator it = client_list.begin(); it != client_list.end(); it++)
		it->second.remove_channel();
}


/*---	
		void					set_topic(std::string topic);
		void					set_invite_only(bool invite_only);
		bool					is_invite_only();
		void					send_to_all(std::string message);
---*/
		

