#include "crash.cpp"
#include "command.cpp"
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <map>

#define ENV_PATH "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin"

extern char ** environ;

namespace {
	class Test_Crash : public ::testing::Test {

	protected:
		string input;
		string prompt;
		crash * cr;
		command * cmd;
		vector<string> args;
		map<string, string> * myAliasMap;

		Test_Crash() {
		}

		virtual ~Test_Crash() {
		}

		virtual void SetUp() {
			input = "ls -l -a";
			prompt = "Enter a command >";
			myAliasMap = new map<string, string>;
			cr = new crash(input, prompt, environ, myAliasMap);
			args = {"-l", "-a"};
			cmd = new command("ls", args, ENV_PATH);
		}

		virtual void TearDown() {
		}
	};

	TEST_F(Test_Crash, SplitInput) {
		vector<string> splitInput = command::split(input, ' ');
		ASSERT_EQ(splitInput[0], "ls");
		ASSERT_EQ(splitInput[1], "-l");
		ASSERT_EQ(splitInput[2], "-a");
	}

	TEST_F(Test_Crash, FindPathEnvironmentVariable) {
		cr->findPATH();
		ASSERT_EQ(cr->path, ENV_PATH);
	}

	TEST_F(Test_Crash, SplitEnvironmentPath) {
		cr->findPATH();

		vector<string> paths = command::split(cr->path, ':');
		ASSERT_EQ(paths[0], "/usr/local/sbin");
		ASSERT_EQ(paths[5], "/bin");
	}

	TEST_F(Test_Crash, cmdExists) {
		ASSERT_TRUE(cmd->cmdExists("/bin/"));
	}

	TEST_F(Test_Crash, findCmdPath) {
		cr->findPATH();
		string expected = "/bin/ls";
		cmd->findCmdPath();
		string result = cmd->cmdPath;
		ASSERT_EQ(expected, result);
	}

	TEST_F(Test_Crash, badCommandCleanExit) {
		args.clear();
		command * cmd_bad = new command("lssss", args, ENV_PATH);
		string expected = "";
		string result = cmd_bad->findCmdPath();
		ASSERT_EQ(expected, result);
	}

	TEST_F(Test_Crash, splitCanHandleMissingTokens) {
		vector<string> splitInput = command::split(input, '|');
		ASSERT_EQ(splitInput[0], "ls -l -a");

		string input2 = "ls -l -a | wc -w";
		splitInput = command::split(input2, '|');
		ASSERT_EQ(splitInput[0], "ls -l -a ");
		ASSERT_EQ(splitInput[1], " wc -w");
	}

	TEST_F(Test_Crash, cleanArgs) {
		args = {"$HOME", "tmp/*.txt", "tmp/nope.*"};
		command * cmd_cleanArgs = new command("echo", args, ENV_PATH);
		cmd_cleanArgs->cleanArgs();
		int expected = 1;
		int result = cmd_cleanArgs->args.size();
		ASSERT_EQ(expected, result);

	}

	TEST_F(Test_Crash, globExpand) {
		args = {"tmp/*.txt", "tmp/nope.*"};
		string expected1 = "tmp/nope.txt";
		string expected2 = "tmp/test.txt";
		command * cmd_glob = new command("touch", args, ENV_PATH);
		cmd_glob->globExpand();
		string result1 = cmd_glob->args[1];
		string result2 = cmd_glob->args[2];
		ASSERT_EQ(expected1, result1);
		ASSERT_EQ(expected2, result2);
	}

	TEST_F(Test_Crash, createAlias) {
		ASSERT_EQ(0, cr->aliasMap->size());
		string ret = cr->createAlias("dir=ls");
		string ret1 = cr->createAlias("ff=ls");
		ASSERT_EQ("dir", ret);
		ASSERT_EQ("ff", ret1);
		ASSERT_EQ(2, cr->aliasMap->size());
		string expected = "ls";
		string result = cr->aliasMap->at("dir");
		ASSERT_EQ(expected, result);
		ASSERT_EQ("ls", cr->aliasMap->at("ff"));
	}
	TEST_F(Test_Crash, getAlias) {
		string ret = cr->createAlias("ff=ls");
		string expected = "ls";
		string result = cr->getAlias("ff");
		ASSERT_EQ(expected, result);

	}

} // Namespace
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}