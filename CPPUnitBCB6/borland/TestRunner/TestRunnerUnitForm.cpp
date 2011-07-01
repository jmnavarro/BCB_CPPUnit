/**
 * @class TTestRunnerUnitForm
 *
 * Formulario base para la ejecución de pruebas en entorno gráfico
 *
**/
#include <vcl.h>
#pragma hdrstop

#include "TestRunnerUnitForm.h"
#include "Test.h"
#include "TestCase.h"
#include "TestSuite.h"
#include "TestResult.h"
#include "CppUnitException.h"
#include "GUITestResult.h"
#include <typeinfo>
#include <mmsystem.h>
//--------------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//--------------------------------------------------------------------------------------

__fastcall TTestRunnerForm::TTestRunnerForm(TComponent* Owner)
	: TForm(Owner)
{
	m_tests = 0;
}

__fastcall TTestRunnerForm::~TTestRunnerForm()
{
	freeState();
}


long GetTimePoint()
{
	return timeGetTime();
}


void TTestRunnerForm::showTests(Test* suite, TListBox *list)
{
	if( typeid(*suite) == typeid(TestSuite) )
	{
		list->Items->Add( "TestSuite:" );
		TestSuite* test_suite = dynamic_cast<TestSuite*>(suite);
		TestList& tests = test_suite->Tests();
		for(TestListIterator it = tests.begin(); it != tests.end(); ++it) {
			showTests(*it, list);
		}
	}
	else
	{
		TestCase* test_case = dynamic_cast<TestCase*>(suite);
		list->Items->Add( test_case->name().c_str() );
	}
}


void TTestRunnerForm::setTests (std::vector<Test *> *tests)
{
	m_tests = tests;
}


void TTestRunnerForm::addError(TestResult *result, Test *test, CppUnitException *e)
{
	addListEntry(ICO_ERROR, "Error", result, test, e);
	m_errors++;

	updateCountsDisplay();
}


void TTestRunnerForm::addFailure(TestResult *result, Test *test, CppUnitException *e)
{
	addListEntry (ICO_FALLO, "Fallo", result, test, e);
	m_failures++;

	updateCountsDisplay();
}


void TTestRunnerForm::addMessage(TestResult *result, Test *test, CppUnitException *e)
{
	addListEntry (ICO_INFO, "Nota", result, test, e);
//	m_failures++;

//	updateCountsDisplay();
}


void TTestRunnerForm::endTest(TestResult *result, Test *test)
{
	if (m_selectedTest.size() == 0)
		return;

	m_testsRun++;

	testsProgress->Position = m_testsRun;
//    step (m_failures == 0 && m_errors == 0);

	m_testEndTime = GetTimePoint();
	m_testTotalTime += m_testEndTime - m_testStartTime;

	updateCountsDisplay();

	if (m_testsRun >= test->countTestCases())
		beIdle();
}

void TTestRunnerForm::addListEntry(int ico, std::string type, TestResult *result, Test *test, CppUnitException *e)
{
//    int         currentEntry    = result->testErrors () + result->testFailures ();

	TListItem	*item = lv_result->Items->Add();

	string nr = (e->lineNumber() == CPPUNIT_UNKNOWNLINENUMBER)?
		"?"
		:
		IntToStr(e->lineNumber()).c_str();

	string name(test->toString());

	int p = name.find('<');
	if (p >= 0)
		name.erase(0, p+1);

	p = name.find('>');
	if (p >= 0)
		name.erase(p, 1);

	item->ImageIndex = ico;
	item->Caption = type.c_str();

	item->SubItems->Add(name.c_str()); // Set class string

	item->SubItems->Add(e->what()); // Set the asserted text
	item->SubItems->Add(nr.c_str()); // Set the line number
	item->SubItems->Add(e->fileName().c_str()); // Set the file name

//    listCtrl->RowCount = listCtrl->RowCount + 1;

	// mostramos el nuevo elemento
	item->MakeVisible(false);

	lv_result->Repaint();
}

void TTestRunnerForm::addListEntry(int ico, std::string type, TestResult *result, Test *test, std::string msg)
{

}



void TTestRunnerForm::beIdle()
{
	runButton->Enabled = true;
	closeButton->Enabled = true;
}


void TTestRunnerForm::beRunning()
{
	runButton->Enabled = false;
	closeButton->Enabled = false;
}


void TTestRunnerForm::beRunDisabled()
{
	runButton->Enabled = false;
	stopButton->Enabled = false;
	closeButton->Enabled = true;
}


void TTestRunnerForm::reset()
{
	m_testsRun    = 0;
	m_errors      = 0;
	m_failures    = 0;
	m_testTotalTime = 0;
	m_testEndTime = m_testStartTime;

	updateCountsDisplay();

	m_activeTest = 0;
	m_result     = 0;

	lv_result->Items->BeginUpdate();
	try {
		lv_result->Items->Clear();
	}
	__finally {
		lv_result->Items->EndUpdate();
		lv_result->Repaint();
	}

	setUplistCtrl();
//    m_testsProgress->reset();
	testsProgress->Position = 0;
}


void TTestRunnerForm::freeState()
{
//    delete m_activeTest;
	delete m_result;
}


