//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ITestRunner.h"

#include "DatosDiscoTest.h"

//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
        ITestRunner runner;

        runner.addTest( DatosDiscoTest::suite() );

        runner.run ();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

