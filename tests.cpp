#include "crash.cpp"
#include "command.cpp"
#include <gtest/gtest.h>
#include <vector>
#include <string>

#define ENV_PATH "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin"

extern char ** environ;

namespace {
	class Test_Crash : public ::testing::Test {

	protected:
		string input;
		crash * cr;
		command * cmd;
		vector<string> args;

		Test_Crash() {
		}

		virtual ~Test_Crash() {
		}

		virtual void SetUp() {
			input = "ls -l -a";
			cr = new crash(input, environ);
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

} // Namespace
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}