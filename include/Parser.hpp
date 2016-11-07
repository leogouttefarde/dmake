#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class Parser
{
public:
    Parser(int argc, char* argv[]);
    ~Parser();
    int readMakefile();


private:
    std::ifstream* makefile ;
    // std::string path;
    void parseMakefile();
};

#endif //PARSER_HPP
