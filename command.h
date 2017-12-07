#ifndef _COMMAND_H
#define _COMMAND_H


#include <vector>
#include <string>

using namespace std;

class command {
public:
	string cmd;
	string cmdPath;
	string PATH;
	vector<string> args;

	command(string cmd, vector<string> args, string path);
	~command();
	void error(const char *msg, bool flag);

	static vector<string> split(string input, char token);
	void cleanArgs(void);
	bool cmdExists(string dir);
	void globExpand(void);
	string findCmdPath(void);
	void forkAndExec(void);
	char ** vectorToCharArray(vector<string> strings);
	void execChild(void);


};

#endif