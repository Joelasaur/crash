// Standard Includes
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <map>

// Local Includes
#include "crash.h"
#include "command.h"


using namespace std;

crash::crash(string args, string prompt, char ** environVars, map<string, string> *aliasMap) {
	this->args = args;
	this->environVars = environVars;
	this->prompt = prompt;
	this->path = "";
	this->cmds = new vector<command*>;
	this->aliasMap = aliasMap;
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
	string value = envVar.substr(position + 1);
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

string crash::createAlias(string aliasExpression) {
	int equalsPosition = aliasExpression.find("=");
	string alias = aliasExpression.substr(0, equalsPosition);
	string aliasVal = aliasExpression.substr(equalsPosition + 1);
	aliasMap->insert(pair<string, string>(alias, aliasVal));
	return alias;
}

//http://www.cplusplus.com/reference/map/map/find/
string crash::getAlias(string alias) {
	map<string, string>::iterator it;
	it = aliasMap->find(alias);
	if (it != aliasMap->end()) {
		return aliasMap->at(alias);
	}
	return alias;
}

int crash::parseArgs(vector<string> parsedArgs) {
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
		string alias;
		parsedArgs = expandEnvVariables(parsedArgs);
		//We are expanding just the first environment variable, so print it
		if (variablePosition != -1) {
			program = "echo";
			alias = program;
		}
		else if (parsedArgs[0] == "alias") {
			alias = createAlias(parsedArgs[1]);
			return 0;
		}
		//We are executing a command
		else {
			program = parsedArgs[0];
			alias = program;
			parsedArgs.erase(parsedArgs.begin());
		}
		program = getAlias(alias);
		command * cmd = new command(program, parsedArgs, path);
		cmds->push_back(cmd);
		execCmds();	
	}
	return 0;

}