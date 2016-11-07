
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

	void addDependency(Node *dep) {
		mDeps.push_back(dep);
	}

	void addCmd(const std::string cmd) {
		mCmds.push_back(cmd);
	}

	const std::vector<Node*>& getDeps() {
		return mDeps;
	}

	const std::vector<std::string>& getCmds() {
		return mCmds;
	}


private:
	std::string mName;

	std::vector<Node*> mDeps;
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
				mNext = mNext.substr(pos);

				return true;
			}
			// else
			// 	std::cout << "Wild line" << std::endl;

		}

		return false;
	}

	void parseDependencies(Node *pTarget) {

	}

	void parseCommands(Node *pTarget) {

	}

	Node* parseTarget() {

		std::string name;

		if ( !parseRuleName(name) ) {
			return NULL;
		}

		Node *pTarget = new Node(name);

		parseDependencies(pTarget);
		parseCommands(pTarget);

		std::cout << "Target name : '" << name << "'" << std::endl;

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


