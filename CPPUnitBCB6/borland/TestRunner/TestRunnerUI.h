#ifndef TestRunnerUIH
#define TestRunnerUIH


#include "Test.h"
#include "TreeTestUnitForm.h"


class TTestRunnerForm;


class TestRunnerUI
{
public:
	TestRunnerUI();
	virtual ~TestRunnerUI();

	virtual void setTests(TestList*);
	virtual void ShowModal();

private:
	TTestRunnerForm* m_dlg;
};
#endif
