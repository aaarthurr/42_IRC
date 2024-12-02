#include "Channel.hpp"

Channel::Channel(std::string name_, std::string topic_, int operator_) : name(name_), topic(topic_), _operator(operator_)
{

}

Channel::Channel(std::string name_) : name(name_)
{

}

bool	Channel::is_invite_only()
{
	return (is_on_invite);
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
int	Channel::get_operator() const
{
	return (_operator);
}
std::map<int , User *>		Channel::get_client_list() const
{
	return (client_list);
}

void	Channel::add_to_list(const User   *client)
{
	client_list[client->get_client_fd()] = (User *)client;
	client_list[client->get_client_fd()]->add_channel(this);
}
void					Channel::remove_from_list(User *client)
{
	client_list.erase(client->get_client_fd());
	client_list[client->get_client_fd()]->remove_channel(name);

	if (client->get_op(this) == true)
	{
		client_list[client->get_client_fd()]->remove_op_channel(this);	
	}
}

void					Channel::send_to_all(std::string message)
{
	for (std::map<int, User *>::iterator it = client_list.begin(); it != client_list.end(); it++)
	{
		send_msg(it->first, message);
	}
}

void				Channel::kick_everyone()
{
	for (std::map<int, User *>::iterator it = client_list.begin(); it != client_list.end(); it++)
		it->second->remove_channel(name);
}

std::string	Channel::get_client_str(std::string nickname)
{
	std::string msg = "IRC " + nickname + " = " + name + " :";
	for (std::map<int, User *>::iterator it = client_list.begin(); it != client_list.end(); it++)
	{
		msg += it->second->get_nickname();
		msg += " ";
	}
    //:server-name 353 nickname = #channel-name :@user1 +user2 user3
	return (msg);
}

Channel::~Channel()
{
	
}