#include "irc.hpp"
int main() {

    Socket sock;
try
{
    sock.connectSocket(8080);

}
catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return (0);
}
    std::cout << "Connection accepted!\n";
    // Close sockets
    char *exit;
    while ( exit != "exit")
    {
        sock.receiveMessages();
        read(0, exit, 100);
    }
    return 0;
}
