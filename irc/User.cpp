/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leoherna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 11:26:05 by leoherna          #+#    #+#             */
/*   Updated: 2024/11/28 14:02:30 by leoherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

/*-------Construction-------*/

User::User(int client_fd, struct sockaddr_in client_adress)
{
	this->client_fd = client_fd;
	this->client_adress = client_adress;
	this->authentification = false;
	this->_operator = false;
}
User::~User()
{
	
	//std::cout << "destructor" << std::endl;
}
/*------Getters*/

std::string	User::get_nickname() const
{
	return (nickname);
}

std::string	User::get_username() const
{
	return (username);
}

std::string	User::get_fullname() const
{
	return (fullname);
}

int	User::get_client_fd() const
{
	return (client_fd);
}

bool	User::get_auth() const
{
	return(authentification);
}

bool					User::get_operator() const
{
	return (_operator);
}

struct sockaddr_in		User::get_client_adress() const
{
	return (client_adress);
}

std::map<std::string, Channel *>	User::get_channel_list() const
{
	return (channel_joined);
}

std::string				User::get_userBuffer()
{
	return (userBuffer);
}

bool	User::is_joined(std::string channel)
{
	if (channel_joined.find(channel) != channel_joined.end())
		return (1);
	return (0);	
}

/*----Setters------*/
void	User::set_auth(bool auth)
{
	this->authentification = auth;
}

void	User::set_username(std::vector<char *> _username)
{
	this->username = _username[0];
	this->fullname = _username[2];
	this->fullname.push_back(' ');
	this->fullname.append(_username[3]);
}

void	User::set_nickname(std::string _nickname)
{
	this->nickname = _nickname;
}



/*------Others-------- */

void			User::add_channel( Channel *channel)
{
	channel_joined[channel->get_name()] = channel;
}
void	User::remove_channel(std::string channel_name)
{
	channel_joined.erase(channel_name);
}

void	User::user_buffer(char *new_buffer)
{
	if (userBuffer.find('\n') != std::string::npos)
		userBuffer.clear();
	std::string temp = new_buffer;
	userBuffer += temp;
}

/*----------------------Operator function-------*/

void	User::set_operator(Channel * channel)
{
	if (_operator == false)
	{
		_operator = true;
		for (std::map<std::string, Channel *>::iterator it = channel_joined.begin() ; it != channel_joined.end(); it++)
		{
			if (it->second->get_operator() == client_fd)
				isOp_onChannel[it->second->get_name()] = it->second;

		}
		return ;
	}
	else
		isOp_onChannel[channel->get_name()] = channel;

}

bool	User::get_op(const Channel *channel)
{
	if (channel->get_operator() == client_fd)
		return (true);
	return (false);
}
std::map<std::string, Channel *>	User::get_isOp_onChannel()
{
	return (isOp_onChannel);
}

void	User::remove_op_channel(Channel *channel)
{
	if (_operator == true && isOp_onChannel.size() == 1)
	{
		_operator = false;
		isOp_onChannel.erase(channel->get_name());
	}
	else if (_operator == true && isOp_onChannel.size() == 1)
	{
		channel->kick_everyone();
		isOp_onChannel.erase(channel->get_name()); // Need to delete the whole channel and kick everyone
	}
	
}

