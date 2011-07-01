#ifndef TestRunnerUnitFormH
#define TestRunnerUnitFormH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>

#include <ImgList.hpp>

#pragma pack(push, 8)
	#include <vector>
#pragma pack(pop)

#include "Test.h"
using namespace std;

//
// Índice en el imagelist
//
#define ICO_ERROR	0
#define ICO_FALLO	1
#define ICO_INFO	2
#define ICO_MAX		ICO_INFO


class TestResult;
class CppUnitException;


//---------------------------------------------------------------------------
class TTestRunnerForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TStatusBar *sb;
	TProgressBar *testsProgress;
	TPanel *Panel4;
	TPanel *Panel5;
	TImageList *iconos;
	TPanel *Panel6;
	TListView *lv_result;
	TPanel *Panel7;
	TLabel *Label1;
	TBevel *Bevel3;
	TComboBox *comboTests;
	TLabel *Label6;
	TBevel *Bevel2;
	TBevel *Bevel1;
	TSplitter *Splitter1;
	TPanel *Panel8;
	TButton *runButton;
	TButton *stopButton;
	TButton *closeButton;
    void __fastcall stopButtonClick(TObject *Sender);
    void __fastcall runButtonClick(TObject *Sender);
	void __fastcall comboTestsChange(TObject *Sender);
	void __fastcall closeButtonClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:
	Test            *m_activeTest;
	TestResult      *m_result;
	int             m_testsRun;
	long			m_testStartTime;
	long			m_testEndTime;
	long            m_testTotalTime;

	void addListEntry(int ico, std::string type, TestResult *result, Test *test, CppUnitException *e);
	void addListEntry(int ico, std::string type, TestResult *result, Test *test, std::string msg);
	void beIdle              ();
	void beRunning           ();
	void beRunDisabled       ();
	void freeState           ();
	void updateCountsDisplay ();

	void InitDialog();
	void showTests(Test* suite, TListBox *list);
	void clear_listCtrl();
	void setUplistCtrl();

protected:
	int m_errors;
	int m_failures;

	TestList  *m_tests;
	TestList  m_selectedTest;

	void changeSelectedTest();
	virtual void reset();
	virtual bool isCurrentSelectionValid();
	virtual int getSelectedTest();
	virtual int addTestsToSelectionWidget();
	virtual void SelectedTestIs(int);

public:
	__fastcall TTestRunnerForm(TComponent* Owner);
	__fastcall ~TTestRunnerForm();
	void setTests(TestList*);
	virtual void addError(TestResult *result, Test *test, CppUnitException *e);
	virtual void addFailure(TestResult *result, Test *test, CppUnitException *e);
	virtual void addMessage(TestResult *result, Test *test, CppUnitException *e);
	virtual void endTest(TestResult *result, Test *test);
};
//---------------------------------------------------------------------------
//extern PACKAGE TTestRunnerForm *TestRunnerForm;
//---------------------------------------------------------------------------
#endif

