/*******************************************************************************
  Tester.cpp
    CSC 470 Tester Program
    Date: Feb 2014
    Authors: Ryan Feather, Ryan Brown, and Kelsey Bellew
    
    Usage:
      -Program expects to be passed a .cpp file program via the command line
      -Program will compile and run the .cpp file passed to it and then search
        for all files ending in .tst in the current and subdirectories and read
        these files as inputs to the program being tested.
      -Results of the tests will be listed in a .log file in the same directory
        as this program when ran.
        
    Restrictions:
      -Program expects the .cpp file being tested to:
        o Successfully compile
        o Not crash when given the input from a .tst file
        o Take all input via the command line (cin)
*******************************************************************************/  

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <fstream>
#include <time.h>
#include <vector>
#include <sstream>
// QQQ!!! Alex : added
#include <algorithm>
#include <climits>
#include <cstdlib>

using namespace std;

//Function Prototypes
string runtests(string progname, string specifictestcase);
void writefinaloutfile(string progname, vector<string> finaloutfilecontents);
vector<string> find_tsts(string progdir);
int filesequal(string file1name, string file2name);
void generatetestcases();
void generatetestcasesmenu(bool &doubles, bool &lesserThanAmount, 
                           bool &greaterThanAmount, double &min, double &max, 
                           int &amountToGenerate, int &filesToMake);
void solvetestcases();


/*********************************** main ***********************************/
//  Primary function of the program
/****************************************************************************/
int main(int argc, char* argv[])
{
  string progname, prog_cpp, progcomp, progdir;
  
  //holds each test and result on a separate line
  vector<string> finaloutfilecontents;
  finaloutfilecontents.clear();

  //test for proper program usage from command line
  if(argc < 2) //QQQ!!! Alex: change to != 2
  {
    cout << "\nUsage:\ntester <source_file>\n Exiting.\n" << endl;
    return -1;
  }  
  
  //fill strings with proper names
  prog_cpp = argv[1];
  progname = prog_cpp.substr(0,prog_cpp.find("."));
  progcomp = "g++ -o " + progname  + " " + prog_cpp;
  size_t found = prog_cpp.find_last_of("/\\");// QQQ!!! Alex : why not just ints?
  progdir = progname.substr(0,found+1);
  
  //compile program to be tested
  system(progcomp.c_str());

  //QQQ!!! Alex: inserting here for new functionality
  string ans;
  do
  {
    cout << "Use only existing test cases?" << endl;
    cin >> ans;
    transform( ans.begin(), ans.end(), ans.begin(), ::tolower);
    if (ans.compare("n") == 0 || ans.compare("no") == 0 )
    {
      generatetestcases();
      break;
    }
    else if (ans.compare("y") == 0 || ans.compare("yes") == 0 )
    {
      break;
    }
  }while (1);


  //gathers all of the .tst files in current and sub directories of the program
  // being tested
  vector<string> testcases;
  testcases = find_tsts(progdir);
   
  /*while more .tst files need ran, continue running the tests against the
  program*/
  for(int i=0;i<testcases.size();i++)
  {
    //running the test and capturing results
    string results = runtests(progname, testcases.at(i));
    
    //making sure the runtests() function executed successfully 
    if(results != "files did not open for comparison\n")
    {
      //storing the results of the test into finaloutfilecontents
      finaloutfilecontents.push_back(results);
    }
  }
  
  //writing all of the results to the .out file
  writefinaloutfile(progname, finaloutfilecontents);  
  
  //deleting the temp file
  remove("temp.txt");
  
  //exit program
  return 0;
}
/********************************** END main **********************************/


