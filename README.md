# crash
crash is a unix shell in c++ I'm writing for my Operating Systems class.

For debugging with gdb, make sure to pass the argument "-DCMAKE_BUILD_TYPE=Debug" into cmake.

To build and run the unit tests, follow the instructions located here to setup the googletest framework: https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/

Use the command "set follow-fork-mode child" within gdb to debug the child process from the forkAndExec call.

POINTS:

The Shell Assignment (total 18 points)

	1 Can run an executable
		/bin/ls

	1 You search the path for the executable
		ls

	1 Can set enviornment variables with using putenv/setenv
		PATH=:/bin:/sbin:/usr/sbin:/usr/bin

	2 Expands enviornment variables on the command line
		ls $HOME

	2 Does filename expansion "glob" (Hint:  Use the built in glob.)
		ls a*b

	1 Knows how to change directory
		cd /fred

	1 Control-L clears the screen
		ctrl-l = clear screen

	2 Tab Completion and Arrow History

	1 Catch Keyboard interrupt
		ctrl + c = back to prompt

	2 Can run commands from a file
		./tmp/commands.txt

	2 Automatically runs a file called .myshell when it starts
	
	2 Change Prompt
	        PS1="what is you command?"


