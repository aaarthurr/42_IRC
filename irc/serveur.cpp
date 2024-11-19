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
    while (true)
    {
        sock.wait_for_act();
        if (sock.AddClient())
            std::cout << "handling messages.." << std::endl;
    }
}
   catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return (0);
}
 return 0;
}
