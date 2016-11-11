
#pragma once

#include "Node.hpp"
// #include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>


class Parser {

public:

	Parser(const std::string path);

	bool IsOk();

	void finish();

	bool parseRuleName(std::string& oName);

	void parseDependencies(Node *pTarget);

	void parseCommands(Node *pTarget);

	Node* parseTarget();

	Node* parseFile();

	static Node* ParseFile(const std::string path);


private:
	std::ifstream mFile;
	std::string mNext;

	std::map<std::string,Node*> mTargets;

};


#include <cctype>
#include <algorithm>

inline std::string trim(const std::string &s)
{
   auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
   auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
   return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
}

