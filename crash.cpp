#include <iostream>
#include <vector>
#include <string.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "crash.h"

using namespace std;

crash::crash(string args, char ** environVars) {
	this->args = args;
	this->environVars = environVars;
	this->path = "";
}

crash::~crash(void) {

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
// Adapted this method from a combination of answers from stack overflow:
// https://stackoverflow.com/questions/53849/how-do-i-tokenize-a-string-in-c
// https://stackoverflow.com/questions/236129/the-most-elegant-way-to-iterate-the-words-of-a-string
vector<string> crash::split(string input, char token) {
	stringstream ss(input);
	string str;
	vector<string> words;

	while (getline(ss, str, token)) {
		words.push_back(str);
	}
	return words;
}

bool crash::cmdExists(string dir, string cmd) {
	string name = dir + "/" + cmd;
	return ( access( name.c_str(), F_OK ) != -1 );
}

string crash::findCmdPath(string cmd) {
	string cmdPath = "";
	vector<string> splitPaths = split(path, ':');
	vector<string>::iterator p = splitPaths.begin();
	while(p != splitPaths.end()){
		if(cmdExists(*p, cmd)) {
			cmdPath = *p + "/" + cmd;
			return cmdPath;
		}
		p++;
	}
	return NULL;

}

// Adapted this method from stackoverflow:
// https://stackoverflow.com/questions/19099663/how-to-correctly-use-fork-exec-wait
void crash::forkAndExec(string path, string cmd) {
	pid_t pid = fork();
	if (pid == -1) {
		cout << "Fork failed" << endl;
		exit(1);
	}
	else if (pid > 0) {
    	int status;
    	waitpid(pid, &status, 0);
	}
	else {
		cout << "Trying " << path.c_str() << endl;
		execl(path.c_str(), cmd.c_str(), NULL);
	}

}

void crash::parseArgs(void) {
	vector<string> parsedArgs = split(args, ' ');
	if (strncmp(parsedArgs[0].c_str(), "/", 1) == 0) {
		forkAndExec(parsedArgs[0].c_str(), "");
	}
	else { 
		std::vector<string>::iterator cmd;
		for(cmd = parsedArgs.begin(); cmd != parsedArgs.end(); cmd++) {
			string cmdPath = findCmdPath(*cmd);
			forkAndExec(cmdPath, *cmd);
		}
	}
}