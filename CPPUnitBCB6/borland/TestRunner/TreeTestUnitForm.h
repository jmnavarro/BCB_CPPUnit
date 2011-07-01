//---------------------------------------------------------------------------
#ifndef TreeTestUnitFormH
#define TreeTestUnitFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TestRunnerUnitForm.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TTreeTestForm : public TTestRunnerForm
{
__published:	// IDE-managed Components
	TTreeView *testTree;
	TPopupMenu *m_arbol;
	TMenuItem *m_marcarTodos;
	TMenuItem *m_desmarcarTodos;
	TMenuItem *N1;
	TMenuItem *m_marcarConFallos;
	TMenuItem *N2;
	TMenuItem *m_marcarDesdeAqui;
	TMenuItem *m_desmarcarDesdeAqui;
	TMenuItem *m_expandirTodo;
	TMenuItem *m_contraerTodo;
	TMenuItem *N3;
	TMenuItem *N4;
	TMenuItem *Ejecutartodo1;
	TMenuItem *Ejecutarestaprueba1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall testTreeClick(TObject *Sender);
	void __fastcall testTreeKeyPress(TObject *Sender, char &Key);
	void __fastcall m_marcarTodosClick(TObject *Sender);
	void __fastcall m_desmarcarTodosClick(TObject *Sender);
	void __fastcall m_expandirTodoClick(TObject *Sender);
	void __fastcall m_contraerTodoClick(TObject *Sender);
	void __fastcall m_marcarConFallosClick(TObject *Sender);
	void __fastcall m_marcarDesdeAquiClick(TObject *Sender);
	void __fastcall m_desmarcarDesdeAquiClick(TObject *Sender);
	void __fastcall Ejecutarestaprueba1Click(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

private:
	void addTestToTree(Test*,TTreeNode*);
	void switchNodeState(TTreeNode &node);

	void marcarRama(TTreeNode * node, int imagen);
	void marcarRamaError(TTreeNode * node, int imagen);
	void marcarEstadoSegunHijos(TTreeNode *padre);
	void marcarHaciaAbajo(TTreeNode* node, int ico);
	bool debeEstarMarcado(TTreeNode &node);

	TTreeNode *getTestNode(Test *t);
	int insertSelectedTest(TTreeNode* root);

	bool soloActual;

protected:
	virtual bool isCurrentSelectionValid();
	virtual int getSelectedTest();
	virtual int addTestsToSelectionWidget();
	virtual void SelectedTestIs(int);
	virtual void reset();

public:
	virtual void endTest (TestResult *result, Test *test);
    virtual void addError(TestResult *result, Test *test, CppUnitException *e);
    virtual void addFailure(TestResult *result, Test *test, CppUnitException *e);

    __fastcall TTreeTestForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TTreeTestForm *TreeTestForm;
//---------------------------------------------------------------------------
#endif
