//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "ITestRunner.h"

#include "MulticasterTest.h"
#include "ExampleTestCase.h"

//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        ITestRunner runner;

        runner.addTest( MulticasterTest::suite() );
        runner.addTest( ExampleTestCase::suite() );

        runner.run ();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------

