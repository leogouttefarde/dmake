
#include "Node.hpp"
#include <fstream>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace boost::posix_time;
using namespace boost::filesystem;
    

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

bool Node::needToBeBuilt(){
    std::cout << "\t NeedToBeBuilt : " << mName;

    // if the calculation has already be done
    if (mNeedToBeBuilt)
        return mNeedToBeBuilt.get();

    // we check the children before to check the existence
    // to force the recurcive call
    for (int i = 0; i < mDeps.size(); i++){
        if (mDeps[i]->needToBeBuilt()){
            mNeedToBeBuilt = true;
            return true;
        }
    }
    
    // if the file does not exist it has to be built
    std::ifstream fi(mName);  
    if(fi.fail()){
        mNeedToBeBuilt = true;
        return true;
    }
    
    time_t timeLastBuild = last_write_time(mName);

    for (int i = 0; i < mDeps.size(); i++){
	time_t depLastBuild = last_write_time( mDeps[i]->getName());
	if ( depLastBuild > timeLastBuild){
	    mNeedToBeBuilt = true;
	    return true;
	}
    }

    mNeedToBeBuilt = false;
    return false;
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

