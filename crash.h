#ifndef _CRASH_H
#define _CRASH_H

#include <vector>
#include <string>
#include "command.h"
using namespace std;

class crash {
public:
	string args;
	string path;
	string prompt;
	char ** environVars;
	vector<command *> *cmds;
	crash(string args, string prompt, char * environVars[]);
	~crash(void);

	vector<string> expandEnvVariables(vector<string> parsedArgs);
	static void addCmdsFromFile(string filename);
	void error(const char *msg, bool flag);
	void setEnvVariable(int position, string envVar);
	void findPATH(void);	
	void parseArgs(vector<string> parsedArgs);
	void execCmds(void);
};

#endif