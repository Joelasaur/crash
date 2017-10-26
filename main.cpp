#include "crash.h"
#include <iostream>

using namespace std;

extern char * environ[];

int main (int argc, char * argv[]){
	vector<string> args(argv, argv + argc); //Convert argv into a vector for easier manipulation
	crash * cr = new crash(args, environ);
	cr->printArgs();
	cr->findPATH();
	cout << "PATH: " << cr->path << endl;
}