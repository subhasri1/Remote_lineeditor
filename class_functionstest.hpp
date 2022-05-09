#include <cppunit/TestCase.h>
using namespace CppUnit;
class functions_TestCase: public TestCase
{
public:
	void testshow_current_directoy_client(); //is a test case1
	void testlist_files_and_folder_server(); //is a test case2
	static Test* suite();
};

