#include "TestRunner.h"

void TestRunner::printBanner ()
{
    cout << "Usage: driver [ -wait ] testName, where name is the name of a test case class" << endl;
}


void TestRunner::run (int ac, char **av)
{
    string  testCase;
    int     numberOfTests = 0;

    for (int i = 1; i < ac; i++) {

        if (string (av [i]) == "-wait") {
            m_wait = true;
            continue;
        }

        testCase = av [i];

        if (testCase == "") {
            printBanner ();
            return;
        }

        Test *testToRun = NULL;

        for (mappings::iterator it = m_mappings.begin ();
                it != m_mappings.end ();
                ++it) {
            if ((*it).first == testCase) {
                testToRun = (*it).second;
                run (testToRun);

            }
        }

        numberOfTests++;

        if (!testToRun) {
            cout << "Test " << testCase << " not found." << endl;
            return;

        } 


    }

    if (numberOfTests == 0) {
        printBanner ();
        return;        
    }

    if (m_wait) {
        cout << "<RETURN> to continue" << endl;
        cin.get ();

    }


}


TestRunner::~TestRunner ()
{
    for (mappings::iterator it = m_mappings.begin ();
             it != m_mappings.end ();
             ++it)
        delete it->second;

}


void TestRunner::run (Test *test)
{
    TextTestResult  result;

    test->run (&result);

    cout << result << endl;
}

/*
int main (int ac, char **av)
{
    TestRunner runner;

    runner.addTest ("ExampleTestCase", ExampleTestCase::suite ());
    runner.run (ac, av);

    return 0;
}
*/
