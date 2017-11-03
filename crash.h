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
	char ** environVars;
	command * cmd;
	crash(string args, char * environVars[]);
	~crash(void);

	void findPATH(void);	
	void parseArgs(void);
};

#endif