//QQQ!!! Alex : testcase builder starts here
void generatetestcases()
{
  bool doubles, lesserThanAmount, greaterThanAmount;
  double min, max;
  long long inserts;
  int amountToGenerate, filesToMake;
  ofstream fout;
  string file;
  stringstream temp;
  generatetestcasesmenu(doubles, lesserThanAmount, greaterThanAmount, min, 
                        max, amountToGenerate, filesToMake);

  // if max and min are both max (none selected, use 0,1 as it leaves rand)

  // seed random
  srand (time(NULL));

  // if doubles
  if (doubles)
  {
   // make each file
    for (int i = 0 ; i < filesToMake; i +=1)
    {
      // make string of file name
      temp.str("");
      temp << i;
      file = "GeneratedTestCase" + temp.str() + ".tst";
      fout.open(file.c_str());

      // populate 
      if (lesserThanAmount)
      {
        inserts = rand() % amountToGenerate;
        // if 0, run again
        if (inserts == 0)
        {
          inserts = rand() % amountToGenerate;
        }
      }
      else if (greaterThanAmount)
      {
        inserts = rand() % amountToGenerate * 5 + amountToGenerate;
        // if amount... run again
        if (inserts == amountToGenerate)
        {
          inserts = rand() % amountToGenerate * 5 + amountToGenerate;
        }
      }
      else
      {
        inserts = amountToGenerate;
      }
      for (int j = 0; j < inserts; j +=1)
      {
        // threshold     offset to min   by random fraction   times the difference
        double inValue = min + ((double) rand() / RAND_MAX) * (max - min);
        fout << inValue << endl;
      }

      fout.close();
    }
  }

  else
  {
    // make for ints
    for (int i = 0 ; i < filesToMake; i +=1)
    {
      // make string of file name
      temp.str("");
      temp << i;
      file = "GeneratedTestCase" + temp.str() + ".tst";
      fout.open(file.c_str());
      
      // populate 
      if (lesserThanAmount)
      {
        inserts = rand() % amountToGenerate;
        // if 0, run again
        if (inserts == 0)
        {
          inserts = rand() % amountToGenerate;
        }
      }
      else if (greaterThanAmount)
      {
        inserts = rand() % amountToGenerate * 5 + amountToGenerate;
        // if amount... run again
        if (inserts == amountToGenerate)
        {
          inserts = rand() % amountToGenerate * 5 + amountToGenerate;
        }
      }
      else
      {
        inserts = amountToGenerate;
      }
      // works for ints too
      for (int j = 0; j < inserts; j +=1)
      {
        // threshold  offset to min   by random fraction   times the difference
        int inValue = (int) (min + ((double) rand() / RAND_MAX) * (max - min));
        fout << inValue << endl;
      }

      fout.close();
    }

  }

}



