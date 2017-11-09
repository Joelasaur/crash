#include "crash.h"
#include <iostream>
#include <string>
#include <readline/readline.h>
#include <readline/history.h>
#include <cstdio>

using namespace std;

extern char ** environ;

int main (int argc, char * argv[]){
	char * buf;
	while((buf = readline("Enter a command: > ")) != NULL) {
		string s(buf);
		crash * cr = new crash(buf, environ);

		cr->parseArgs();
	}
}	