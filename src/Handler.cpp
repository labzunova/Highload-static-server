#include "../include/Handler.h"
#include <ctime>
#include <utility>
#include <unistd.h>

Handler::Handler() {

}

void Handler::Handle(std::string request, int socket) {
    Parser parser(std::move(request));
    std::string method = parser.parse_method();
    std::string path = parser.parse_path();

    std::string headers, status, body;
    int size;
    headers.append("\r\nServer: highload_static_server");
    time_t now = time(nullptr);
    headers.append("\r\nDate: " + std::string(ctime(&now)));
    headers.append("\r\nConnection: close");

    if (CheckRootEscaping(path)) {
        status = "403 Forbidden";
    }

    int statusNum = CheckFile(path, &size, &body);
    if (statusNum == 404) {
        status = "404 Not Found";
    }

    if (method == "GET" || method == "HEAD") {
        if (statusNum == 200) {
            status = "200 OK";
            std::string content_type = parser.parse_content_type();
            headers.append("\r\nContent-type: " + content_type);
            headers.append("\r\nContent-length: " + std::to_string(size));
        }
    } else {
        status = "405 Method Not Allowed";
    }

    std::string response = "HTTP/1.1 " + status + headers + "\r\n\r\n" + body;
    write(socket, response.c_str(), response.length());
    close(socket);
}

int Handler::CheckFile(std::string path, int *size, string *body) {
    return 0;
}

bool Handler::CheckRootEscaping(std::string path) {
    int foldersPos = 0;
    std::string folderDirection;
    path = path.substr(1, path.length());
    while (int slash = path.find('/') != -1) {
        folderDirection += path.substr(0, slash);
        if (folderDirection == "..") {
            foldersPos--;
        } else {
            foldersPos++;
        }
        path = path.substr(slash + 1, path.length());
    }

    return foldersPos < 0;
}
