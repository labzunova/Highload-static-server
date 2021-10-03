#ifndef HIGHLOAD_STATIC_SERVER_HANDLER_H
#define HIGHLOAD_STATIC_SERVER_HANDLER_H

#include "Parser.h"
#include "string"

class Handler {
public:
    Handler() = default;
    void Handle(std::string request, int socket);
private:
    int CheckFile(std::string path, int &size, std::string &body, bool isFileIndicated);
    bool CheckRootEscaping(std::string path);
};


#endif //HIGHLOAD_STATIC_SERVER_HANDLER_H
