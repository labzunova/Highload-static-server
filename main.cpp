#include "iostream"
#include "thread"
#include <unistd.h>
#include "map"
#include <sys/socket.h>
#include "istream"
#include "include/ThreadPool.h"
#include <netinet/in.h>
#include <fstream>

int bufferSize = 10000;

//const std::string root = "/home/labzunova/Highload-static-server/";

struct Config {
    std::string documentRoot;
    int threadLimit;
};

Config parseConfig() {
    std::string configPath = "../etc/httpd.conf";
    std::ifstream file(configPath);
    if (!file.is_open()) {
        std::cout << ("can't open config file") << std::endl;
    }

    std::map<std::string, std::string> config;
    for (std::string line; std::getline(file, line);) {
        int space = line.find(' ');
        if (space == std::string::npos) {
            std::cout << ("error parsing config") << std::endl;
        }
        std::string key = line.substr(0, space);
        std::string value = line.substr(space + 1, line.find(' ', space + 1));
        config.emplace(key, value);
    }

    auto threads = config.find("thread_limit");

    Config result;
    result.documentRoot = config.find("document_root")->second;
    result.threadLimit = std::stoi(config.find("thread_limit")->second);
    return result;
}

int main() {
    Config config = parseConfig();
    int num_threads = std::thread::hardware_concurrency();
    num_threads = std::min(num_threads, config.threadLimit);

    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0); //second parameter is the type of the socket, SOCK_STREAM opens a connection ( use for TCD ), SOCK_DGRAM doesn't connect() or accept() it's used for UDP
    if (sock <= 0) {
        std::cout << "invalid socket";
        return -1;
    }

    sockaddr_in sin{};
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(8100);

    int sizeof_sin = sizeof(sin);
    int socketFD = bind(sock, (sockaddr *)&sin, sizeof_sin);
    if (socketFD < 0) {
        std::cout << "bind wasn't made" << std::endl;
        return -1;
    }

    int err = listen(sock, 10);
    if (err < 0) {
        std::cout << "listen() error" << std::endl;
        return -1;
    }

    ThreadPool threadPool(num_threads, config.documentRoot);
    std::cout << "Thread pool initialized" << std::endl;

    while(true) { // listen and serve
        int socket = accept(sock, (struct sockaddr *)&sin, (socklen_t*)&sizeof_sin);
        std::cout << socket << std::endl;
        if (socket < 0) {
            std::cout << "Accept() error" << std::endl;
            return -1;
        }
        std::cout << "Connection ok" << std::endl;

        char buffer[bufferSize];
        read(socket, buffer, bufferSize);
        threadPool.PushTask(socket, std::string(buffer));
    }
}
