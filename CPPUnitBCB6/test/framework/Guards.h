#ifndef CPPUNIT_GUARDS_H
#define CPPUNIT_GUARDS_H

/**
 * Una macro para no permitir la asignaci�n de objetos
**/
#define REFERENCEOBJECT(className) \
private: \
	className (const className& other); \
	className& operator= (const className& other); 

#endif
