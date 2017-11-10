// Standard Includes
#include <vector>
#include <string>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdlib>

// Local Includes
#include "command.h"

using namespace std;

command::command(string cmd, vector<string> args, string PATH) {
	this->cmd = cmd;
	this->args = args;
	this->PATH = PATH;
	this->cmdPath = "";
}

command::~command(void) {

}

void command::error(const char *msg, bool flag) {
	if (flag) {
		perror(msg);
		exit(1);
	}
}

// Adapted this method from a combination of answers from stack overflow:
// https://stackoverflow.com/questions/53849/how-do-i-tokenize-a-string-in-c
// https://stackoverflow.com/questions/236129/the-most-elegant-way-to-iterate-the-words-of-a-string
vector<string> command::split(string input, char token) {
	stringstream ss(input);
	string str;
	vector<string> words;

	while (getline(ss, str, token)) {
		words.push_back(str);
	}
	return words;
}

// Used this vector<string> to char** conversion method from stackoverflow:
// https://stackoverflow.com/questions/7048888/stdvectorstdstring-to-char-array
// https://stackoverflow.com/questions/26032039/convert-vectorstring-into-char-c
/* After much debugging, the following comment saved my skin:
"you could also do std::vector<const char *>cStrArray( origVector.size()+1, NULL); and then in the iterator use 
cStrArray[i]=origVector[i].c_str(); This can help with functs like execv(). 
But as the note above says, we could use more info about ModelInitialize. – don bright Jul 14 '12 at 21:44 "

This method was pure cancer for me to figure out, and I actually still don't understand why it works. I can't even write
a proper unit test for it lmao ¯\_(ツ)_/¯
*/
char ** command::vectorToCharArray(vector<string> strings) {
	char** cstrings = new char*[strings.size()]; 

	size_t i = 0;
	while(i < strings.size()) {
		cstrings[i] = new char[strings[i].size() + 1]; //allocate enough room to null terminate the c strings
		strcpy(cstrings[i], strings[i].c_str());
		++i;
	}
	cstrings[i] = NULL;
	return cstrings;
}

bool command::cmdExists(string dir) {
	string name = dir + "/" + cmd;
	return ( access( name.c_str(), F_OK ) != -1 );
}

string command::findCmdPath() {
	vector<string> splitPaths = split(PATH, ':');
	vector<string>::iterator p = splitPaths.begin();
	while(p != splitPaths.end()){
		if(cmdExists(*p)) {
			cmdPath = *p + "/" + cmd;
			return cmdPath;
		}
		p++;
	}
	return "";
}

void command::execChild() {
	if (args[0] == "cd") {
		error("Change Directory failed", chdir(args[1].c_str()) < 0);
	}
	else {
		string pth = findCmdPath();
		error("Could not find command", pth == "");
		args[0] = pth; //replace command with path + command
		char ** cmdArgs = vectorToCharArray(args);
		error("Exec failed", execv(cmdPath.c_str(), cmdArgs) == -1);
	}
}

// Adapted this method from stackoverflow:
// https://stackoverflow.com/questions/19099663/how-to-correctly-use-fork-exec-wait
void command::forkAndExec() {
	pid_t pid = fork();
	error("Fork failed", pid == -1);
	if (pid > 0) {
		// We are the parent
    	int status;
    	waitpid(pid, &status, 0);
	}
	else {
		// We are the child
		execChild();
	}

}