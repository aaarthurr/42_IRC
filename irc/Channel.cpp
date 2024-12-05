#include "Channel.hpp"

Channel::Channel(std::string name_, std::string topic_, int operator_) : name(name_), topic(topic_), _operator(operator_), is_on_invite(0)
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
	std::string msg = name + " Topic changed from " + topic + " to " + _topic;
	topic =_topic;
	send_to_all(0, msg);
}

std::string		Channel::get_name() const
{
	return (name);
}

std::string		Channel::get_topic() const
{
	return (topic);
}

std::string	Channel::get_pass() const
{
	return (pass);
}

int	Channel::get_operator() const
{
	return (_operator);
}

std::map<int , User *>		Channel::get_client_list() const
{
	return (client_list);
}

void	Channel::set_user_limit(int new_limit)
{
	std::cout << (int)client_list.size() << std::endl;
	if ((int)client_list.size() > new_limit && new_limit != 0)
	{
		send_to_all(0, "Error Limit under total of users");
		return ;
	}
	else
		user_limit = new_limit;
}

int		Channel::get_user_limit()
{
	return (user_limit);
}

void	Channel::set_topic_changement(bool changement)
{
	topic_changement = changement;
}

bool	Channel::get_topic_changement()
{
	return (topic_changement);
}

void	Channel::add_to_list(const User   *client)
{
	client_list[client->get_client_fd()] = (User *)client;
	client_list[client->get_client_fd()]->add_channel(this);
}
void					Channel::remove_from_list(User *client)
{
	client->remove_channel(name);
	if (client->get_op(this) == true)
	{
		client_list[client->get_client_fd()]->remove_op_channel(this);	
	}

	client_list.erase(client->get_client_fd());
}

void					Channel::send_to_all(int client_fd, std::string message)
{
	for (std::map<int, User *>::iterator it = client_list.begin(); it != client_list.end(); it++)
	{
		if (it->first != client_fd)
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
		if (it->first == _operator)
			msg += '@';
		msg += it->second->get_nickname();
		msg += " ";
	}
    //:server-name 353 nickname = #channel-name :@user1 +user2 user3
	return (msg);
}

int	get_client_fd_by_nickname(std::string _nickname, std::map<int, User *> client_list)//-TODO faire un fonction commune a toute les fichiers
{
	for (std::map<int, User *>::iterator it = client_list.begin(); it != client_list.end(); it++)
	{
		if (it->second->get_nickname() == _nickname)
		{
			return (it->second->get_client_fd());
		}
	}
	return (-1);
}

void	Channel::set_password(int client_fd, std::string password)
{
	std::string msg = name + " Channel password has been";

	if (pass.empty())
		msg += " set to " + password; 
	else
		msg += name + " changed to " + password; 
	
	pass = password;
	send_msg(client_fd, msg);
}

void	Channel::set_operator(int client_fd, std::string nickname)
{
	int target = get_client_fd_by_nickname(nickname, client_list);
	if (target == -1)
	{
		std::string msg = name + " ERR_CANNOTFINDUSER";
		send_msg(client_fd , msg);
	}
	else
	{
		std::string msg = name + " User " + client_list[client_fd]->get_nickname() + " is the new operator of this channel";
		client_list[_operator]->remove_op_channel(this);
		_operator = target;
		client_list[client_fd]->set_operator(this);
		send_to_all(client_fd, msg);
	}
}

void	Channel::set_invite_only(bool _invite_only)
{
	is_on_invite = _invite_only;
}

Channel::~Channel()
{
	
}