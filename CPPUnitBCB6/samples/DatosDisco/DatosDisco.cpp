//---------------------------------------------------------------------------
#include "DatosDisco.h"
#include <windows.h>
//---------------------------------------------------------------------------

DatosDisco::DatosDisco()
{
	ZeroMemory(&this->dato, sizeof(DATOS));
}

DatosDisco::~DatosDisco()
{
}


LPDATOS DatosDisco::getDato()
{
	return &this->dato;
}

void DatosDisco::setDato(LPDATOS value)
{
	ZeroMemory(&dato, sizeof(DATOS));
    if (value)
    {
		dato.numero = value->numero;
	    strcpy(dato.cadena, value->cadena);
    }
    else
    	throw DatoNuloException();
}

bool DatosDisco::leer(char *archivo)
{
	bool ret;
	DWORD leido;
	HANDLE fich = CreateFile(archivo, GENERIC_READ, 0, NULL,
						 	 OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

   	if (INVALID_HANDLE_VALUE == fich)
    	return false;

	try {
		ret = ReadFile(fich, &dato, sizeof(DATOS), &leido, NULL);
    }
	__finally {
		CloseHandle(fich);
	}

    return ret;
}

bool DatosDisco::guardar(char *archivo)
{
	DWORD guardado;
	HANDLE fich = CreateFile(archivo, GENERIC_WRITE, 0, NULL,
			                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

   	if (INVALID_HANDLE_VALUE == fich)
    	return false;

	try {
		WriteFile(fich, &dato, sizeof(DATOS), &guardado, NULL);
    }
	__finally {
		CloseHandle(fich);
	}

	return (guardado == sizeof(DATOS));
}

