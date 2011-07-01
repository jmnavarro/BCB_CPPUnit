/**
 * @class Test
 *
 * Una prueba (Test) puede ejecutarse y obtener los resultados de la ejecución
 * Es una clase abstracta de la que descenderán todas las clases para implementar
 * pruebas, entre ellas las colecciones de pruebas (TestSuite) y los casos de prueba
 * (TestCase)
 *
**/
#ifndef CPPUNIT_TEST_H
#define CPPUNIT_TEST_H

#include <pshpack8.h>
	#include <string>
	#include <vector>
#include <poppack.h>


class TestResult;

class Test
{
public:
	virtual ~Test()
	{
	}

	virtual void run(TestResult *result) = 0;
	virtual int countTestCases() = 0;
	virtual std::string toString() = 0;
};


typedef std::vector<Test*>			  TestList;
typedef std::vector<Test*>::iterator  TestListIterator;

#endif

