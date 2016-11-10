
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

bool Parser::IsOk() {
	return mFile.is_open();
}

void Parser::finish() {
	mFile.close();
}

bool Parser::parseRuleName(std::string& oName, std::string& nextLine) {

	while ( std::getline(mFile, nextLine) ) {

		size_t pos = nextLine.find(':');

		if ( pos != std::string::npos ) {
			oName = nextLine.substr(0, pos);
			oName = trim(oName);

			// Remove rule name from buffer
			nextLine = nextLine.substr(pos + 1);

			return true;
		}
		// else
		// 	std::cout << "Wild line" << std::endl;

	}

	return false;
}

void Parser::parseDependencies(Node *pTarget, std::string nextLine) {

	std::stringstream ss(nextLine);
	std::string dep;

	while ( std::getline(ss, dep, ' ') ) {
		dep = trim(dep);

		if (dep.size() > 0) {
			pTarget->addDepName(dep);
			std::cout << "Dependency added : '" << dep << "'" << std::endl;
		}
	}
}

void Parser::parseCommands(Node *pTarget) {

	std::string commands;

	while ( std::getline(mFile, commands) ) {
		commands = trim(commands);

		if (commands.size() > 0) {
			pTarget->addCmd(commands);
			std::cout << "Command added : '" << commands << "'" << std::endl;
		}
		else {
			break;
		}
	}
}

Node* Parser::parseTarget() {

	std::string name;
	std::string nextLine; // will contain the dependencies after parsing the name of the rule

	if ( !parseRuleName(name, nextLine) ) {
		return NULL;
	}

	std::cout << "Target name : '" << nextLine << "'" << std::endl;

	Node *pTarget = new Node(name);
	if (pTarget == NULL) {
		return NULL;
	}

	parseDependencies(pTarget, nextLine);
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

	// 16:07 : Leo a dit que je peux changer
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

