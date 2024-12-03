#include "Server.hpp"

void	Server::join_channel(std::string command, int client_fd)
{
    std::vector<char *> buffer = parse_request((char *)command.c_str(), " \r\n", 2);

    if (buffer.size() < 2)
    {
        send_msg(client_fd, "IRC Error: not enough parameter");
        return ;
    }
    std::string channel = buffer[1];
    int count = std::count(channel.begin(), channel.end(), ',');
    if (!client_list[client_fd]->get_auth())
    {
        send_msg(client_fd, "IRC Error: you need to be authenticated first");
        return ;
    }

    if (!count)
        count = 1;
    std::vector<char *> names = parse_request(buffer[1], ", ", count);

    for (int x = 0; x < count; x++)
    {
        std::string channel_name = names[x];
        std::map<std::string , Channel *>::iterator it = channel_list.find(channel_name);
        //------if created--------
            //set operator
            //name max 200 ch beginning by & or # -search why- , no space or ctrlg or ,
            //can join multiple channel by separating them with commas
        if (channel_list.empty() || it == channel_list.end())
        {
            if (channel_name[0] != '#'|| std::count(channel_name.begin(), channel_name.end(), '#') > 1 || channel_name.size() > 200 || channel_name.find(' ') != std::string::npos)
            {
                send_msg(client_fd, "IRC Error: ERR_WRONGNAME");
                break ;
            }
            Channel *chat = new Channel(channel_name, "No topic yet", client_fd);
            channel_list[channel_name] = chat;
            send_msg(client_fd, chat->get_client_str(client_list[client_fd]->get_nickname()));
            client_list[client_fd]->add_channel(chat);
            chat->add_to_list(client_list[client_fd]);
            //msg at the creation of the channel ?
            return ;
        }
        else
        {
            if (it->second->is_invite_only())
            {
                std::string msg = "IRC Can't join channel " + it->first + " : Channel is invite only";
                send_msg(client_fd, msg);
            }
            else if (client_list[client_fd]->is_joined(channel_name))
            {
                std::string msg = "IRC Can't join channel " + it->first + " : you already joined this channel";
                send_msg(client_fd, msg);
            }
            else
            {
                 client_list[client_fd]->add_channel(it->second);
                std::string msg = it->first + " User " + client_list[client_fd]->get_nickname() + " just joined this channel !";
                it->second->add_to_list(client_list[client_fd]);
                it->second->send_to_all(client_fd, msg);
                std::string msg2 = "IRC " + client_list[client_fd]->get_nickname()  + " " + it->first + " :<" + it->second->get_topic() + ">";
                send_msg(client_fd, msg2);
                send_msg(client_fd, it->second->get_client_str(client_list[client_fd]->get_nickname()));
            }
        }
    }
    //------if not created-----
        //broadcast to all user if not created
        //if it's invite only, do not join
        //send the topic of the channel
        //:server-name 332 nickname #channel-name :<topic>
        //:server-name 353 nickname = #channel-name :@user1 +user2 user3
}

void	Server::quit_channel(std::string command, int client_fd)
{
    std::vector<char *> buffer = parse_request((char *)command.c_str(), " \r\n", 3);

    std::string channels = buffer[1];
    std::string message = buffer[2];
    int count = std::count(channels.begin(), channels.end(), ',');
    std::vector<char *> names = parse_request(buffer[1], " ,", count);
    if (!client_list[client_fd]->get_auth())
    {
        send_msg(client_fd, "IRC Error: you need to be authenticated first");
        return ;
    }
    if (!count)
        count = 1;
    for (int x = 0; x < count; x++)
    {
        std::string channel_name = names[x];
        channel_list[channel_name]->remove_from_list(client_list[client_fd]);
        if (!message.empty())
            channel_list[channel_name]->send_to_all(client_fd, message);
        if (channel_list[channel_name]->get_operator() == client_fd || channel_list[channel_name]->get_client_list().size() == 0)
        {
            if (!channel_list[channel_name]->get_client_list().empty())
            {
                channel_list[channel_name]->send_to_all(0, "IRC Channel is being terminated");
                channel_list[channel_name]->kick_everyone();
            }
            delete channel_list[channel_name];
            channel_list.erase(channel_name);
        }
    }

}


/*
void	Server::handle_mod(std::vector<std::string> mod_request)
{

}
*/