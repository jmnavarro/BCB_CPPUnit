/**
 * @class TestCaller
 *
 * Se define la clase TestCaller, que permite la ejecución de pruebas, ya sean 
 * de individuales (TestCase) o en conjunto (TestSuites)
 *
**/
#ifndef CPPUNIT_TESTCALLER_H
#define CPPUNIT_TESTCALLER_H


#include "Guards.h"
#include "TestCase.h"
#include "TestResult.h"


template <class Fixture> class TestCaller : public TestCase
{
   REFERENCEOBJECT (TestCaller)

   typedef void (Fixture::*TestMethod)();

public:
	TestCaller(std::string name, TestMethod test)
		: TestCase (name)
		, m_fixture(new Fixture(name))
		, m_test(test)
	{
	}

protected:
	void runTest(TestResult* r)
	{
		m_fixture.get()->setResult(r);
		(m_fixture.get()->*m_test)();
		m_fixture.get()->setResult(NULL);
	}

	void setUp()
	{
		m_fixture.get()->setUp();
	}

	void tearDown()
	{
		m_fixture.get()->tearDown();
	}

private:
   TestMethod               m_test;
   std::auto_ptr<Fixture>   m_fixture;

};


#endif