#ifndef GUITestResultH
#define GUITestResultH

#include "TestResult.h"


class TTestRunnerForm;


class GUITestResult : public TestResult
{
public:
	GUITestResult(TTestRunnerForm *runner);
	~GUITestResult();

	void addError(Test *test, CppUnitException *e);
	void addFailure(Test *test, CppUnitException *e);
	void addMessage(Test *test, CppUnitException *e);

	void endTest(Test *test);
	void stop();

protected:
	class LightweightSynchronizationObject : public TestResult::SynchronizationObject
	{
		// CCriticalSection    m_syncObject;

	public:
		void lock ()
		{
			//m_syncObject.Lock ();
		}
		void unlock()
		{
			//m_syncObject.Unlock ();
		}
	};

private:
	TTestRunnerForm  *m_runner;
};


#endif
