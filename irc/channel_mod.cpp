#include "Server.hpp"

void Server::handle_mod(std::string buffer, int client_fd)
{
	std::vector<char *> tab = parse_request((char *)buffer.c_str(), " \r\n", 4);

	if (tab.size() < 3 || tab.size() > 4)
	{
		send_msg(client_fd, "Syntaxe : MODE <target> <modes> [<mode params>]");
		return;
	}
	std::cout << "TARGET : " << tab[1] << "\n";
	if (channel_list.find(tab[1]) == channel_list.end())
	{
		send_msg(client_fd, "IRC ERR_CHANNELDOESNTEXIST");
		return;
	}
	else if (channel_list[tab[1]]->get_operator() != client_fd)
	{
		send_msg(client_fd, "IRC ERR_DOESTHAVEOPRIGHTS");
		return;
	}
	std::string mode(tab[2]);
	if (mode == "-i")
	{
		if (!channel_list[tab[1]]->is_invite_only())
		{
			std::string msg = channel_list[tab[1]]->get_name() + " Channel was set to invite only";
			channel_list[tab[1]]->set_invite_only(true);
			channel_list[tab[1]]->send_to_all(0, msg);
		}
		else
			send_msg(client_fd, "IRC Channel already invite only dumbass");
	}
	else if (mode == "-t")
	{
		if (tab.size() > 3)
			channel_list[tab[1]]->set_topic(tab[3]);
		else
			send_msg(client_fd, "IRC ERR_NEEDMOREPARAM");
	}
	else if (mode == "-k")
        std::cout << "Change Key :" << (tab.size() > 3 ? tab[3] : "*no params*") << std::endl;
	else if (mode == "-o")
		channel_list[tab[1]]->set_operator(client_fd, tab[3]);
	else if (mode == "-l")
        std::cout << "Change limit of user :" << (tab.size() > 3 ? tab[3] : "*no params*") << std::endl;
	else
	{
		std::string msg = "Unknown flag : ";
		msg += tab[2];
    	send_msg(client_fd, msg);
	}
    return;
}
