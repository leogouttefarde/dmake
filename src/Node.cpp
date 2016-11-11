
#include "Node.hpp"


Node::Node(const std::string name) {
	mName = name;
	mIsDone = false;
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

bool Node::isDone() {
	return mIsDone;
}

bool Node::setDone() {
	mIsDone = true;
}

bool Node::isReady() {

	if ( isLeaf() ) {
		return true;
	}

	for (Node *node : mDeps) {

		if ( !node->isDone() ) {
			return false;
		}
	}

	return true;
}

const std::vector<Node*>& Node::getDeps() {
	return mDeps;
}

const std::vector<Node*>& Node::getFathers()
{
	return mFathers;
}

const std::vector<std::string>& Node::getCmds() {
	return mCmds;
}

bool Node::setDeps( std::list<Node*>& nodes, std::list<Node*>& leaves,
					std::map<std::string, Node*>& targets ) {

	bool success = true;

	std::cout << "setDeps IN " << std::endl;

	for (std::string name : mDepNames) {

		// Parcours des dépendances
		std::cerr << "for " << name << std::endl;

		Node *dep = targets[name];

		if (dep != NULL) {

			// Set child deps
			success &= dep->setDeps(nodes, leaves, targets);

			// Add new dep
			mDeps.push_back(dep);


		}
		else {
			std::cout << "FATAL ERROR : missing dependency "
				<< name << std::endl;

			success = false;
		}

		if ( !success ) {
			return success;
		}
	}

	if ( isLeaf() ) {

		// Add leaves
		leaves.push_back(this);
	}
	else {

		// Add highest deps at the end
		nodes.push_back(this);
	}

	return success;
}

