#ifndef CRASH_H
#define CRASH_H
#endif

#include <vector>
#include <string>

using namespace std;

class crash {
public:
	string args;
	string path;
	char ** environVars;
	crash(string args, char * environVars[]);
	~crash(void);

	void findPATH(void);
	string findCmdPath(string cmd);
	bool cmdExists(string dir, string cmd);
	vector<string> split(string input, char token);
	void parseArgs(void);
	void forkAndExec(string path, string cmd);
};