#include "TestCase.h"
#include "TestResult.h"
#include "estring.h"

#include <stdexcept>
#include <cmath>


TestCase::TestCase(std::string name)
	: m_name(name)
{
	m_result = NULL;
	estadoRendimiento = timingStatusStoped;
	tiempoRendimiento = 0;
}


TestCase::~TestCase()
{
}



int TestCase::countTestCases()
{
	return 1;
}


std::string TestCase::name()
{
	return m_name;
}


void TestCase::setUp()
{
}

void TestCase::tearDown()
{
}


std::string TestCase::toString()
{
	return (std::string(typeid(*this).name()) + "." + name());
}


TestResult* TestCase::defaultResult()
{
	return new TestResult;
}


void TestCase::showMessageImplementation(
							std::string message,
							long        lineNumber,
							std::string fileName)

{
	if (m_result != NULL)
	{
		CppUnitException	*e;

		e = new CppUnitException(message, lineNumber, fileName);

		m_result->addMessage(this, e);
	}
}


void TestCase::assertImplementation(
							bool        condition,
							std::string conditionExpression,
							long        lineNumber,
							std::string fileName)
{
	if (!condition)
		throw CppUnitException(conditionExpression, lineNumber, fileName);
}


void TestCase::assertEqualsImplementation(
							long        expected,
							long        actual,
							long        lineNumber,
							std::string fileName)
{
	if (expected != actual)
		assertImplementation(false, notEqualsMessage(expected, actual), lineNumber, fileName);
}


void TestCase::assertEqualsImplementation(
							double      expected,
							double      actual,
							double      delta,
							long        lineNumber,
							std::string fileName)
{
	if (std::fabs(expected - actual) > delta)
		assertImplementation(false, notEqualsMessage(expected, actual), lineNumber, fileName);

}


void TestCase::assertEqualsImplementation(
							std::string expected,
							std::string actual,
							long        lineNumber,
							std::string fileName)
{
	assertEqualsImplementation(expected.c_str(), actual.c_str(), lineNumber, fileName);
}


void TestCase::assertEqualsImplementation(
							const char *expected,
							const char *actual,
							long        lineNumber,
							std::string fileName)
{
	if (0 != strcmp(expected, actual))
		assertImplementation(false, notEqualsMessage(expected, actual), lineNumber, fileName);
}


void TestCase::run (TestResult *result)
{
	m_result = result;

	result->startTest(this);

	// código de inicialización: lo sobrescribe el descendiente para crear el fixture
	setUp();

	try
	{
		runTest(result);
	}
	catch (CppUnitException e)
	{
		// si es una excepción de CPPUnit, entonces se trata de un fallo
		CppUnitException *copy = new CppUnitException(e);
		result->addFailure(this, copy);
	}
	catch (std::exception e)
	{
		// si es una excepción de STL, se considera error, pero con mensaje
		result->addError(this, new CppUnitException(e.what()));
	}
	catch (...)
	{
		// cualquier otro tipo de excepción, se considera con error
		CppUnitException *e = new CppUnitException("excepción desconocida");
		result->addError(this, e);
	}

	// liberar recursos reservados durante "setUp"
	tearDown();

	result->endTest(this);
	m_result = NULL;
}


TestResult* TestCase::run()
{
	TestResult *result = defaultResult();

	run(result);

	return result;
}


void TestCase::runTest(TestResult* r)
{
}


std::string TestCase::notEqualsMessage(long expected, long actual)
{
	return "esperado: " + estring(expected) + " pero obtenido: " + estring(actual);
}

std::string TestCase::notEqualsMessage(double expected, double actual)
{
	return "esperado: " + estring(expected) + " pero obtenido: " + estring(actual);
}

std::string TestCase::notEqualsMessage(const char *expected, const char *actual)
{
	return "esperado: \"" + estring(expected) + "\" pero obtenido: \"" + estring(actual) + "\"";
}

std::string TestCase::notEqualsSizeMessage(const char *file1, const char *file2)
{
	return "El tamaño de los archivos \"" + estring(file1) + "\" y \"" + estring(file2) + "\" es distinto";
}

std::string TestCase::notEqualsFileMessage(const char *file1, const char *file2)
{
	return "Los archivos \"" + estring(file1) + "\" y \"" + estring(file2) + "\" son distintos";
}

std::string TestCase::notExistsFileMessage(const char *fileName)
{
	return "Error abriendo el archivo \"" + estring(fileName) + "\" ¿existe?";
}

std::string TestCase::notSizeFileMessage(const char *fileName)
{
	return "Error obteniendo tamaño del archivo" + estring(fileName) + "\" ¿existe?";
}


void TestCase::startTiming(bool pausado)
{
	if (estadoRendimiento == timingStatusStoped)
	{
		tiempoRendimiento = 0;
		if (pausado)
			estadoRendimiento = timingStatusPaused;
		else
		{
			estadoRendimiento = timingStatusStarted;
			inicioRendimiento = GetTickCount();
		}
	}
}

void TestCase::pauseTiming()
{
	if (estadoRendimiento == timingStatusStarted)
	{
		tiempoRendimiento += GetTickCount() - inicioRendimiento;
		estadoRendimiento = timingStatusPaused;
	}
}

void TestCase::resumeTiming()
{
	if (estadoRendimiento == timingStatusPaused)
	{
		inicioRendimiento = GetTickCount();
		estadoRendimiento = timingStatusStarted;
	}
}

void TestCase::stopTimingImplementation(const char *fichero, int linea, const char *mensaje, DWORD *tiempoCalculado)
{
	DWORD horas, minutos, segundos, milisegundos;
	char buffer[1024];

	if (estadoRendimiento == timingStatusStarted || estadoRendimiento == timingStatusPaused)
	{
		if (estadoRendimiento == timingStatusStarted)
			tiempoRendimiento += GetTickCount() - inicioRendimiento;
			
		if (tiempoCalculado != NULL)
			(* tiempoCalculado) = tiempoRendimiento;
		milisegundos = tiempoRendimiento % 1000;
		tiempoRendimiento /= 1000;
		segundos = tiempoRendimiento % 60;
		tiempoRendimiento /= 60;
		minutos = tiempoRendimiento % 60;
		tiempoRendimiento /= 60;
		horas = tiempoRendimiento;

		tiempoRendimiento = 0;

		sprintf (buffer,
					"%s%s%u:%02u:%02u,%03u",
					(mensaje == NULL) ? "" : mensaje,
					(mensaje == NULL) ? "" : ": ",
					horas,
					minutos,
					segundos,
					milisegundos);
		showMessageImplementation (buffer, linea, fichero);

		estadoRendimiento = timingStatusStoped;
	}
}


