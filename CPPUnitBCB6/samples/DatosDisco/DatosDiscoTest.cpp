//---------------------------------------------------------------------------
#include <vcl.h>

#include "DatosDiscoTest.h"
#include "TestSuite.h"
//---------------------------------------------------------------------------

DatosDiscoTest::DatosDiscoTest(std::string name)
	: TestCase(name)
{
	obj = NULL;
}

DatosDiscoTest::~DatosDiscoTest()
{
}

//---------------------------------------------------------------------------

void DatosDiscoTest::setUp()
{
	if (!obj)
		obj = new DatosDisco();
}


void DatosDiscoTest::tearDown()
{
	if (obj) {
		delete obj;
	    obj = NULL;
    }
}


Test* DatosDiscoTest::suite()
{
	TestSuite *suite = new TestSuite("Pruebas con la clase DatosDisco");

	suite->addTest( new DatosDiscoTestCaller("testLeer",	 &DatosDiscoTest::testLeer) );
	suite->addTest( new DatosDiscoTestCaller("testGuardar",  &DatosDiscoTest::testGuardar) );
	suite->addTest( new DatosDiscoTestCaller("testDatoNulo", &DatosDiscoTest::testDatoNulo) );

	return suite;
}

//---------------------------------------------------------------------------

//
// Los datos que sabemos que están guardados en el fichero patrón.
//
#define FICHERO_PATRON	"patron.dat"
#define NUMERO_PATRON	19
#define CADENA_PATRON	"este es el texto de prueba del archivo de patrón"

#define FICHERO_TMP		"copia.tmp"

void DatosDiscoTest::testLeer()
{
	// se ejecuta la acción...
	assert( obj->leer(FICHERO_PATRON) );

	// ...y se comprueban los resultados con aserciones
    assertEquals(NUMERO_PATRON, obj->getDato()->numero);
    assert( 0 == strcmp(CADENA_PATRON, obj->getDato()->cadena) );
}

void DatosDiscoTest::testGuardar()
{
	DATOS	d;
    LPDATOS	dTmp, dPatron;
	DWORD  	sizeTMP, sizePatron, leidoTMP, leidoPatron;
	HANDLE 	fichTMP, fichPatron;
    char 	*buffTMP, *buffPatron;

    d.numero = NUMERO_PATRON;
    strcpy(d.cadena, CADENA_PATRON);

	// se ejecuta la acción...
    obj->setDato(&d);
	obj->guardar(FICHERO_TMP);

    // ...y se comprueban los resultados
    // el contenido del fichero temporal debe ser el mismo que el fichero patrón
	fichTMP = CreateFile(FICHERO_TMP, GENERIC_READ, 0, NULL,
						 OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	fichPatron = CreateFile(FICHERO_PATRON, GENERIC_READ, 0, NULL,
							OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    try {
        // los ficheros deben existir
        assert(INVALID_HANDLE_VALUE != fichTMP);
        assert(INVALID_HANDLE_VALUE != fichPatron);


        sizeTMP = GetFileSize(fichTMP, NULL);
        assert(sizeTMP != 0xFFFFFFFF);

        sizePatron = GetFileSize(fichPatron, NULL);
        assert(sizePatron != 0xFFFFFFFF);

        // el tamaño de ambos archivos debe ser igual a la estructura
        assertEquals(sizePatron, 	sizeTMP);
        assertEquals(sizeof(DATOS), sizeTMP);

        buffTMP    = new char[sizeTMP];
        buffPatron = new char[sizePatron];
        try {
			ZeroMemory(buffTMP,    sizeof(DATOS));
			ZeroMemory(buffPatron, sizeof(DATOS));

            assert( ReadFile(fichTMP, buffTMP, sizeTMP, &leidoTMP, NULL) );
            assertEquals(sizeTMP, leidoTMP);

            assert( ReadFile(fichPatron, buffPatron, sizePatron, &leidoPatron, NULL) );
            assertEquals(sizePatron, leidoPatron);

            assertEquals(leidoTMP, leidoPatron);

            // el contenido de los dos ficheros tiene que ser el mismo
            assert( CompareMem(buffTMP, buffPatron, sizeof(DATOS)) );
        }
        __finally {
            delete [] buffTMP;
            delete [] buffPatron;
        }
    }
    __finally {
		CloseHandle(fichTMP);
		CloseHandle(fichPatron);

        DeleteFile(FICHERO_TMP);
    }
}

void DatosDiscoTest::testDatoNulo()
{
	try {
		obj->setDato(NULL);
        assert(false);
    }
    catch(DatoNuloException) {
    }
}


