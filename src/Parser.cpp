
#include "Parser.hpp"


Parser::Parser(const std::string path) {

		mFile.open(path, std::ifstream::in);

		if (mFile.is_open()) {
			std::cout << "Parser started";
		}
		else {
			std::cout << "Error opening file";
		}

		std::cout << std::endl;
}

bool Parser::Parser::IsOk() {
	return mFile.is_open();
}

void Parser::finish() {
	mFile.close();
}

bool Parser::parseRuleName(std::string& oName) {

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

void Parser::parseDependencies(Node *pTarget) {

	if (pTarget == NULL) {
		return;
	}

	std::stringstream ss(mNext);
	std::string dep;

	while ( std::getline(ss, dep, ' ') ) {
		boost::algorithm::trim(dep);

		if (dep.size() > 0) {
			pTarget->addDepName(dep);
			std::cout << "Dependency added : '" << dep << "'" << std::endl;
		}
	}
}

void Parser::parseCommands(Node *pTarget) {

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

Node* Parser::parseTarget() {

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

Node* Parser::parseFile() {
	Node *first = NULL;

	if (IsOk()) {
		Node *pTarget = NULL;

		do {
			pTarget = parseTarget();

			if ( pTarget != NULL ) {
				mTargets[ pTarget->getName() ] = pTarget;

				if ( first == NULL ) {
					first = pTarget;
				}
			}

		} while ( pTarget != NULL );
	}

	if ( first != NULL ) {
		first->mTargets = new std::map<std::string,Node*>(mTargets);
	}

	return first;
}

Node* Parser::ParseFile(const std::string path) {

	Parser parser(path);

	Node *first	= parser.parseFile();
	parser.finish();

	return first;
}

