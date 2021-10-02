#include "../include/Parser.h"
//
//Request Parser::ParseHTTP(string requestString) {
//    Request request;
//    int methodPos = requestString.find(' ')
//    request.method = request.substr(0, methodPos);// первое слово до пробела в запросе - метод
//    request.path = request.substr(methodPos + 1, );
//
//    return request;
//}

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
    if (path.at(0) == '/')
        path = path.erase( 0, 1 );
    path = DecodeUrl(path);
    _path = path;
    return path;
}

unordered_map<string, string> Parser::parse_body()
{
    string request_ = request;
    unordered_map<string, string> data;
    string body = request_.erase( 0, request_.find( "\r\n\r\n" ) + 4 ); // стираем все что до body
    body = body.substr( 0 , body.find( '\000' ) );
    string key, value;
    while ( body.length() != 0 )
    {
        int index = body.find( '=' );
        key = body.substr( 0, index );
        int endpos = body.find( '&' );
        if( endpos == -1 ) endpos = body.length(); // если последний параметр
        value = body.substr( index + 1, endpos - index - 1 );
        data.insert(std::make_pair( key, value ));
        body.erase( 0, endpos + 1 );
    }
    return data;
}

string Parser::parse_content_type() {
    if (_path.find(".html") > 0) {
        return "text/html";
    }
    if (_path.find(".css") > 0) {
        return "text/css";
    }
    if (_path.find(".js") > 0) {
        return "text/js";
    }
    if (_path.find(".jpg") > 0) {
        return "image/jpg";
    }
    if (_path.find(".jpeg") > 0) {
        return "image/jpeg";
    }
    if (_path.find(".png") > 0) {
        return "image/png";
    }
    if (_path.find(".gif") > 0) {
        return "image/gif";
    }
    if (_path.find(".swf") > 0) {
        return "application/x-shockwave-flash";
    }
    return "";
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
