
#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <map>


class Node {

public:

	Node(const std::string name);

	std::string getName();

	void addDepName(const std::string name);

	void addCmd(const std::string cmd);

	const std::vector<std::string>& getDepNames();

	bool isLeaf();

	const std::vector<Node*>& getDeps();

	const std::vector<std::string>& getCmds();

	void setDeps( std::queue<Node*>& nodes, std::queue<Node*>& leaves );


	std::map<std::string,Node*> *mTargets;


private:
	std::string mName;

	std::vector<Node*> mDeps;
	std::vector<std::string> mDepNames;

	std::vector<std::string> mCmds;

};

