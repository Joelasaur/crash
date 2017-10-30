#include "crash.h"
#include <iostream>
#include <string>

using namespace std;

extern char ** environ;

int main (int argc, char * argv[]){
	string rawArgs;

	cout << "Welcome to crash!! Please enter a command:" << endl;
	getline(cin, rawArgs);

	crash * cr = new crash(rawArgs, environ);
	cr->findPATH();

	cr->parseArgs();
}