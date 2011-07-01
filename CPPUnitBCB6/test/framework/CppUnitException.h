/**
 * @class CppUnitException
 *
 * Se define una excepción expecializada que retorna su descripción a través del
 * método "what()"
 *
**/
#ifndef CPPUNIT_CPPUNITEXCEPTION_H
#define CPPUNIT_CPPUNITEXCEPTION_H


#include <pshpack8.h>
	#include <exception>
	#include <string>
#include <poppack.h>


#define CPPUNIT_UNKNOWNFILENAME        "<desconocido>"
#define CPPUNIT_UNKNOWNLINENUMBER      (-1)


class CppUnitException : public std::exception
{
public:
	CppUnitException(std::string  message    = "",
					 long         lineNumber = CPPUNIT_UNKNOWNLINENUMBER,
					 std::string  fileName   = CPPUNIT_UNKNOWNFILENAME);
	CppUnitException(const CppUnitException& other);

	virtual ~CppUnitException() throw();

	CppUnitException& operator= (const CppUnitException& other);

	const char* what() const throw();

	long        lineNumber();
	std::string fileName();

private:
	std::string m_message;
	long        m_lineNumber;
	std::string m_fileName;
};



CppUnitException::CppUnitException(const CppUnitException& other)
	: std::exception(other)
{
	m_message    = other.m_message;
	m_lineNumber = other.m_lineNumber;
	m_fileName   = other.m_fileName;
}

CppUnitException::CppUnitException (std::string message, long lineNumber, std::string fileName)
{
	m_message    = message;
	m_lineNumber = lineNumber;
	m_fileName   = fileName;
}


CppUnitException::~CppUnitException () throw()
{
}


CppUnitException& CppUnitException::operator= (const CppUnitException& other)
{
	std::exception::operator= (other);

	if (&other != this)
	{
		m_message    = other.m_message;
		m_lineNumber = other.m_lineNumber;
		m_fileName   = other.m_fileName;
	}

	return (*this);
}


const char* CppUnitException::what() const throw()
{
	return m_message.c_str();
}


long CppUnitException::lineNumber()
{
	return m_lineNumber;
}


std::string CppUnitException::fileName()
{
	return m_fileName;
}


#endif

