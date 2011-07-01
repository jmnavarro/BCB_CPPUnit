/**
 * @class TestSuite
 *
 * Un TestSuite es un conjunto de pruebas (objetos descencientes de Test.)
 * Estas pruebas pueden ser a su vez otros TestSuites o bien TestCases, ya que ambas
 * clases heredan de Test.
 * Cuando se ejecuta el TestSuite, se ejecutan en orden cada uno de las pruebas que
 * contiene. Por ejemplo:
 * @code
 * 		TestSuite *operaciones= new TestSuite();
 * 		suite->addTest(new TestCaller<SumaTest> ("testSumar", testSumar));
 * 		suite->addTest(new TestCaller<RestaTest> ("testRestar", testRestar));
 * @endcode
 *
 * Los objetos Test que se almacenan dentro de un TestSuite, se liberan cuando el
 * TestSuite se destruya.
 *
 * @author Modificado por JM - www.lawebdejm.com
 *
**/
#if !defined(_CPPUNIT_TESTSUITE_H_)
#define _CPPUNIT_TESTSUITE_H_


#include <string>

#include "Guards.h"
#include "Test.h"


class TestResult;


class TestSuite : public Test
{
	REFERENCEOBJECT(TestSuite)
public:
	TestSuite(std::string name = "");
	virtual ~TestSuite();

	/// Ejecuta todos las pruebas dentro del TestSuite
	void run(TestResult *result);

	/// Cuenta el número de TestCases que se ejecutarán.
	/// Hay que tener en cuenta que esas pruebas pueden ser a su vez de tipo TestSuite, por
	/// lo que habrá que contar el número de pruebas que contenga el suite
	int  countTestCases();
	void addTest(Test *test);

	/// Retorna una cadena que representa al conjunto de pruebas
	std::string toString();

	/// Libera todos las pruebas dentro de este TestSuite
	virtual void deleteContents();
	virtual TestList& Tests();

private:
	TestList		   m_tests;
	const std::string  m_name;
};


#endif
