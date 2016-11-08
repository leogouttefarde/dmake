
#pragma once

#include "Node.hpp"
#include <boost/algorithm/string.hpp>
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