void TTestRunnerForm::updateCountsDisplay()
{
	sb->Panels->Items[1]->Text = "Ejecuciones: " + IntToStr(m_testsRun);
	sb->Panels->Items[2]->Text = "Errores: " + IntToStr(m_errors);
	sb->Panels->Items[3]->Text = "Fallos: " + IntToStr(m_failures);

	AnsiString secs;

	if (m_testStartTime == 0 && m_testEndTime == 0)
		secs = "Tiempo: ? segs.";
	else if (m_testTotalTime >= 1000)
		secs = "Tiempo: " + FloatToStr(m_testTotalTime / 1000) + " segs.";
	else
		secs = "Tiempo: < 1 seg.";

	sb->Panels->Items[0]->Text = secs;

	sb->Repaint();
}


void TTestRunnerForm::clear_listCtrl()
{
	lv_result->Items->BeginUpdate();
	try {
		lv_result->Items->Clear();
	}
	__finally {
		lv_result->Items->EndUpdate();
	}
}


void TTestRunnerForm::setUplistCtrl()
{
/*
	clear_listCtrl();
	listCtrl->RowCount = 2;
	listCtrl->FixedRows = 1;

	listCtrl->Cells[0][0] = "Type";
//    listCtrl->ColWidths[0] = 60; //2 * listCtrl->GetStringWidth ("Type");

	listCtrl->Cells[1][0] = "Name";
//    listCtrl->ColWidths[1] = 70; //20 * listCtrl->GetStringWidth ("#")

	listCtrl->Cells[2][0] = "Failed Condition";
//    listCtrl->ColWidths[2] = 100; //1.75 * listCtrl->GetStringWidth ("Failed Condition")

	listCtrl->Cells[3][0] = "Line#";
//    listCtrl->ColWidths[3] = 70; //1.5 * listCtrl->GetStringWidth ("Line Number")

	listCtrl->Cells[4][0] = "File Name";
//    listCtrl->ColWidths[4] = 80; //4.0 * listCtrl->GetStringWidth ("File Name")
*/
}


void out(const char*m)
{
	MessageBox(GetActiveWindow(), m, "Información", MB_ICONINFORMATION);
}


int TTestRunnerForm::addTestsToSelectionWidget()
{
	int result;
	int numberOfCases = 0;
	for (TestListIterator it = m_tests->begin(); it != m_tests->end(); ++it)
	{
		comboTests->Items->Add( (*it)->toString ().c_str() );
		m_selectedTest.push_back(*it);
		numberOfCases++;
	}
	result = numberOfCases;
	return result;
}


void TTestRunnerForm::SelectedTestIs(int n)
{
	comboTests->ItemIndex = n;
}


void TTestRunnerForm::InitDialog()
{
	setUplistCtrl();

	int numberOfCases = addTestsToSelectionWidget();

	if (numberOfCases > 0)
		SelectedTestIs(numberOfCases-1);
	else
		beRunDisabled();

	this->Icon->Handle = Application->Icon->Handle;

	reset();
}


void __fastcall TTestRunnerForm::stopButtonClick(TObject *Sender)
{
	if (m_result)
		m_result->stop();

	beIdle();
}


void __fastcall TTestRunnerForm::runButtonClick(TObject *Sender)
{
	if (getSelectedTest() == 0)
		return;

	Screen->Cursor = crHourGlass;
	try
	{
		freeState();
		reset();

		beRunning();

		int numberOfTests = 0;
		TestListIterator it;

		for (it = m_selectedTest.begin(); it != m_selectedTest.end(); ++it)	{
			numberOfTests += (*it)->countTestCases();
		}

//        m_testsProgress->start(numberOfTests);
		testsProgress->Position = 0;
		testsProgress->Max = numberOfTests;

	//    ShowMessage(IntToStr(testsProgress->Max));

		m_result = new GUITestResult (this);

		m_testTotalTime = 0;

		for (it = m_selectedTest.begin(); it != m_selectedTest.end(); ++it)
		{
			m_activeTest = (*it);

			m_testStartTime = GetTimePoint();
			m_activeTest->run(m_result);
		}

		testsProgress->Position = testsProgress->Max;
	}
	__finally
	{
		Screen->Cursor = crDefault;

		if (m_errors == 0 && m_failures == 0)
			MessageBox(Handle, "Ejecución terminada correctamente.", "Ejecución terminada", MB_ICONINFORMATION);
		else
		{
			MessageBeep(0xFFFFFFFF);
			MessageBox(Handle, "Ejecución terminada con errores.", "Ejecución terminada", MB_ICONERROR);
		}
	}

}
//---------------------------------------------------------------------------


bool TTestRunnerForm::isCurrentSelectionValid()
{
	int currentSelection = comboTests->ItemIndex;
	int n_tests = m_tests->size();

	return (currentSelection >= 0) && (currentSelection < n_tests);
}


int TTestRunnerForm::getSelectedTest()
{
	int currentSelection = comboTests->ItemIndex;
	m_selectedTest.clear();
	m_selectedTest.push_back(*(m_tests->begin () + currentSelection));
	return m_selectedTest.size();
}


void TTestRunnerForm::changeSelectedTest()
{
	if ( isCurrentSelectionValid() ) {
		getSelectedTest();
		beIdle();
	}
	else {
		m_selectedTest.clear();
		beRunDisabled();
	}
	freeState();
	reset();
}


void __fastcall TTestRunnerForm::comboTestsChange(TObject *Sender)
{
	changeSelectedTest();
}


void __fastcall TTestRunnerForm::closeButtonClick(TObject *Sender)
{
	if (m_result)
		m_result->stop();

	Close();
}


void __fastcall TTestRunnerForm::FormShow(TObject *Sender)
{
	InitDialog();
}



