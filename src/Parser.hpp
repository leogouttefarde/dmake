
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


class Node {

public:

	Node(const std::string name) {
		mName = name;
	}

	std::string getName() {
		return mName;
	}

	void addDependency(const std::string dep) {
		mDeps.push_back(dep);
	}

	void addCmd(const std::string cmd) {
		mCmds.push_back(cmd);
	}

	const std::vector<std::string>& getDeps() {
		return mDeps;
	}

	const std::vector<std::string>& getCmds() {
		return mCmds;
	}


private:
	std::string mName;

	std::vector<std::string> mDeps;
	std::vector<std::string> mCmds;

};


class Parser {

public:

	Parser(const std::string path) {

		mFile.open(path, std::fstream::in);

		if (mFile.is_open()) {
			std::cout << "Parser started";
		}
		else {
			std::cout << "Error opening file";
		}

		std::cout << std::endl;
	}

	bool IsOk() {
		return mFile.is_open();
	}

	void finish() {
		mFile.close();
	}

	bool parseRuleName(std::string& oName) {

		while ( std::getline(mFile, mNext) ) {

			size_t pos = mNext.find(':');

			if ( pos != std::string::npos ) {
				oName = mNext.substr(0, pos);
				boost::algorithm::trim(oName);

				// Remove rule name from buffer
				mNext = mNext.substr(pos + 1);

				return true;
			}
			// else
			// 	std::cout << "Wild line" << std::endl;

		}

		return false;
	}

	void parseDependencies(Node *pTarget) {

		if (pTarget == NULL) {
			return;
		}

		std::stringstream ss(mNext);
		std::string dep;

		while ( std::getline(ss, dep, ' ') ) {
			boost::algorithm::trim(dep);

			if (dep.size() > 0) {
				pTarget->addDependency(dep);
				std::cout << "Dependency added : '" << dep << "'" << std::endl;
			}
		}
	}

	void parseCommands(Node *pTarget) {

		if (pTarget == NULL) {
			return;
		}

		while ( std::getline(mFile, mNext) ) {
			boost::algorithm::trim(mNext);

			if (mNext.size() > 0) {
				pTarget->addCmd(mNext);
				std::cout << "Command added : '" << mNext << "'" << std::endl;
			}
			else {
				break;
			}
		}
	}

	Node* parseTarget() {

		std::string name;

		if ( !parseRuleName(name) ) {
			return NULL;
		}

		std::cout << "Target name : '" << name << "'" << std::endl;

		Node *pTarget = new Node(name);

		parseDependencies(pTarget);
		parseCommands(pTarget);

		std::cout << std::endl;

		return pTarget;
	}

	Node* parseFile() {

		if (IsOk()) {
			Node *pTarget = NULL;

			do {
				pTarget = parseTarget();

				if ( pTarget != NULL ) {
					mTargets[ pTarget->getName() ] = pTarget;
				}

			} while ( pTarget != NULL );
		}

		return mTargets["all"];
	}

	static Node* ParseFile(const std::string path) {
		Parser parser(path);

		Node *tree = parser.parseFile();
		parser.finish();

		return tree;
	}


private:
	std::fstream mFile;
	std::string mNext;

	std::map<std::string,Node*> mTargets;

};


