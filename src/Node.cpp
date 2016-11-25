
#include "Node.hpp"
#include <fstream>


Node::Node(const std::string name, bool isDone) {
	mName = name;
	mIsDone = isDone;
	mDepsOk = false;
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

void Node::setDone() {
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

const std::vector<std::string>& Node::getCmds() {
	return mCmds;
}

bool Node::setDeps( std::list<Node*>& nodes, std::list<Node*>& leaves,
					std::map<std::string, Node*>& targets ) {

	bool success = true;

	// Process each node only one time
	if ( mDepsOk ) {
		return success;
	}

	//std::cout << "setDeps IN " << std::endl;

	for (std::string name : mDepNames) {

		// Parcours des dépendances
		//std::cerr << "for " << name << std::endl;

		Node *dep = targets[name];

		// If dependency not found but already existing,
		// create the node and set it to "built" state
		if ( dep == NULL && std::ifstream( name ).good() ) {
			dep = new Node(name, true);
			targets[name] = dep;
		}

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

	mDepsOk = true;

	return success;
}