//QQQ!!! Alex : testcase builder menu is here... long...
void generatetestcasesmenu(bool &doubles, bool &lesserThanAmount, 
                           bool &greaterThanAmount, double &min, double &max, 
                           int &amountToGenerate, int &filesToMake)
{
  string input = "";
  bool range;

  //make welcome menu
  cout << "\nWelcome to the test-case generator!\n" << endl;
  cout << "\033[1;34mbold Press 'x' and enter at any time to leave.\033[0m\n";

  // start by asking about type
  while(1)
  {
    cout << "What type of data would you like?  Ints or Doubles?"  << endl;
    cin >> input;
    transform( input.begin(), input.end(), input.begin(), ::tolower); 
    if (!input.compare("int") || !input.compare("ints") || !input.compare("i"))
    {
      doubles = false;
      break;
    }   
    else if (!input.compare("double") || !input.compare("doubles") 
             || !input.compare("d"))
    {
      doubles = true;
      break;
    }
    else if (!input.compare("x"))
    {
      return; // change of heart
    }
    else
    {
      cout << "\nI did not understand.  Please re-enter." << endl;
    }
  }

  // count of test cases?
  while(1)
  {
    cout << "How many test cases would you like?"  << endl;
    cin >> input;
    filesToMake = atoi(input.c_str());
    if (filesToMake >= 0)
    { 
      break;
    }
    else if (!input.compare("x"))
    {
      return; // change of heart
    }
    else
    {
      cout << "\nPlease re-enter as a positive number or 'x' to cancel." << endl;
    }
  }

  // specific amount to work with?
  while(1)
  {
    cout << "Would you like a specific amount of items to test?"  << endl;
    cin >> input;
    transform( input.begin(), input.end(), input.begin(), ::tolower);
    if (!input.compare("y") || !input.compare("yes"))
    {
      lesserThanAmount = greaterThanAmount = false;
      break;
    }
    else if (!input.compare("n") || !input.compare("no"))
    {
      lesserThanAmount = greaterThanAmount = true;
    }
    else if (!input.compare("x"))
    {
      return; // change of heart
    }
    else
    {
      cout << "\nPlease re-enter." << endl;
    }
  }
  
  // if not
  if (lesserThanAmount)
  {
    while(1)
    {
      cout << "More or less than a specific value?"  << endl;
      cin >> input;
      transform( input.begin(), input.end(), input.begin(), ::tolower);
      if (!input.compare("<") || !input.compare("less"))
      {
        greaterThanAmount = false;
        break;
      }
      else if (!input.compare(">") || !input.compare("more"))
      {
        lesserThanAmount = false;
      }
      else if (!input.compare("x"))
      {
        return; // change of heart
      } 
      else
      {
        cout << "\nPlease re-enter." << endl;
      }
    }
  }

  // value of items in tests
  while(1)
  {
    cout << "What value should I work with?"  << endl;
    cin >> input;
    amountToGenerate = atoi(input.c_str());
    if (amountToGenerate > 0)
    {
      break;
    }
    else if (!input.compare("x"))
    {
      return; // change of heart
    }
    else
    {
      cout << "\nPlease re-enter value greater than 0." << endl;
    }
  }

  //range
  do
  {
    cout << "range of values?" << endl;
    cin >> input;
    transform( input.begin(), input.end(), input.begin(), ::tolower);
    if (input.compare("n") == 0 || input.compare("no") == 0 )
    {
      range = false;
      break;
    }
    else if (input.compare("y") == 0 || input.compare("yes") == 0 )
    {
      break;
    }
  }
  while (1);
  
  if (range)
  {
    cout << "What minimum value should I work with?"  << endl;
    cin >> input;
    min = atoi(input.c_str());

    cout << "What max value should I work with?"  << endl;
    cin >> input;
    max = atoi(input.c_str());
  }
  else
  {
    // set min and max to max
    min = LONG_MIN;
    max = LONG_MAX; 
  }
}



/********************************* find_tsts **********************************/
// Locates all .tst files to be ran against the program to be tested
/******************************************************************************/ 
vector<string> find_tsts(string progdir)
{
    vector<string> tstfilelist;
    tstfilelist.clear();    
    
    string popencommand = "find "+progdir+" -name '*.tst'";
    FILE * f = popen( popencommand.c_str(), "r" );
   
    const int BUFSIZE = 1000;
    char buf[ BUFSIZE ];
    int i = 0;
    while( fgets( buf, BUFSIZE,  f ) ) {
        tstfilelist.push_back(buf);
    }
    pclose( f );
    
    //removing that frustrating invisible character at the end of the strings
    for(int i=0;i<tstfilelist.size();i++)// QQQ!!! Alex: clearer way to do this?
    {
      tstfilelist.at(i).replace(tstfilelist.at(i).end()-1,
      tstfilelist.at(i).end(),"");
    }
    
    return tstfilelist;
}
/******************************* END find_tsts ********************************/ 



/********************************** runtests **********************************/
// Runs all of the .tst test cases against the program one at a time
//  and returns the results of that particlar test, stored in a string
/******************************************************************************/
string runtests(string progname, string specifictestcase)
{ 
  string testresult;
  
  //temporary file used to compare results
  string tempfile = "temp.txt";
  
  /*building the string to run the program, adds the appropriate .tst file for
   input to the program and the temp#.txt file as the output of the program 
   being tested*/
  string progrun = "./"+progname+" < "+specifictestcase+" > "+tempfile;
  
  //running the program
  system(progrun.c_str());
  
  //building string for the .ans file name to compare against
  string testcaseans = specifictestcase;
  testcaseans.replace(testcaseans.end()-4, testcaseans.end(),".ans"); 
    
  //comparing answer to the correct answer and storing the results
  int nodifference = filesequal(testcaseans, tempfile);
  if(nodifference == 0)
  {
    testresult = "Result: pass    Case: " + specifictestcase; 
  }
  else if(nodifference ==1)
  {
    testresult = "Result: fail    Case: " + specifictestcase;
  }
  else
  {
    testresult = "files did not open for comparison";
  }
      
  return testresult;
}
/******************************* END runtests *********************************/



