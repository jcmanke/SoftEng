#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <string>
#include <unistd.h>
#include <dirent.h>
#include <ctime>
#include <sstream>

using namespace std;

/*************************************************************************//**
 * F:ile grade.C
 *
 * Program 1 - Automated Grader 
 *
 * Course Information
 * Authors: Alex Wulf, Adam Meaney, Joe Manke
 * 
 * Due Date: 2-19-2014
 * 
 * Professor: Dr. Logar
 * 
 * Course: Software Engineering CSC470
 *
 * Program Information 
 * 
 * Program Description: The program automatically grades basic programs.
 *
 * When a program is given to our program, we compile it and open a log file
 * to facilitate grading. We then search all following directories for tests
 * in the form of .tst files. We then use those tests as input files to 
 * write the output of the programs and test it against the answer files.
 *
 *
 * Compiling and Usage 
 *
 * Compiling Instructions:
 *  make
 * 
 * Usage:  
 *  ./grade <source>
 *****************************************************************************/

//prototypes
string CompileSourceFile ( string name );
void ExecuteTests(string prog);
void FindTests ();
void UsageMenu();
void ParseDirectory(string root);
bool WriteLog(string prog);

//globals
int CORRECTTESTS = 0;
vector<string> TESTVECTOR;
struct dirent *ITEM;

/**************************************************************************//**
 * Function: Main
 * 
 * Description: Starting point of the program. Checks command line arguments
 *  and calls other functions to perform main work of the program.
 * 
 * Parameters:
 *  argc - number of command line arguments
 *  argv - array of command line arguments
 *
 * Returns:
 *  0 - program completed successfully
 *  1 - incorrect number of command line arguments
 *  -1 - no tests found
 *  -2 - error opening log file
 *****************************************************************************/
int main( int argc, char * argv[] )
{
    string target, sourceFile;
    int logSuccess = 0;
    if (argc != 2)
    {
        UsageMenu();
        return 1;
    }

    sourceFile = argv[1];
    
    FindTests();
    
    target = CompileSourceFile( sourceFile ); 
       
    if ( TESTVECTOR.empty() )
    {
        //no tests found, do something
        cout << "No tests found." << endl;
        return -1;
    }
    
    ExecuteTests( target );

    if(!WriteLog( sourceFile ) )
    {
        cout << "Error opening log file." << endl;
        return -2;        
    }
    return 0;
}

/**************************************************************************//**
 * Function: CompileSourceFile
 * 
 * Description: Compiles an executable program from a given source code file.
 * 
 * Parameters:
 *  name - the name of the source code file to compile
 *
 * Returns:
 *  the executable's name (source file stripped of extension)
 *****************************************************************************/
string CompileSourceFile ( string name )
{
    int index = name.find_last_of(".C");
    // if not found, try cpp
    if (index == -1)
    {
        index = name.find_last_of(".cpp");
    }

    string executable = name.substr(0,index);
    // compile the prog
    string cmd = "g++ -o " + executable + " " + name + " -g ";
    system(cmd.c_str());
    
    return executable;
}

/**************************************************************************//**
 * Function: ExecuteTests
 * 
 * Description: Runs the target program against all test cases stored in the
 *  TESTVECTOR. Uses the diff command to determine pass or fail on each case.
 *  Rewrites TESTVECTOR with results and compiles overall statistics.
 * 
 * Parameters:
 *  prog - the name of the target executable program
 *
 * Returns:
 *  none
 *****************************************************************************/
void ExecuteTests(string prog)
{
    stringstream ss;
    string inFile, outFile, ansFile;
    for (int i = 0; i < TESTVECTOR.size(); i++)
    {
        inFile = TESTVECTOR[i];
        outFile = TESTVECTOR[i].substr(0,TESTVECTOR[i].length() - 3) + "out";
        ansFile = TESTVECTOR[i].substr(0,TESTVECTOR[i].length() - 3) + "ans";

        // Execute code against test case
        string cmdString = "./" + prog + " < " + inFile + " > " + outFile;
        cmdString += " | > /dev/null";
        system( cmdString.c_str() );
    
        // compare
        string cmd = "diff " + ansFile + " " + outFile + " > /dev/null";
        int diff = system(cmd.c_str());
        // convert to string
        ss << diff;
        // if diff's length is 0, there is no difference in the files...
        // however the system likes to give /177 as an empty or delete string
        if ( diff == 0)
        {
            int index = TESTVECTOR[i].find_last_of(".tst");
            TESTVECTOR[i] = TESTVECTOR[i].substr(0,index) + ": succeeded perfectly\n";
            CORRECTTESTS ++;
        }
        else 
        {
            int index = TESTVECTOR[i].find_last_of(".tst");
            TESTVECTOR[i] = TESTVECTOR[i].substr(0,index) + ": has discrepancies\n";
        }
    }
    
}

/**************************************************************************//**
 * Function: FindTests
 * 
 * Description: Gets the root directory of the program, then makes a call to
 *  ParseDirectory() to find all test cases with the directory and its children.
 * 
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 *****************************************************************************/
void FindTests ()
{   

    //does work on directories and files with spaces
    char cwd[1024] = {0};
    string root;
    
    getcwd(cwd, sizeof(cwd));
    // error
    if (!cwd)
    {
        return;
    }

    root = cwd;
    
    ParseDirectory(root);
}

/**************************************************************************//**
 * Function: Main
 * 
 * Description: Recursively searches for test case files (notated by extension
 *  .tst). Test cases are added to the TESTVECTOR.
 * 
 * Parameters:
 *  root - the directory to search in
 *
 * Returns:
 *  none
 *****************************************************************************/
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
        if ( temp != "." && temp != ".." )
        {
            if ( temp[temp.size() - 1] != '~' )
            {
                int length = temp.length();
                if ( length > 4 && temp.substr(length-4) == ".tst")
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
    
    closedir(dir);
}

/**************************************************************************//**
 * Function: UsageMenu
 * 
 * Description: Prints a usage statement for this program to the screen.
 * 
 * Parameters:
 *  none
 *
 * Returns:
 *  none
 *****************************************************************************/
void UsageMenu()
{
    cout << "Please re-run as './grade <source to test>'" << endl << endl;
}

/**************************************************************************//**
 * Function: Main
 * 
 * Description: Writes test results to a file named <prog>.log.
 * 
 * Parameters:
 *  prog - the name of the tested source code file
 *
 * Returns:
 *  true - if log was written successfully
 *  false - if log file failed to open
 *****************************************************************************/
bool WriteLog(string prog)
{
    // make name / date log.
    time_t now;
    time(&now);
    string currTime = ctime(&now);
    
    //open log file
    string name = prog + ".log";
    ofstream log;
    
    log.open(name.c_str(), std::ofstream::app);    
    if(!log)
        return false;

    log << currTime.substr(0,currTime.length() - 2) << endl;
    for (int i = 0; i < TESTVECTOR.size(); i+=1)
    {
        log << TESTVECTOR[i] << endl; // flush buffer as long strings
    }

    // now push stats
    log << "            |" << endl;
    log << "Bottom Line V" << endl;
    log << "--------------------------------------------------------------------------------" << endl;
    log << "Correct: " << CORRECTTESTS << "\nFailed: " << TESTVECTOR.size() - CORRECTTESTS << endl;
    log << "Success Rate: " << (double)CORRECTTESTS / TESTVECTOR.size() * 100 << "%" << endl; 
    log << "--------------------------------------------------------------------------------" << endl << endl;
    
    log.close();
    return true;
}
