/**
 * @class TTreeRunnerUnitForm
 *
 * Formulario de ejecución de pruebas en entorno gráfico, mostrando la estructura de
 * colecciones de pruebas en forma de árbol.
 *
**/
#include <vcl.h>
#pragma hdrstop

#include "TreeTestUnitForm.h"
#include "Test.h"
#include "TestCase.h"
#include "TestSuite.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TestRunnerUnitForm"
#pragma resource "*.dfm"

#define ICO_CHECK	ICO_MAX + 1
#define ICO_UNCHECK	ICO_MAX + 2
#define ICO_OK		ICO_MAX + 3
#define ICO_BAD		ICO_MAX + 4


__fastcall TTreeTestForm::TTreeTestForm(TComponent* Owner)
	: TTestRunnerForm(Owner)
{
	soloActual = false;
}


bool TTreeTestForm::isCurrentSelectionValid()
{
	TTreeNode *node = testTree->Items->Item[0];

	while (node) {
		bool checked = (node->StateIndex != ICO_UNCHECK);

		if (checked)
			return true;

		node = node->getNextSibling();
	}
	return false;
}


int TTreeTestForm::getSelectedTest()
{
	int total;
	TTreeNode *node;

	m_selectedTest.clear();

	if (soloActual)
	{
		node = testTree->Selected;
		total = insertSelectedTest(node);
	}
	else
	{
		node = testTree->Items->Item[0];

		total = 0;
		while (node != NULL)
		{
			total += insertSelectedTest(node);
			node = node->getNextSibling();
		}
	}

	return (total);
}


int TTreeTestForm::insertSelectedTest(TTreeNode* root)
{
	TTreeNode	*hijo;

	hijo = root->getFirstChild();

	if (hijo == NULL)
	{
		// si no tiene hijos, se considera un test y no una suite
		if (root->StateIndex != ICO_UNCHECK || soloActual)
			m_selectedTest.push_back(static_cast<Test*>(root->Data));
	}
	else
	{
		while (hijo != NULL)
		{
			insertSelectedTest(hijo);
			hijo = hijo->getNextSibling();
		}
	}

	return m_selectedTest.size();
}



void TTreeTestForm::addTestToTree(Test* test,TTreeNode* root)
{
	if( typeid(*test) == typeid(TestSuite) )
	{
		TestSuite* test_suite = dynamic_cast<TestSuite*>(test);
		AnsiString id = test_suite->toString().c_str();
		AnsiString label = (id.Length() > 0)? id : AnsiString("SUITE");
		TTreeNode* node = testTree->Items->AddChildObject(root,label,(void*)test_suite);
		node->StateIndex = ICO_CHECK;

		TestList& tests = test_suite->Tests();
		for(TestListIterator it = tests.begin(); it != tests.end();	++it) {
			addTestToTree(*it,node);
		}
	}
	else
	{
		TestCase* test_case = dynamic_cast<TestCase*>(test);
		AnsiString id = test_case->toString().c_str();
		AnsiString label = (id.Length() > 0)? id : AnsiString("TEST CASE");
		if (label.Pos(">.") > 0)
			label.Delete(1, label.Pos(">.") + 1);

		TTreeNode* node = testTree->Items->AddChildObject(root,label,(void*)test_case);
		node->StateIndex = ICO_CHECK;
	}
}


void TTreeTestForm::switchNodeState(TTreeNode &node)
{
	testTree->Items->BeginUpdate();
	try
	{
		bool checked = (node.StateIndex != ICO_UNCHECK);

		if (checked)
			node.StateIndex = ICO_UNCHECK;
		else
			node.StateIndex = ICO_CHECK;

		// heredar a los hijos
		TTreeNode *aux = node.getFirstChild();
		while (aux)
		{
			marcarRama(aux, node.StateIndex);

			aux = aux->getNextSibling();
		}

		// marcar los padres
		aux = node.Parent;
		while (aux != NULL)
		{
			if ( debeEstarMarcado(*aux) )
				aux->StateIndex = ICO_CHECK;
			else
				aux->StateIndex = ICO_UNCHECK;

			aux = aux->Parent;
		}
	}
	__finally {
		testTree->Items->EndUpdate();
	}
}

