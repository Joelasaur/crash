// Standard Includes
#include <iostream>
#include <vector>
#include <string>
#include <string.h>

// Local Includes
#include "crash.h"
#include "command.h"


using namespace std;

crash::crash(string args, char ** environVars) {
	this->args = args;
	this->environVars = environVars;
	this->path = "";
	this->cmds = new vector<command*>;
}

crash::~crash(void) {

}

void crash::findPATH(void) {
	int i = 0;
	while (environVars[i] != NULL) {
		if (strncmp(environVars[i], "PATH=", 5) == 0) {
			path = environVars[i];
			path = path.substr(5);
		}
		i++;
	}
}

void crash::execCmds(void) {
	for (auto cmdIterator = cmds->begin(); cmdIterator != cmds->end(); ++cmdIterator) {
		(*cmdIterator)->forkAndExec();
	}
}

void crash::parseArgs(void) {
	findPATH();
	vector<string> parsedArgs = command::split(args, ' ');
	string program = parsedArgs[0];
	command * cmd = new command(program, parsedArgs, path);
	cmds->push_back(cmd);
	execCmds();
}