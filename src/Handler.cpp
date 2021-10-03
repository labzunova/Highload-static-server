#include "../include/Handler.h"
#include <iostream>
#include <ctime>
#include <utility>
#include <unistd.h>
#include <fstream>

const std::string root = "/home/labzunova/Highload-static-server/";

void Handler::Handle(std::string request, int socket) {
    std::cout << "handle request start" << std::endl;
    Parser parser(std::move(request));
    std::string method = parser.parse_method();
    std::string path = parser.parse_path();
    std::cout << "handle" << std::endl;

    std::string headers, status, body;
    int size;
    headers.append("\r\nServer: highload_static_server");
    time_t now = time(nullptr);
    headers.append("\r\nDate: " + std::string(ctime(&now)));
    headers.append("Connection: close");
    std::cout << "handle" << std::endl;

    if (CheckRootEscaping(path)) {
        status = "403 Forbidden";
    }
    std::cout << "handle" << std::endl;
    std::string content_type = parser.parse_content_type();

    std::cout << "is file indicated: " << parser.isFileIndicated() << std::endl;
    int statusNum = CheckFile(path, size, body, parser.isFileIndicated());
    std::cout << "handle" << std::endl;
    if (statusNum == 404) {
        status = "404 Not Found";
    }
    if (statusNum == 403) {
        status = "403 Forbidden";
    }

    if (method == "GET" || method == "HEAD") {
        if (statusNum == 200) {
            status = "200 OK";
            headers.append("\r\nContent-Type: " + content_type);
            headers.append("\r\nContent-Length: " + std::to_string(size));
            std::cout << "content-length" << std::to_string(size);
        }
        if (method == "HEAD") {
            body = "";
        }
    } else {
        status = "405 Method Not Allowed";
    }

    std::cout << "handle" << std::endl;

    std::string response = "HTTP/1.1 " + status + headers + "\r\n\r\n" + body;
    std::cout << "response" << response << std::endl;
    write(socket, response.c_str(), response.length());
    close(socket);
}

int Handler::CheckFile(std::string path, int &size, string &body, bool isFileIndicated) {
    if (!isFileIndicated) {
        std::cout << "not indicated" << std::endl;
        path += "index.html";
    }

    std::cout << "root_path: " << root+path << std::endl;
    std::ifstream file(root + path);
    if (file) {
        std::cout << "file";
        file.seekg(0, std::ios::end);
        size_t len = file.tellg();
        file.seekg(0);

        body = std::string(len + 1, '\0');
        file.read(&body[0], len);
        size = file.tellg();
        return 200;
    } else {
        if (isFileIndicated) {
            std::cout << "404";
            return 404;
        } else {
            std::cout << "403";
            return 403;
        }
    }
}

bool Handler::CheckRootEscaping(std::string path) {
    std::cout << "path " << path << std::endl;
    int foldersPos = 0, slash = 0;
    std::string folderDirection;
    path = path.substr(1, path.length());
    while (true) {
        folderDirection = path.substr(0, slash);
        if (path.length() == 0) {
            return false;
        }
        std::cout << "slash " << slash << std::endl;
        std::cout << "path " << path << std::endl;
        std::cout << "folderdir " << folderDirection << std::endl;
        if (folderDirection == "..") {
            foldersPos--;
        } else {
            foldersPos++;
        }
        path = path.substr(slash + 1, path.length());

        if (path.find('/', 0) == std::string::npos) {
            break;
        }
    }

    return foldersPos < 0;
}
