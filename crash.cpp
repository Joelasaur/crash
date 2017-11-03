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
	this->cmd = NULL;
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

void crash::parseArgs(void) {
	findPATH();
	vector<string> parsedArgs = command::split(args, ' ');
	string program = parsedArgs[0];
	cmd = new command(program, parsedArgs, path);
	cmd->forkAndExec();
}