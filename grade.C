#include <iostream>
#include <fstream>
#include <string>



using namespace std;


ofstream openLogFile( char * filename )
{
    ofstream fout;

    string file = filename + ".log";
    
    fout.open(file.c_str());

    if (!fout)
        fout = NULL;
    return fout;
}

void findTests (vector<string> &testVector)
{
    //traverse all directories
}

int main( int argc, char * argv[] )
{

    ofstream logFout;
    vector<string> testVector;

    logFout = openLog(argv[1]);

    if ( logFout == NULL )
    {
        cout << "Error opening log file." << endl;
        return -1;
    }

    findTests(testVector);

}