bool TTreeTestForm::debeEstarMarcado(TTreeNode &node)
{
	TTreeNode *aux = node.getFirstChild();
	int total = 0;
	int uncheck = 0;

	while (aux)
	{
		total++;

		if (aux->StateIndex == ICO_UNCHECK)
			uncheck++;

		aux = aux->getNextSibling();
	}

	return (total != uncheck);
}


int TTreeTestForm::addTestsToSelectionWidget()
{
	for (TestListIterator it = m_tests->begin(); it != m_tests->end(); ++it)
	{
		addTestToTree(*it, testTree->Selected);
	}
	return m_tests->size();
}


void TTreeTestForm::SelectedTestIs(int)
{
	testTree->Selected = testTree->TopItem;
}


void __fastcall TTreeTestForm::FormShow(TObject *Sender)
{
	TTestRunnerForm::FormShow(Sender);

	testTree->DoubleBuffered = true;
}


void __fastcall TTreeTestForm::testTreeClick(TObject *Sender)
{
	// marcado y desmarcado del nodo

	if (!testTree->Selected)
		return;

	THitTests	HitInfo;
	TTreeNode 	*node;
	TPoint		p;

	GetCursorPos(&p);
	p = testTree->ScreenToClient(p);

	HitInfo = testTree->GetHitTestInfoAt(p.x, p.y);
	node = testTree->GetNodeAt(p.x, p.y);

	if (node != NULL && HitInfo.Contains(htOnStateIcon))
		switchNodeState(*node);

	runButton->Enabled = isCurrentSelectionValid();
}

void TTreeTestForm::endTest(TestResult *result, Test *test)
{
	TTestRunnerForm::endTest(result, test);

	TTreeNode *n = getTestNode(test);
	if (n->StateIndex == ICO_CHECK || (soloActual && n->StateIndex == ICO_UNCHECK) )
	{
		n->StateIndex = ICO_OK;

		if (n->Parent->Expanded) {
			n->MakeVisible();
		}

		// marcar padre
		n = n->Parent;
		while (n != NULL)
		{
			marcarEstadoSegunHijos(n);
			n = n->Parent;
		}
	}

	testTree->Repaint();
}


void TTreeTestForm::marcarEstadoSegunHijos(TTreeNode *padre)
{
	TTreeNode *n = padre->getFirstChild();
	int bad = 0;
	int ok = 0;
	int check = 0;
	while (n)
	{
		switch (n->StateIndex)
		{
			case ICO_OK:
				++ok;
				break;
			case ICO_BAD:
				++bad;
				break;
			case ICO_CHECK:
				++check;
				break;
		}
		n = n->getNextSibling();
	}

	if (bad > 0)
	{
		padre->StateIndex = ICO_BAD;
		padre->MakeVisible();
	}
	else if (ok > 0)
		padre->StateIndex = ICO_OK;
	else if (check > 0)
		padre->StateIndex = ICO_CHECK;
	else
		padre->StateIndex = ICO_UNCHECK;
}


void TTreeTestForm::marcarRama(TTreeNode * node, int imagen)
{
	node->StateIndex = imagen;
	node = node->getFirstChild();
	while (node)
	{
		marcarRama(node, imagen);
		node = node->getNextSibling();
	}
}


void TTreeTestForm::marcarRamaError(TTreeNode * node, int imagen)
{
	if (node->StateIndex == ICO_BAD)
		node->StateIndex = ICO_CHECK;
	else if (node->StateIndex == ICO_OK)
		node->StateIndex = ICO_UNCHECK;

	node = node->getFirstChild();
	while (node)
	{
		if (node->StateIndex == ICO_BAD)
			node->StateIndex = ICO_CHECK;
		else if (node->StateIndex == ICO_OK)
			node->StateIndex = ICO_UNCHECK;

		node = node->getNextSibling();
	}
}


void TTreeTestForm::marcarHaciaAbajo(TTreeNode* node, int ico)
{
	TTreeNode *aux = node;

	while (aux != NULL)
	{
		marcarRama(aux, ico);
		aux = aux->getNextSibling();
	}

	if (node)
	{
		aux = testTree->Selected->Parent;
		while (aux != NULL)
		{
			marcarEstadoSegunHijos(aux);
			aux = aux->Parent;
		}
	}
}


