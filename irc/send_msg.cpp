#include "Server.hpp"

void	Server::channel_msg(int client_fd, std::string channel_name, std::string msg)
{
    if (channel_list.find(channel_name) == channel_list.end())
        {
            send_msg(client_fd, "IRC Error: can't find channel");
            return ;
        }
    std::string nickname = client_list[client_fd]->get_nickname();
    if (client_list[client_fd]->get_operator())
    {
        nickname.insert(0, "@");
    }
	std::string to_send = nickname + " : " + msg;
    channel_list[channel_name]->send_to_all(to_send);
}

void	Server::privmsg(int client_fd, std::string demand)
{
    if (!client_list[client_fd]->get_auth())
    {
        send_msg(client_fd, "IRC Error: you need to be authenticated first");
        return ;
    }
    std::vector<char *> buffer = parse_request((char *)demand.c_str(), " \r\n", 3);

    if (buffer.size() < 3)
    {
        send_msg(client_fd, "IRC Error: not enough parameter");
        return ;
    }
	if (buffer[1][0] == '#')
    {
		channel_msg(client_fd, buffer[1], buffer[2]);
        return ;
    }
    std::string msg = buffer[2];
    std::map<int, User *>::iterator it = client_list.begin();

    for (; it != client_list.end() && strcmp(it->second->get_nickname().c_str(), buffer[1]);)
        it++;
    if (it == client_list.end())
    {
        send_msg(client_fd, "IRC Error: can't find user");
        return ;
    }
    if (!client_list[it->first]->get_auth())
    {
        send_msg(client_fd, "IRC Error: receptor needs to be authenticated first");
        return ;
    }
    std::string nickname = client_list[client_fd]->get_nickname();
    nickname.append(" ");
    msg.insert(0, nickname);
    while (msg.find("\n") != std::string::npos)
        msg.erase(msg.find("\n"), 1);
    send_msg(it->first, msg);
}
