#include "TestSuite.h"
#include "TestResult.h"


TestSuite::TestSuite(std::string name)
	: m_name(name)
{
}

TestSuite::~TestSuite()
{
	deleteContents();
}


void TestSuite::addTest(Test *test)
{
	m_tests.push_back(test);
}


std::string TestSuite::toString()
{
	return ("suite " + m_name);
}


void TestSuite::deleteContents()
{
	for (TestListIterator it = m_tests.begin(); it != m_tests.end(); ++it) {
		delete *it;
	}
}


TestList& TestSuite::Tests()
{
	return m_tests;
}


void TestSuite::run(TestResult *result)
{
	for (TestListIterator it = m_tests.begin(); it != m_tests.end(); ++it)
	{
		if (result->shouldStop())
			break;

		(*it)->run(result);
	}
}


int TestSuite::countTestCases()
{
	int total = 0;

	for (TestListIterator it = m_tests.begin(); it != m_tests.end(); ++it) {
		total += (*it)->countTestCases();
	}

	return total;
}

