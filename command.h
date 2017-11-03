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

	static vector<string> split(string input, char token);
	bool cmdExists(string dir);
	string findCmdPath(void);
	void forkAndExec(void);
	char ** vectorToCharArray(vector<string> strings);


};

#endif