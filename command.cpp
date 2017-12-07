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
#include <glob.h>

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

// Removes all args with "*"
// Adapted from cplusplus:
// http://www.cplusplus.com/forum/beginner/99524/
void command::cleanArgs() {
	vector<string>::iterator iter = args.begin();
	while (iter != args.end()) {
		if ((*iter).find("*") != -1) {
			iter = args.erase(iter);
		}
		else {
			iter++;
		}
	}
}
// Adapted from stackoverflow:
// https://stackoverflow.com/questions/8401777/simple-glob-in-c-on-unix-system
void command::globExpand() {
	vector<string> expandedArgs;
	// Iterate through each argument that needs to be expanded
	for (auto argsIterator = args.begin(); argsIterator != args.end(); ++argsIterator) {
		glob_t glob_result;
		string arg = *argsIterator;
		glob(arg.c_str(), GLOB_TILDE, NULL, &glob_result);
		// Iterate through every result that glob returns for that argument
		for(unsigned int i = 0; i < glob_result.gl_pathc; ++i){
			expandedArgs.push_back(string(glob_result.gl_pathv[i]));
		}
		globfree(&glob_result);
	}
	cleanArgs();
	args.insert(args.end(), expandedArgs.begin(), expandedArgs.end());
}

string command::findCmdPath() {
	if (cmd.find("/") == 0) {
		//execute the command immediately
		cmdPath = cmd;
		return cmdPath;

	}
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
	cmdPath = findCmdPath();
	error("Could not find command", cmdPath == "");
	globExpand();
	args.insert(args.begin(), cmdPath); //prepend the command name to the arg list so execv works
	char ** cmdArgs = vectorToCharArray(args);
	error("Exec failed", execv(cmdPath.c_str(), cmdArgs) == -1);
}

// Adapted this method from stackoverflow:
// https://stackoverflow.com/questions/19099663/how-to-correctly-use-fork-exec-wait
void command::forkAndExec() {
	if (cmd == "cd") {
		error("Change Directory failed", chdir(args[0].c_str()) < 0);
	}
	else {
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

}