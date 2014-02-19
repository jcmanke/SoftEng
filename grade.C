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


string CompileSourceFile ( string name );
string diffCall(string cmd);
void ExecuteTests(string prog);
void FindTests ();
void UsageMenu();
void ParseDirectory(string root);
void WriteLog(string prog);


int CORRECTTESTS = 0;
vector<string> TESTVECTOR;
struct dirent *ITEM;

int main( int argc, char * argv[] )
{
    ofstream logFout;
    string target, sourceFile;
    if (argc != 2)
    {
        UsageMenu();
        return 1;
    }

    sourceFile = argv[1];
    
    FindTests();
    
    target = CompileSourceFile( sourceFile );
    
    if ( !logFout )
    {
        cout << "Error opening log file." << endl;
        return -1;
    }  
       
    if ( TESTVECTOR.empty() )
    {
        //no tests found, do something
        cout << "No tests found." << endl;
        return -2;
    }
    
    ExecuteTests( target );

    WriteLog( sourceFile );

    return 0;
}

//returns shortened name
string CompileSourceFile ( string name )
{
    int index = name.find_last_of(".C");
    // if not found, try cpp
    if (index == -1)
    {
        index = name.find_last_of(".cpp");
    }

    string baseName = name.substr(0,index);
    // compile the prog
    string cmd = "g++ -o " + baseName + " " + name + " -g ";
    system(cmd.c_str());
    
    return baseName;
}

string diffCall(string cmd)
{
    // don't know how to do this without a ptr to a pipe
    FILE *diff = popen(cmd.c_str(),"r");
    char answer[1024]; // bit large, but w/e
    string returnString;
    
    // while not termed end of pipe (end of reply)
    while(!feof(diff))
    {
        // check for nulls
        if(!fgets(answer, 1024, diff))
            returnString = returnString + answer; // save the answer
    }
    // Debug
    //cout << cmd << endl << returnString << endl;
    // close pipe
    pclose(diff);

    return returnString;
}

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
        //string diff = diffCall(cmd);
        int diff = system(cmd.c_str());
        // convert to string
        ss << diff;
        // if diff's length is 0, there is no difference in the files...
        // however the system likes to give /177 as an empty or delete string
        if ( diff == 0)
        {
            int index = TESTVECTOR[i].find_last_of(".tst");
            TESTVECTOR[i] = TESTVECTOR[i].substr(0,index) + ": succeeded perfectly\n";
            CORRECTTESTS += 1;
        }
        else 
        {
            int index = TESTVECTOR[i].find_last_of(".tst");
            TESTVECTOR[i] = TESTVECTOR[i].substr(0,index) + ": has discrepancies\n";
        }
    }
    
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
        if ( temp != "." && temp != ".." )
        {
            if ( temp[temp.size() - 1] != '~' )
            {
                int length = temp.length();
                if ( length > 4 && temp.substr(length-4) == ".tst")
                    /*temp[length-1] == 't' && temp[length-2] 
                    == 's' && temp[length-3] == 't' && temp[length-4] == '.' )*/
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


void UsageMenu()
{
    cout << "Please re-run as './grade <source to test>'" << endl << endl;
}

void WriteLog(string prog)
{
    // make name / date log.
    time_t now;
    time(&now);
    string currTime = ctime(&now);
    string name = prog + ".log";
    ofstream log;
    log.open(name.c_str(), std::ofstream::app);
    
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
    log << "Success Rate: " << CORRECTTESTS/ TESTVECTOR.size() * 100 << "%" << endl; 
}
