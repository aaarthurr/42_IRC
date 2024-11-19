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

class User
{
    protected:
        std::string username;
        std::string nickname;
        std::string mdp;
        int         sock;
    public:
        User();
        User(const User &usr);
        User(std::string username, std::string nickname, std::string mdp, int sock);
        std::string getUsername();
        std::string getNickname();
        std::string getMdp();
        int         getClientSock();
        void        sendText(std::string text, int dest_sock);
        ~User();
};

class Admin : public User
{
	public:
		Admin(User usr);
		~Admin();
};