void TTreeTestForm::addError(TestResult *result, Test *test, CppUnitException *e)
{
	TTreeNode *padre;
	TTreeNode *n;

	TTestRunnerForm::addError(result, test, e);

	n = getTestNode(test);
	n->StateIndex = ICO_BAD;

	padre = n->Parent;
	while (padre != NULL)
	{
		marcarEstadoSegunHijos(padre);
		padre = padre->Parent;
	}

	n->MakeVisible();
}


void TTreeTestForm::addFailure(TestResult *result, Test *test, CppUnitException *e)
{
	TTestRunnerForm::addFailure(result, test, e);
	TTreeNode *n = getTestNode(test);
	n->StateIndex = ICO_BAD;
	n->Parent->StateIndex = ICO_BAD;
}


TTreeNode* TTreeTestForm::getTestNode(Test *t)
{
	for (int i=0; i<testTree->Items->Count; i++) {
		if (static_cast<Test*>(testTree->Items->Item[i]->Data) == t)
			return testTree->Items->Item[i];
	}
	return NULL;
}


void TTreeTestForm::reset()
{
	TTestRunnerForm::reset();

	for (int i=0; i<testTree->Items->Count; i++)
	{
		if (testTree->Items->Item[i]->StateIndex != ICO_UNCHECK)
			testTree->Items->Item[i]->StateIndex = ICO_CHECK;
	}
}


void __fastcall TTreeTestForm::testTreeKeyPress(TObject *Sender, char &Key)
{
	if (Key == ' ' && testTree->Selected != NULL)
	{
		this->switchNodeState(*testTree->Selected);

		runButton->Enabled = isCurrentSelectionValid();
	}
}


void __fastcall TTreeTestForm::m_marcarTodosClick(TObject *Sender)
{
	TTreeNode  *node = testTree->Items->Item[0];

	while (node)
	{
    	marcarRama(node, ICO_CHECK);
		node = node->getNextSibling();
	}
}


void __fastcall TTreeTestForm::m_desmarcarTodosClick(TObject *Sender)
{
	TTreeNode	*node = testTree->Items->Item[0];

	while (node)
	{
		marcarRama(node, ICO_UNCHECK);
		node = node->getNextSibling();
    }

}


void __fastcall TTreeTestForm::m_expandirTodoClick(TObject *Sender)
{
	testTree->FullExpand();
}


void __fastcall TTreeTestForm::m_contraerTodoClick(TObject *Sender)
{
	testTree->FullCollapse();
}


void __fastcall TTreeTestForm::m_marcarConFallosClick(TObject *Sender)
{
	TTreeNode  *node = testTree->Items->Item[0];

    while (node)
	{
		if (node->StateIndex == ICO_BAD)
			marcarRamaError(node, ICO_CHECK);
		else if (node->StateIndex == ICO_OK)
			marcarRamaError(node, ICO_UNCHECK);

		node = node->getNextSibling();
	}
}


void __fastcall TTreeTestForm::m_marcarDesdeAquiClick(TObject *Sender)
{
	marcarHaciaAbajo(testTree->Selected, ICO_CHECK);
}


void __fastcall TTreeTestForm::m_desmarcarDesdeAquiClick(TObject *Sender)
{
	marcarHaciaAbajo(testTree->Selected, ICO_UNCHECK);
}


void __fastcall TTreeTestForm::Ejecutarestaprueba1Click(TObject *Sender)
{
	soloActual = true;
	try {
		runButtonClick(Sender);
	}
	__finally {
		soloActual = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TTreeTestForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	switch(Key)
	{
		case VK_F9:
		{
			if (Shift.Contains(ssCtrl))
			{
				Ejecutarestaprueba1Click(Sender);
			}
			else if (Shift.Empty())
			{
				runButtonClick(Sender);
			}
			break;
		}

		case VK_RETURN:
		{
			if (Shift.Contains(ssCtrl))
			{
				Ejecutarestaprueba1Click(Sender);
			}
			break;
		}
	}
}
//---------------------------------------------------------------------------





