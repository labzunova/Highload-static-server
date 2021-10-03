#include "iostream"
#include "thread"
#include <unistd.h>
#include <sys/socket.h>
#include "include/ThreadPool.h"
#include <netinet/in.h>

int bufferSize = 10000;

int main() {
//    int num_threads = std::thread::hardware_concurrency();
    int num_threads = 100; // todo

    int sock; //defines the sockets
    sock = socket(AF_INET, SOCK_STREAM, 0); //second parameter is the type of the socket, SOCK_STREAM opens a connection ( use for TCD ), SOCK_DGRAM doesn't connect() or accept() it's used for UDP
    if (sock <= 0) {
        std::cout << "invalid socket";
        return 0;
    }
    std::cout << "socket() was made" << std::endl;

    sockaddr_in sin{}; //information about the socket
//    sin.sin_addr.s_addr = htonl(INADDR_ANY); //since it's the server we accept any connection
//    sin.sin_family = AF_INET; //family of the socket, for internet it's AF_INET
//    sin.sin_port = htons(1234); // 23 for telnet etc, it's the port
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(8100);

    int sizeof_sin = sizeof(sin); //size of the socket used to take the information from the client connected
    int socketFD = bind(sock, (sockaddr *)&sin, sizeof_sin);
    if (socketFD < 0) {
        std::cout << "bind wasn't made" << std::endl;
        return 0;
    }

    int err = listen(sock, 10); //listens on the port of the socket, second parameter is the maximum of connections accepted
    if (err < 0) {
        std::cout << "listen() error" << std::endl;
        return 0;
    }
    std::cout << "listen() was made" << std::endl;

    ThreadPool threadPool(num_threads);
    std::cout << "Thread pool initialized" << std::endl;

    while(true) { // listen and serve
//        int socket = accept(sock, (sockaddr *)&sin, (socklen_t*)&sizeof_sin); //first parameter : socket, second parameter : client information socket, third parameter : size of the information about the socket
        std::cout << "sizeof sin: " << sizeof_sin << std::endl;
        int socket = accept(sock, (struct sockaddr *)&sin, (socklen_t*)&sizeof_sin); //first parameter : socket, second parameter : client information socket, third parameter : size of the information about the socket
        std::cout << socket << std::endl;
        if (socket < 0) {
            std::cout << "Accept() error" << std::endl;
            return 0;
        }
        std::cout << "Connection ok" << std::endl;

        char buffer[1000];
        read(socket, buffer, 1000);
        threadPool.PushTask(socket, reinterpret_cast<string &>(buffer));
    }
}
