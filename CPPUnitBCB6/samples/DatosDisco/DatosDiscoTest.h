//---------------------------------------------------------------------------
#ifndef DatosDiscoTestH
#define DatosDiscoTestH

#include "TestCase.h"
#include "TestCaller.h"
#include "DatosDisco.h"
//---------------------------------------------------------------------------

class DatosDiscoTest : public TestCase
{
public:
	DatosDiscoTest(std::string name);
	virtual ~DatosDiscoTest();

	void setUp();
	void tearDown();

	static Test* suite();

protected:
	void testLeer();
	void testGuardar();
    void testDatoNulo();

private:
	DatosDisco	*obj;
};

typedef TestCaller<DatosDiscoTest> DatosDiscoTestCaller;

#endif
