#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <string>
#include <omp.h>
#include <dirent.h>


using namespace std;


void CompileSourceFile ( string name );
void FindTests ( vector<string> &testVector );
void OpenLogFile( ofstream & fout, string fileName );
void UsageMenu();


vector<string> TESTVECTOR;
struct dirent *ITEM;

int main( int argc, char * argv[] )
{
    int threadCount = 0;

    ofstream logFout;
//    vector<string> testVector;
    string target = argv[1];

    #pragma omp master
    {
        threadCount = omp_get_num_procs() - 2 <= 0 ? 1 : omp_get_num_procs() - 2;
    }

    if (argc != 2)
    {
        UsageMenu();
        return 1;
    }

    OpenLogFile(logFout, target);
    CompileSourceFile( target );

    if ( !logFout ) // not java so we don't need this compare
    {
        cout << "Error opening log file." << endl;
        return -1;
    }

    #pragma omp parallel num_threads(threadCount)
    {
        FindTests(testVector);

    }

    // debug;
    cout << threadCount << endl;

    return 0;
}


void CompileSourceFile ( string name )
{
    int index = name.find_last_of(".C");
    // if not found, try cpp
    if (index == -1)
    {
        index = name.find_last_of(".cpp");
    }

    // compile the prog
    string cmd = "g++ -o " + name.substr(0,index-1) + " " + name + " -g ";
    system(cmd.c_str());
}

void FindTests ()
{   
    // c ish, sorry
    char[1024] = cwd;
    getcwd(cwd, sizeof(cwd));
    // error
    if (!cwd)
    {
        TESTVECTOR = NULL;
        return;
    }

    string root = cwd;
    DIR *dir;
      
    //traverse all directories

}


void OpenLogFile( ofstream & fout, string fileName )
{

    string file = fileName + ".log";

    fout.open(file.c_str());

//    if (!fout)  should be null if bad ptr
//        fout = NULL;
}

void UsageMenu()
{
    cout << "Please re-run as './grade <source to test>'" << endl << endl;
}
