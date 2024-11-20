#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <poll.h>
#include <vector>

class User
{
    protected:
        std::string username;
        std::string nickname;
        int         sock;
    public:
        User();
        User(const User &usr);
        User(std::string username, std::string nickname, int sock);
        std::string getUsername();
        std::string getNickname();
        int         getClientSock();
        static void sendText(std::string text, int dest_sock, const std::vector<pollfd>& fds);
        ~User();
};

class Admin : public User
{
	public:
		Admin(User usr);
		~Admin();
};