/********************************* filesequal *********************************/
// compares two files and returns 1 if not equal, 0 if equal
/******************************************************************************/ 
int filesequal(string file1name, string file2name)  // QQQ!!! Alex: used as boolean,
// so change return type or make clear that return is 0 for false (based on name)
{
  ifstream file1, file2;

  //opening the two files to compare and ensuring they open properly
  file1.open( file1name.c_str(), ios::in ); // QQQ!!! Alex: doesn't infile usualy
  file2.open( file2name.c_str(), ios::in ); // defualt to in ios?
  if (!file1)
  {
    cout << "Couldn't open the file  " << file1name <<endl;
    file2.close();
    return 2;
  }
  if (!file2)
  {
    file1.close();
    cout << "Couldn't open the file " << file2name << endl;
    return 2;
  }
    
  //read the two files into two string vectors
  vector<string> string1, string2;
  int i = 0;
  while(file1.good())  // QQQ!!! Alex : change this to diff? or leave at io heavy?
  {
    string1.push_back("");
    getline(file1,string1.at(i)); 
    i++;
  }
  i = 0;
  while(file2.good())
  {
    string2.push_back("");
    getline(file2,string2.at(i)); 
    i++;
  }

  //close the two files being compared
  file1.close();
  file2.close();
    
  //first testing if the files had same amount of lines
  if(string1.size() == string2.size())
  {
    //if both had same amount of lines, test actual contents of each line
    for(int i=0;i<string1.size();i++)
    {
      //removing the VERY frustrating occasional \r in the string
      string1.at(i) = string1.at(i).substr(0,string1.at(i).find("\r"));
      string2.at(i) = string2.at(i).substr(0,string2.at(i).find("\r"));
 
      //compare the contents of the two lines.  if equal, return 0, else 1
      if(strcmp(string1.at(i).c_str(),string2.at(i).c_str()) == 0)
      {
        //move on to next string comparison
      }
      else 
      {
        return 1;
      }
    }
    return 0;
  }
  else
  {
    return 1; //files had different amount of lines, thus not equal
  }
}
/******************************* END filesequal *******************************/



/***************************** writefinaloutfile ******************************/
// Writes all of the data to the final .log file
/******************************************************************************/   
void writefinaloutfile(string progname, vector<string> finaloutfilecontents)
{
  //counter to calculate summary of tests
  int totalpassed = 0;
  
  //getting current date and time for filename
  time_t rawtime;
  struct tm * timeinfo;
  char buffer [16];
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,16,"%m_%d_%H:%M:%S",timeinfo);
  
  //string to hold the final output file name
  string outfilename = progname+"_"+buffer+".log";
  
  //opening final output file
  ofstream fout;
  fout.open(outfilename.c_str());
  
  //writing the contents to the output file
  for(int i=0;i<finaloutfilecontents.size();i++)
  {
    //counting the number of passed tests for computing the summary
    if(finaloutfilecontents.at(i).find("pass") != string::npos)
      totalpassed++;
      
    fout << finaloutfilecontents.at(i) << endl;
  }
  
  //printing out the final summary of the tests to the output file
  fout << 
  "\nTest Summary  \n" 
    "     Total # of tests ran:    " << finaloutfilecontents.size() << 
  "\n     Total # of tests passed: " << totalpassed <<
  "\n              Percent passed: " << 
  ((((float)totalpassed)*100)/finaloutfilecontents.size())  << "%" << endl;
  
  
  //closing the output file
  fout.close();
}
/*************************** END writefinaloutfile ****************************/