#include <windows.h>

#include "TestRunner.h"
#include "TestRunnerUI.h"
#include "Test.h"

TestRunner::TestRunner()
{
}


TestRunner::~TestRunner()
{
	TestListIterator it;

	for (it = m_tests.begin(); it != m_tests.end(); ++it) {
		delete *it;
	}
}


void TestRunner::run()
{
	try
	{
		TestRunnerUI  dlg;

		dlg.setTests(&m_tests);
		dlg.ShowModal();
	}
	catch(std::exception& e)
	{
		MessageBox(GetActiveWindow(), e.what(), "Excepción", MB_ICONINFORMATION);
	}
}


void TestRunner::addTest(Test* test)
{
	m_tests.push_back(test);
}

