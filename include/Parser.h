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
    unordered_map<string, string> parse_body();
    string parse_method();
    string parse_path();
    string parse_content_type();
private:
    string request;
    std::string _path;
    std::string DecodeUrl(std::string filePath);
};

#endif //HIGHLOAD_STATIC_SERVER_PARSER_H
