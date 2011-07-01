#include "TestRunnerUnitForm.h"
#include "GUITestResult.h"


GUITestResult::GUITestResult(TTestRunnerForm *runner)
{
	m_runner = runner;
	setSynchronizationObject(new LightweightSynchronizationObject ());
}


GUITestResult::~GUITestResult()
{
}


void GUITestResult::stop()
{
	m_stop = true;
}


void GUITestResult::addError(Test *test, CppUnitException *e)
{
	ExclusiveZone zone(m_syncObject);

	TestResult::addError(test, e);
	m_runner->addError(this, test, e);
}


void GUITestResult::addFailure(Test *test, CppUnitException *e)
{
	ExclusiveZone zone(m_syncObject);

	TestResult::addFailure(test, e);
	m_runner->addFailure(this, test, e);
}

void GUITestResult::addMessage(Test *test, CppUnitException *e)
{
	ExclusiveZone zone(m_syncObject);

	TestResult::addMessage(test, e);
	m_runner->addMessage(this, test, e);
}


void GUITestResult::endTest(Test *test)
{
	ExclusiveZone zone(m_syncObject);

	TestResult::endTest(test);
	m_runner->endTest(this, test);
}

