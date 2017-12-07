// Standard Includes
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>

// Local Includes
#include "crash.h"
#include "command.h"


using namespace std;

crash::crash(string args, string prompt, char ** environVars) {
	this->args = args;
	this->environVars = environVars;
	this->prompt = prompt;
	this->path = "";
	this->cmds = new vector<command*>;
}

crash::~crash(void) {

}

void crash::error(const char *msg, bool flag) {
	if (flag) {
		perror(msg);
		exit(1);
	}
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

void crash::setEnvVariable(int position, string envVar) {
	string var = envVar.substr(0, position);
	string value = envVar.substr(position);
	if (var == "PS1") {
		prompt = value;
	}
	else {
		error("Could not set environment variable", setenv(var.c_str(), value.c_str(), 1) == -1);
	}
}

vector<string> crash::expandEnvVariables(vector<string> parsedArgs) {
	// Iterate through each argument that needs to be expanded
	for (auto argsIterator = parsedArgs.begin(); argsIterator != parsedArgs.end(); ++argsIterator) {
		int envVarPos = (*argsIterator).find("$");
		if (!(envVarPos == -1)) {
			// Drop the $ character and get the value
			string envVar = (*argsIterator).substr(1);
			(*argsIterator) = getenv(envVar.c_str());
			if (argsIterator == parsedArgs.begin()) {
				return parsedArgs;
			}
		}
	}
	return parsedArgs;
}

void crash::parseArgs(vector<string> parsedArgs) {
	findPATH();
	int equalsPosition = parsedArgs[0].find("=");
	int variablePosition = parsedArgs[0].find("$");
	//Set the environment variable
	if ((parsedArgs.size() == 1) && (equalsPosition != -1)) {
		setEnvVariable(equalsPosition, parsedArgs[0]);
	} 
	//Execute the command and its arguments
	else {
		string program;
		parsedArgs = expandEnvVariables(parsedArgs);
		//We are expanding just the first environment variable, so print it
		if (variablePosition != -1) {
			program = "echo";
		}
		//We are executing a command
		else {
			program = parsedArgs[0];
			parsedArgs.erase(parsedArgs.begin());
		}
		command * cmd = new command(program, parsedArgs, path);
		cmds->push_back(cmd);
		execCmds();	
	}

}