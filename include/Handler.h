#ifndef HIGHLOAD_STATIC_SERVER_HANDLER_H
#define HIGHLOAD_STATIC_SERVER_HANDLER_H

#include <utility>
#include "string"

#include "Parser.h"

class Handler {
public:
    Handler() = default;
    Handler(std::string doc_root): _doc_root(std::move(doc_root)){};
    void Handle(int socket);
private:
    int CheckFile(std::string path, int &size, std::string &body, bool isFileIndicated);
    static bool CheckRootEscaping(std::string path);
    std::string _doc_root;
};


#endif //HIGHLOAD_STATIC_SERVER_HANDLER_H
