#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h>
using namespace CppUnit;
#include <iostream>
using namespace std;
#include "headers.hpp"
#include "class_functionstest.hpp"
//test case1 definition
void functions_TestCase::testshow_current_directoy_client()
{	
	Show_Current_Directory c;
	
	CPPUNIT_ASSERT("pwd "==c.show_current_directoy_client());
}
//test case2 definition
void functions_TestCase::testlist_files_and_folder_server()
{	
	List_Files_And_Folders b;
	
	CPPUNIT_ASSERT(true==b.list_files_and_folder_server());
}

//creating suite of tests for various functionalities
Test* functions_TestCase::suite()
{
	TestSuite* testsuite = new TestSuite("functions_TestCase");//class 	
	testsuite->addTest(new TestCaller<functions_TestCase>("testshow_current_directoy_client",&functions_TestCase::testshow_current_directoy_client));
	testsuite->addTest(new TestCaller<functions_TestCase>("testlist_files_and_folder_server",&functions_TestCase::testlist_files_and_folder_server));
	

	return testsuite;
}
int main(int argc, char *argv[])
{
	if (argc!=2)
	{
		cout<<"usage error " <<endl;
		cout<< "tester <name of the class to be tested>" <<endl;
		exit(1);
	}
	
	CppUnit::TextUi::TestRunner testrunner;
	testrunner.addTest(functions_TestCase::suite());
	testrunner.run();
}

