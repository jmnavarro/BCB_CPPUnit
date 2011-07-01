//---------------------------------------------------------------------------
#ifndef DatosDiscoH
#define DatosDiscoH

#include <stdexcept>

//---------------------------------------------------------------------------

typedef struct _DATOS
{
	int  numero;
    char cadena[256];
} DATOS, *LPDATOS;

//---------------------------------------------------------------------------

class DatoNuloException : public std::exception
{
public:
	DatoNuloException() {}
    virtual ~DatoNuloException() {}
};

//---------------------------------------------------------------------------

class DatosDisco
{
public:
	DatosDisco();
	~DatosDisco();

	LPDATOS getDato();
	void setDato(LPDATOS value);

	bool leer(char *archivo);
	bool guardar(char *archivo);

protected:

private:
	DATOS dato;
};

#endif
 