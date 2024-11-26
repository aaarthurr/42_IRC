#include "Server.hpp"

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: ./irc <port> <password>" << std::endl;
		return (0);
	}
	int port;
	std::istringstream(argv[1]) >> port;
	try
	{
		Server server(port, argv[2]);
		server.start_server();
		server.handle_request();

	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}