#ifndef CRASH_H
#define CRASH_H
#endif

#include <vector>
#include <string>

using namespace std;

class crash {
public:
	vector<string> args;
	string path;
	char ** environVars;
	crash(vector<string> args, char * environVars[]);
	~crash(void);

	void printArgs(void);
	void findPATH(void);
	vector<string> splitPaths(void);
};