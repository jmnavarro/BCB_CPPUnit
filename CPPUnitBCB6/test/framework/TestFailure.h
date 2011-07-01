/**
 * @class TestFailure
 *
 * Define la clase TestFailure que contiene el un test que ha fallado junto con la
 * excepción que produjo el fallo.
 *
 * @author Modificado por JM - www.lawebdejm.com
 *
**/
#if !defined(_CPPUNIT_TESTFAILURE_H_)
#define _CPPUNIT_TESTFAILURE_H_


#include "Guards.h"
#include "CppUnitException.h"

#include <vector>


class Test;


class TestFailure
{
	REFERENCEOBJECT(TestFailure)

public:
	TestFailure(Test *failedTest, CppUnitException *thrownException);
	~TestFailure();

	Test*  failedTest();
	CppUnitException*  thrownException();
	std::string  toString();

protected:
	Test             *m_failedTest;
	CppUnitException *m_thrownException;
};


typedef	std::vector<TestFailure *>			  TestFailureList;
typedef	std::vector<TestFailure *>::iterator  TestFailureListIterator;


#endif
