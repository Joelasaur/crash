#ifndef _CRASH_H
#define _CRASH_H

#include <vector>
#include <string>
#include <map>
#include "command.h"
using namespace std;

class crash {
public:
	string args;
	string path;
	string prompt;
	char ** environVars;
	vector<command *> *cmds;
	map<string, string> *aliasMap;
	crash(string args, string prompt, char * environVars[], map<string, string> *aliasMap);
	~crash(void);

	vector<string> expandEnvVariables(vector<string> parsedArgs);
	static void addCmdsFromFile(string filename);
	string getAlias(string alias);
	string createAlias(string aliasExpression);
	void error(const char *msg, bool flag);
	void setEnvVariable(int position, string envVar);
	void findPATH(void);	
	int parseArgs(vector<string> parsedArgs);
	void execCmds(void);
};

#endif