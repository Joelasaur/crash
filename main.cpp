#include "crash.h"
#include "command.h"
#include <iostream>
#include <string>
#include <readline/readline.h>
#include <readline/history.h>
#include <cstdio>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>	
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <map>

using namespace std;

#define DEFAULT_SHELL ".myshell"

extern char ** environ;

bool fileExists (const string& name) {
    return ( access( name.c_str(), F_OK ) != -1 );
}

// Adapted from stackoverflow:
// https://stackoverflow.com/questions/7868936/read-file-line-by-line
void addCmdsFromFile(string filename, string prompt, map<string, string> * aliases) {
	ifstream infile(filename);
	string line;
	vector<string> commandArgs;
	while (getline(infile, line)) {
		commandArgs = command::split(line, ' ');
		crash * cr = new crash(line, prompt, environ, aliases);
		cr->parseArgs(commandArgs);
	}
}

void run(struct sigaction sigIntHandler) {
	char * buf;
	string prompt = "Enter a command: > ";	
	map<string, string> * myAliasMap = new map<string, string>;

	if (fileExists(DEFAULT_SHELL)) {
		addCmdsFromFile(DEFAULT_SHELL, prompt, myAliasMap);
	}
	while((buf = readline(prompt.c_str())) != NULL) {
		sigaction(SIGINT, &sigIntHandler, NULL);
		string s(buf);
		crash * cr = new crash(buf, prompt, environ, myAliasMap);
		if (buf[0] != 0) {
			add_history(buf);
		}
		vector<string> splitArgs = command::split(buf, ' ');

		//We are executing commands from a file
		int executableFilePosition = splitArgs[0].find("./");
		if (executableFilePosition != -1) {
			addCmdsFromFile(splitArgs[0].substr(executableFilePosition), prompt, myAliasMap);
		}
		else {
			cr->parseArgs(splitArgs);
		}
		myAliasMap = cr->aliasMap;
		prompt = cr->prompt;
	}
}

void handleSignal(int signal) {
	cout << "\nHEY! You interrupted me with this signal: " << signal << endl;
}

// Signal handling adapted from stackoverflow:
// https://stackoverflow.com/questions/1641182/how-can-i-catch-a-ctrl-c-event-c
// https://stackoverflow.com/questions/17766550/ctrl-c-interrupt-event-handling-in-linux
int main (int argc, char * argv[]){
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = handleSignal;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);
	run(sigIntHandler);
}	