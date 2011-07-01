#include "TestResult.h"


TestResult::TestResult()
{
	m_runTests = 0;
	m_stop = false;
	m_syncObject = new SynchronizationObject();
}

TestResult::~TestResult ()
{
	TestFailureListIterator it;

	for (it = m_errors.begin(); it != m_errors.end(); ++it) {
		delete *it;
	}

	for (it = m_failures.begin(); it != m_failures.end(); ++it) {
		delete *it;
	}

	for (it = m_messages.begin(); it != m_messages.end(); ++it) {
		delete *it;
	}


	delete m_syncObject;
}


void TestResult::addError(Test *test, CppUnitException *e)
{
	ExclusiveZone zone(m_syncObject);

	m_errors.push_back(new TestFailure(test, e));
}


void TestResult::addFailure(Test *test, CppUnitException *e)
{
	ExclusiveZone zone(m_syncObject);

	m_failures.push_back(new TestFailure (test, e));
}

void TestResult::addMessage(Test *test, CppUnitException *e)
{
	ExclusiveZone zone(m_syncObject);

	m_messages.push_back(new TestFailure(test, e));
}


void TestResult::startTest(Test *test)
{
	ExclusiveZone zone(m_syncObject);

	m_runTests++;
}


void TestResult::endTest (Test *test)
{
	ExclusiveZone zone(m_syncObject);
}


int TestResult::runTests()
{
	ExclusiveZone zone(m_syncObject);

	return m_runTests;
}


int TestResult::testErrors()
{
	ExclusiveZone zone(m_syncObject);

	return m_errors.size();
}


int TestResult::testFailures()
{
	ExclusiveZone zone(m_syncObject);

	return m_failures.size();
}


bool TestResult::wasSuccessful()
{
	ExclusiveZone zone(m_syncObject);

	return m_failures.size() == 0 && m_errors.size() == 0;
}


TestFailureList& TestResult::errors()
{
	ExclusiveZone zone(m_syncObject);

	return m_errors;
}


TestFailureList& TestResult::failures()
{
	ExclusiveZone zone(m_syncObject);

	return m_failures;
}


TestFailureList& TestResult::messages()
{
	ExclusiveZone zone(m_syncObject);

	return m_messages;
}

bool TestResult::shouldStop()
{
	ExclusiveZone zone(m_syncObject);

	return m_stop;
}


void TestResult::stop()
{
	ExclusiveZone zone(m_syncObject);

	m_stop = true;
}


void TestResult::setSynchronizationObject(SynchronizationObject *syncObject)
{
	delete m_syncObject;
	m_syncObject = syncObject;
}


