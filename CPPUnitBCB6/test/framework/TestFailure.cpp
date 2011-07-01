#include "TestFailure.h"
#include "Test.h"


TestFailure::TestFailure(Test *failedTest, CppUnitException *thrownException)
{
	m_failedTest = failedTest;
	m_thrownException = thrownException;
}


TestFailure::~TestFailure()
{
	delete m_thrownException;
}


Test *TestFailure::failedTest()
{
	return m_failedTest;
}


CppUnitException *TestFailure::thrownException()
{
	return m_thrownException;
}


std::string TestFailure::toString()
{
	return (m_failedTest->toString() + ": " + m_thrownException->what());
}

