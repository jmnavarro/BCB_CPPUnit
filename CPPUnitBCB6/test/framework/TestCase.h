/**
 * @class TestCase
 *
 * Define la clase TestCase, que representa un caso de prueba, pudiendo contener
 * distintos métodos de prueba.
 * Para definir un método de prueba se debe hacer lo siguiente:
 * 1.- implementar una clase derivada de TestCase
 * 2.- definir una variable de instancia para el objeto "fixture"
 * 3.- sobrescribir el método setUp para crear el objeto fixture
 * 4.- sobrescribir el método tearDown para liberar el objeto fixture
 *
 * Cada uno de los métodos de prueba del caso de prueba, utilizan un objeto fixture
 * recién creado así que el objeto no puede quedar en mal estado después de ejecutar
 * ciertas pruebas.
 *
 * @author Modificado por JM - www.lawebdejm.com
 *
**/
#if !defined(_CPPUNIT_TESTCASE_H_)
#define _CPPUNIT_TESTCASE_H_

#include <string>
#include <typeinfo>
#include <windows.h>

#include "Guards.h"
#include "Test.h"
#include "CppUnitException.h"


class TestResult;


class TestCase : public Test
{
	REFERENCEOBJECT(TestCase)

public:
	TestCase(std::string Name);
	~TestCase();

	virtual void run(TestResult *result);

	/// Método principal: ejecuta un test y captura las excepciones que pueda lanzar
	virtual TestResult* run();

	/// Retorna el total de las pruebas ejecutadas
	virtual int countTestCases();

	std::string name();
	std::string toString();

	/// inicialización del objeto <i>fixture</i>
	virtual void setUp();

	/// liberación del objeto <i>fixture</i>
	virtual void tearDown();

protected:
	virtual void runTest(TestResult* r);

	TestResult* defaultResult();

	/// Mensajes
	void showMessageImplementation(std::string message,
							  long        lineNumber = CPPUNIT_UNKNOWNLINENUMBER,
							  std::string fileName   = CPPUNIT_UNKNOWNFILENAME);


	/// Implementación de las aseciones
	void assertImplementation(bool        condition,
							  std::string conditionExpression = "",
							  long        lineNumber = CPPUNIT_UNKNOWNLINENUMBER,
							  std::string fileName   = CPPUNIT_UNKNOWNFILENAME);

	/// Aserción para números enteros
	void assertEqualsImplementation(
						long        expected,
						long        actual,
						long        lineNumber = CPPUNIT_UNKNOWNLINENUMBER,
						std::string fileName   = CPPUNIT_UNKNOWNFILENAME);

	/// Aserción para números decimales
	void assertEqualsImplementation(
						double      expected,
						double      actual,
						double      delta,
						long        lineNumber = CPPUNIT_UNKNOWNLINENUMBER,
						std::string fileName	 = CPPUNIT_UNKNOWNFILENAME);

	/// Aserción para cadenas STL
	void assertEqualsImplementation(
						std::string expected,
						std::string actual,
						long        lineNumber = CPPUNIT_UNKNOWNLINENUMBER,
						std::string fileName   = CPPUNIT_UNKNOWNFILENAME);

	/// Aserción para cadenas terminadas en nulo
	void assertEqualsImplementation(
						const char *expected,
						const char *actual,
						long        lineNumber = CPPUNIT_UNKNOWNLINENUMBER,
						std::string fileName   = CPPUNIT_UNKNOWNFILENAME);


	std::string notEqualsMessage(long  expected, long  actual);
	std::string notEqualsMessage(double  expected, double  actual);
	std::string notEqualsMessage(const char *expected, const char *actual);
	std::string notEqualsSizeMessage(const char *file1, const char *file2);
	std::string notEqualsFileMessage(const char *file1, const char *file2);
	std::string notExistsFileMessage(const char *fileName);
	std::string notSizeFileMessage(const char *fileName);

	void startTiming(bool pausado = false);
	void pauseTiming();
	void resumeTiming();
	void stopTimingImplementation(const char *fichero, int linea, const char *mensaje, DWORD *tiempoCalculado = NULL);

public:
	void setResult(TestResult *result)
	{
		m_result = result;
	}

private:
	typedef enum
	{
		timingStatusStoped,
		timingStatusStarted,
		timingStatusPaused
	} TimingStatus;


	const std::string   m_name;
	TestResult 			*m_result;

	DWORD inicioRendimiento;
	DWORD tiempoRendimiento;
	TimingStatus estadoRendimiento;
};


//
// Macros para obtener el número de línea y archivo fuente donde se produce un error
//
#undef assert
#define assert(condition) \
			this->assertImplementation ((condition),(#condition),__LINE__, __FILE__)


#define assertMessage(condition, message) \
			this->assertImplementation	(condition, message, __LINE__, __FILE__)


#define assertDoublesEqual(expected, actual, delta) \
			this->assertEqualsImplementation(expected, actual, delta, __LINE__, __FILE__)

#define assertLongsEqual(expected, actual) \
			this->assertEqualsImplementation(expected, actual, __LINE__, __FILE__)

#define assertEquals(expected, actual) \
			this->assertEqualsImplementation(expected, actual, __LINE__, __FILE__)


#define showMessage(message) \
			this->showMessageImplementation(message, __LINE__, __FILE__)


#define stopTiming(mensaje) \
	this->stopTimingImplementation(__FILE__, __LINE__, mensaje)

#define stopTimingWithValue(mensaje, numero) \
	this->stopTimingImplementation(__FILE__, __LINE__, mensaje, &numero)


#endif
