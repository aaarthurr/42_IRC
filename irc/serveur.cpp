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
            int activity = sock.wait_for_act();
            if (activity == -1)
            {
                std::cerr << "poll() error" << std::endl;
            }
            sock.addClient();
        // sock.handleMess();
            //sock.handleRequest();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return (0);
    }
    return 0;
}
