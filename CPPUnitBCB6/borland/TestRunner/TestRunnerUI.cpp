#include <vcl.h>
#pragma hdrstop

#include "TestRunnerUI.h"

#include "TestRunnerUnitForm.h"
#include "TreeTestUnitForm.h"


TestRunnerUI::TestRunnerUI()
	: m_dlg(0)
{
//	m_dlg = new TTestRunnerForm(Application);
	m_dlg = new TTreeTestForm(Application);
}

TestRunnerUI::~TestRunnerUI()
{
	delete m_dlg;
}


void TestRunnerUI::setTests (TestList* tests)
{
	m_dlg->setTests(tests);
}


void TestRunnerUI::ShowModal()
{
	m_dlg->ShowModal();
}

