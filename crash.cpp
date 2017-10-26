// crash.cpp

#include <iostream>
#include <vector>
#include <string.h>
#include <sstream>
#include <string>
#include "crash.h"

using namespace std;

crash::crash(vector<string> args, char ** environVars) {
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
//Adapted this method from stackoverflow.com/questions/53849/how-do-i-tokenize-a-string-in-c
vector<string> crash::splitPaths(void) {
	stringstream ss(path);
	string str;
	vector<string> paths;

	while (getline(ss, str, ':')) {
		paths.push_back(str);
	}
	return paths;
}

void crash::printArgs(void) { 
	vector<string>::iterator arg;
	for(arg = args.begin(); arg != args.end(); arg++) {
		cout << *arg << endl;
	}
}