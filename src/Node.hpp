
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>


class Node {

public:

	Node(const std::string name);

	std::string getName();

	void addDepName(const std::string name);

	void addCmd(const std::string cmd);

	const std::vector<std::string>& getDepNames();

	bool isLeaf();

	bool isReady();

	bool isDone();

	bool setDone();

	const std::vector<Node*>& getDeps();

	const std::vector<Node*>& getFathers();

	const std::vector<std::string>& getCmds();

	void setDeps( std::list<Node*>& nodes, std::list<Node*>& leaves, std::map<std::string, Node*>& targets);


	std::map<std::string,Node*> *mTargets;

	int 						mNbUnfinishedSons;
	bool	 					mNeedToBeBuilt;
private:
	std::string 				mName;

	std::vector<Node*> 			mDeps;
	std::vector<Node*> 			mFathers;

	std::vector<std::string> 	mDepNames;
	std::vector<std::string> 	mCmds;

	bool	 					mIsDone;
};

