
#pragma hdrstop
#include <condefs.h>
#include <iostream>

//---------------------------------------------------------------------------
#include "ExampleTestCase.h"
#include "TestRunner.h"

#pragma argsused
int main (int ac, char **av)
{
    TestRunner runner;

    runner.addTest("ExampleTestCase", ExampleTestCase::suite() );
    runner.run(ac, av);

    getchar();
    return 0;
}
