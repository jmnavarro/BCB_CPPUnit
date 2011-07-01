#ifndef TestRunnerH
#define TestRunnerH

#include "Test.h"


class TestRunner
{
public:
	TestRunner();
	virtual ~TestRunner();

	void run();
	void addTest(Test*);

private:
	TestList  m_tests;
};

#endif
