// tests.cpp
#include "crash.cpp"
#include <gtest/gtest.h>
#include <vector>
#include <string>

#define ENV_PATH "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin"

extern char ** environ;

namespace {
	class Test_Crash : public ::testing::Test {

	protected:
		vector<string> args;
		crash * cr;

		Test_Crash() {
		}

		virtual ~Test_Crash() {
		}

		virtual void SetUp() {
			args = {"ls", "-l"};
			cr = new crash(args, environ);
		}

		virtual void TearDown() {
		}
	};

	TEST_F(Test_Crash, BuildCrashObject) {
		ASSERT_EQ(cr->args[0], "ls");
		ASSERT_EQ(cr->args[1], "-l");
	}

	TEST_F(Test_Crash, FindPathEnvironmentVariable) {
		cr->findPATH();
		ASSERT_EQ(cr->path, ENV_PATH);
	}

	TEST_F(Test_Crash, SplitEnvironmentPath) {
		cr->findPATH();

		vector<string> paths = cr->splitPaths();
		ASSERT_EQ(paths[0], "/usr/local/sbin");
		ASSERT_EQ(paths[5], "/bin");
	}

	TEST_F(Test_Crash, ForkAndExecls-l) {
		
	}

} // Namespace
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}