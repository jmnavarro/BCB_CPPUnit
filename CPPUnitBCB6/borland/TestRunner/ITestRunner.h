#ifndef ITestRunnerH
#define ITestRunnerH

#ifdef TESTRUNNER_BUILDING
	#define CPPUNIT_API __declspec(dllexport)
#else
	#define CPPUNIT_API __declspec(dllimport)
#endif


class Test;
class TestRunner;


class CPPUNIT_API ITestRunner
{
	TestRunner* m_impl;

public:
	ITestRunner();
	virtual ~ITestRunner();
	virtual void run();
	virtual void addTest(Test*);
};


#endif
