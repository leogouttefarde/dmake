
#include "Node.hpp"


Node::Node(const std::string name) {
	mName = name;
	mTargets = NULL;
}

std::string Node::getName() {
	return mName;
}

void Node::addDepName(const std::string name) {
	mDepNames.push_back(name);
}

void Node::addCmd(const std::string cmd) {
	mCmds.push_back(cmd);
}

const std::vector<std::string>& Node::getDepNames() {
	return mDepNames;
}

bool Node::isLeaf() {
	return !mDeps.size() && !mDepNames.size();
}

const std::vector<Node*>& Node::getDeps() {
	return mDeps;
}

const std::vector<std::string>& Node::getCmds() {
	return mCmds;
}

void Node::setDeps( std::queue<Node*>& nodes, std::queue<Node*>& leaves ) {

	std::map<std::string,Node*>& targets = *mTargets;

	std::cout << "setDeps IN " << std::endl;

	for (std::string name : mDepNames) {
	std::cout << "for " << name << std::endl;

		Node *dep = targets[name];

		if (dep != NULL) {

			// Set child deps
			dep->setDeps(nodes, leaves);

			// Add new dep
			mDeps.push_back(dep);

		}
		else {
			std::cout << "FATAL ERROR : missing dependency "
				<< name << std::endl;
		}
	}

	if ( isLeaf() ) {

		// Add leaves
		leaves.push(this);
	}
	else {

		// Add highest deps at the end
		nodes.push(this);
	}
}

