#include "ITestRunner.h"
#include "TestRunner.h"


ITestRunner::ITestRunner()
	: m_impl(0)
{
	m_impl = new TestRunner;
}

ITestRunner::~ITestRunner()
{
	delete m_impl;
}

void ITestRunner::run()
{
	m_impl->run();
}

void ITestRunner::addTest(Test* test)
{
	m_impl->addTest(test);
}

