/**
 * @class TestResult
 *
 * La clase TestResult representa los resultados de la ejecuci�n de una prueba
 *
 * Existen dos tipos de errores: fallos y errores
 * Un fallo se produce por una condici�n falta en una aseci�n. Denota que no se pasa un
 * test debido a las condiciones impuestas por el programador del test-
 * Los errores son problemas o excepciones generadas por el programa, tales como
 * violaciones de acceso a memoria, acceso a fichero, etc.
 *
 * Tambi�n se proporcionan clases internas para permitir la sincronizaci�n de varios
 * hilos concurrentes.
 * Para usar esta sincronizaci�n, se debe sobrescribir el m�todo virtual
 * setSynchronizationObject(), y crear una instancia de la clase interna ExclusiveZone
 * al principio de cada uno de los m�todos.
 *
 * @author Modificado por JM - www.lawebdejm.com
 *
**/
#if !defined(_CPPUNIT_TESTRESULT_H_)
#define _CPPUNIT_TESTRESULT_H_


#include "Guards.h"
#include "TestFailure.h"


class CppUnitException;
class Test;


class TestResult
{
	REFERENCEOBJECT (TestResult)

public:
	TestResult();
	virtual ~TestResult();

	/// A�ade un error, causado por el par�metro "e"
	virtual void addError(Test *test, CppUnitException *e);
	/// A�ade un fallo, causado por el par�metro "e"
	virtual void addFailure(Test *test, CppUnitException *e);
	/// A�ade mensaje informativo, con los datos en el par�metro "e"
	virtual void addMessage(Test *test, CppUnitException *e);

	/// Informa que se comienza un test
	virtual void startTest(Test *test);
	/// Informa que me ha completado un test
	virtual void endTest(Test *test);

	/// Retorna el n�mero de test
	virtual int  runTests();

	/// Retorna el n�mero de errores
	virtual int  testErrors();
	/// Retorna el n�mero de fallos
	virtual int  testFailures();

	/// Retorna si un test se ha pasado o no
	virtual bool wasSuccessful();
	/// Retorna si un test puede pararse o no
	virtual bool shouldStop();
	/// Detiene las pruebas
	virtual void stop();

	/// Retorna la lista de errores
	virtual TestFailureList& errors();
	/// Retorna la lista de fallos
	virtual TestFailureList& failures();
	/// Retorna la lista de mensajes
	virtual TestFailureList& messages();


	//
	// Clase interna de sincronizaci�n
	//
	class SynchronizationObject
	{
	public:
		SynchronizationObject()
		{
		}
		virtual ~SynchronizationObject()
		{
		}

		virtual void lock()
		{
		}
		virtual void unlock()
		{
		}
	};

	class ExclusiveZone
	{
		SynchronizationObject   *m_syncObject;

	public:
		ExclusiveZone(SynchronizationObject *syncObject)
			: m_syncObject(syncObject)
		{
			m_syncObject->lock();
		}

		~ExclusiveZone ()
		{
			m_syncObject->unlock();
		}
	};

protected:
	/// M�todo para establecer un objeto de sincronizaci�n.
	/// Esta clase liberar� el objeto pasado cuando crea necesario
	virtual void setSynchronizationObject(SynchronizationObject *syncObject);

	TestFailureList  m_errors;
	TestFailureList  m_failures;
	TestFailureList	 m_messages;

	int              m_runTests;
	bool             m_stop;

	SynchronizationObject  *m_syncObject;
};


#endif
