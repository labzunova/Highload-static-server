#include <iostream>
#include "../include/Parser.h"

string Parser::parse_method()
{
    return request.substr( 0, request.find(' '));// первое слово до пробела в запросе - метод
}

string Parser::parse_path()
{
    string  temp = request,
            path;
    temp = temp.erase(0, request.find(' ') + 1); // удаляем метод и пробел между методом и путем
    path = temp.substr( 0, temp.find(' ')); // теперь часть до пробела - путь
    std::cout << "parsed path" << path << std::endl;
    if ((path.at(0) == '/') & (path.length() > 1))
        path = path.erase( 0, 1 );
    if (path.find('?') != std::string::npos) {
        path.erase(path.find('?'), path.length());
    }
    path = DecodeUrl(path);
    std::cout << "parsed path" << path << std::endl;
    _path = path;
    return path;
}

string Parser::parse_content_type() {
    _isFileIndicated = true;
    if (_path.find(".html", 0) != std::string::npos) {
        return "text/html";
    }
    if (_path.find(".txt", 0) != std::string::npos) {
        return "text/txt";
    }
    if (_path.find(".css", 0) != std::string::npos) {
        return "text/css";
    }
    if (_path.find(".js", 0) != std::string::npos) {
        return "text/javascript";
    }
    if (_path.find(".jpg", 0) != std::string::npos) {
        return "image/jpeg";
    }
    if (_path.find(".jpeg", 0) != std::string::npos) {
        return "image/jpeg";
    }
    if (_path.find(".png", 0) != std::string::npos) {
        return "image/png";
    }
    if (_path.find(".gif", 0) != std::string::npos) {
        return "image/gif";
    }
    if (_path.find(".swf", 0) != std::string::npos) {
        return "application/x-shockwave-flash";
    }
    _isFileIndicated = false;
    return "text/html";
}

std::string Parser::DecodeUrl(std::string filePath) {
    std::string resultPath;
    for (int i = 0; i < filePath.size(); ++i) {
        if (filePath[i] == '%') {
            int nSymbol;
            sscanf(filePath.substr(i + 1, 2).c_str(), "%x", &nSymbol);
            char ch = static_cast<char>(nSymbol);
            resultPath+= ch;
            i += 2;
        } else {
            resultPath += filePath[i];
        }
    }

    return resultPath;
}

bool Parser::isFileIndicated() {
    return _isFileIndicated;
}
