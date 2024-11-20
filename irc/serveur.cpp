#include "Socket.hpp"

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
try {
    // Close sockets
    while (sock.wait_for_act())
    {
        sock.addClient();
        sock.handleMess();
        sock.handleRequest();
    }
}
   catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return (0);
}
 return 0;
}
