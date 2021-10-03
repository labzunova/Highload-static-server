#ifndef HIGHLOAD_STATIC_SERVER_PARSER_H
#define HIGHLOAD_STATIC_SERVER_PARSER_H

#include <string>
#include <unordered_map>
#include <utility>

using std::unordered_map;
using std::string;

class Parser
{
public:
    Parser( string request_ ): request( std::move( request_ ) ) {};
    string parse_method();
    string parse_path();
    string parse_content_type();
    bool isFileIndicated() const;
private:
    string request;
    std::string _path;
    bool _isFileIndicated;
    static std::string DecodeUrl(std::string filePath);
};

#endif //HIGHLOAD_STATIC_SERVER_PARSER_H
