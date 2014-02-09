#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <string>
#include <omp.h>
#include <unistd.h>
#include <dirent.h>


using namespace std;


void CompileSourceFile ( string name );
void FindTests ();
void OpenLogFile( ofstream & fout, string fileName );
void UsageMenu();
void ParseDirectory(string root);


vector<string> TESTVECTOR;
struct dirent *ITEM;

int main( int argc, char * argv[] )
{
    int threadCount = 0;

    ofstream logFout;
    string target = argv[1];

    #pragma omp master
    {
        // debug; You made a damn ? line!!
        threadCount = omp_get_num_procs() - 2 <= 0 ? 1 : omp_get_num_procs() - 2;
    }

    if (argc != 2)
    {
        UsageMenu();
        return 1;
    }

    OpenLogFile(logFout, target);
    CompileSourceFile( target );

    if ( !logFout )
    {
        cout << "Error opening log file." << endl;
        return -1;
    }
    
    // debug; You can try to parallelize things, but I think this stays serial
    //#pragma omp parallel num_threads(threadCount)
    {
        FindTests();
    }
    
    if ( TESTVECTOR.empty() )
    {
        //no tests found, do something
        return -2;
    }
    
    // debug;
    //for ( int i = 0; i < TESTVECTOR.size(); i++ )
        //cout << TESTVECTOR[i] << endl;
    // debug;
    // cout << threadCount << endl;

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

    //does work on directories and files with spaces
    char cwd[1024] = {0};
    string root;
    
    getcwd(cwd, sizeof(cwd));
    // error
    if (!cwd)
    {
        // This line does not compile. Also, seems useless.
        // TESTVECTOR = NULL;
        return;
    }

    root = cwd;
    
    ParseDirectory(root);
}

void ParseDirectory(string root)
{  
    string temp;

    DIR *dir = opendir(root.c_str()); // open the current directory
    struct dirent *entry;

    if (!dir)
    {
        // not a directory
        return;
    }

    while (entry = readdir(dir)) // notice the single '='
    {
        temp = entry->d_name;
        if ( temp != "." )
        {
            if ( temp != ".." )
            {
                if ( temp[temp.size() - 1] != '~' )
                {
                    int length = temp.length();
                    if ( length > 4 && temp[length-1] == 't' && temp[length-2] 
                        == 's' && temp[length-3] == 't' && temp[length-4] == '.' )
                    {
                        TESTVECTOR.push_back(root+'/'+temp);
                    }
                    else
                    {
                        ParseDirectory(root+'/'+temp);
                    }
                }
            }
        }
    }
    
    closedir(dir);

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
