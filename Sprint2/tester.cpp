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

/*******************************************************************************
  Tester.cpp (Version 2.0.0)
    CSC 470 Tester Program
    Date: March 23 2014
    Authors: Joseph Manke, Adam Meaney and Alex Wulff
    
    Usage:
      -This program expects to find all program source files to test in 
       subdirectories of exectution.  This program also expects a golden or 
       truth program's source to be located at the level of execution and all 
       pre-built test files at the execution level or below.
      -The program offers a text menu for test case generation and immediately
       prompts the user at the start of the program.  The generator will 
       terminate execution of the program and will require running again to
       use the new test cases.
      -Results of the tests will be listed in a .log file in the same directory
       as all test programs when ran as well as the level of execution for a 
       quick view of performance.  Any tests that fail a critical test as noted
       by "*_crit.tst" will be awarded no points on earlier success and will 
       fail.
        
    Updates:
       All contributors of Lounge Against The Machine have noted their 
       contributions and comments on the original by noting the start of their
       changes with a //QQQ!!! <editor> : <comments> format.
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
#include <unistd.h>
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <dirent.h>


using namespace std;

//Function Prototypes
//QQQ!!! Alex : dropped for new prototype : string runtests(string progname, string specifictestcase);
int runtests(string prog, string specifictestcase);
void writefinaloutfile(vector<string> finaloutfilecontents);//QQQ!!! Alex : commented out new processing method
//QQQ!!! Alex : made global : string progname, vector<string> finaloutfilecontents);
void find_students(string directory, int level);
vector<string> find_tsts(string progdir);
string Generate_Performance_Report(string file, int score, int total);
int filesequal(string file1name, string file2name);
void generatetestcases();
void generatetestcasesmenu(bool &doubles, bool &lesserThanAmount, 
                           bool &greaterThanAmount, double &min, double &max, 
                           int &amountToGenerate, int &filesToMake);
void solvetestcases();
void cleanup();
void pregenerateclean();
void writeindividualreport(string program, string testcase, int success);
void generateanswers();

/********************************Defines*************************************/
int MININT = -2147483647;
/****************************************************************************/

/*********************************GLOBALS************************************/
vector<string> STUDENTVECTOR;
vector<string> TESTCASES;
string GOLDCPP;
int TOTALPASSED;
/****************************************************************************/

/*********************************** main ***********************************/
//  Primary function of the program
/****************************************************************************/
int main(int argc, char* argv[])
{
  string progname, prog_cpp, progcomp, progdir;
  
  //holds each test and result on a separate line
  vector<string> finaloutfilecontents;
  finaloutfilecontents.clear();
  TOTALPASSED = 0;
  //test for proper program usage from command line
  if(argc != 1) //QQQ!!! Alex: change to != 2
  {
    cout << "\nUsage:\ntester <source_file>\n Exiting.\n" << endl;
    return -1;
  }  

  // QQQ!!! Alex : make gold cpp = ""
  GOLDCPP.clear();
  
  //fill strings with proper names

  // QQQ!!! Alex: moved to runtests to run on each test 
/*  prog_cpp = argv[1];
  progname = prog_cpp.substr(0,prog_cpp.find("."));
  progcomp = "g++ -o " + progname  + " " + prog_cpp;
  size_t found = prog_cpp.find_last_of("/\\");// QQQ!!! Alex : why not just ints?
  progdir = progname.substr(0,found+1);

  //compile program to be tested
  system(progcomp.c_str());*/

  char dir[1024];
  getcwd(dir, sizeof(dir));
  string loc (dir);

  TESTCASES = find_tsts(progdir);

  find_students(loc, 0);
  //QQQ!!! Alex: inserting here for new functionality
  string ans;
  do
  {
    cout << "\nGenerate new test cases?" << endl;
    cin >> ans;
    transform( ans.begin(), ans.end(), ans.begin(), ::tolower);
    if (ans.compare("y") == 0 || ans.compare("yes") == 0 )
    {
      // make tests
      generatetestcases();
      cout << "\nTest generation completed\n\n";
      // find all tests and use generated tests to make ans
      TESTCASES = find_tsts(progdir);
      find_students(loc, 0);
      generateanswers();
      // clean  
      cleanup();
      return 0;
    }
    else if (ans.compare("n") == 0 || ans.compare("no") == 0 )
    {
      break;
    }
  }while (1);

  // QQQ!!! Alex : gathers all of the .tst files in current and sub directories of the program
  // being tested
  // vector<string> testcases;
  TESTCASES = find_tsts(progdir);

  // QQQ!!! Alex: get the testcases
  find_students(loc, 0);
   
  // QQQ!!! Alex : while more .tst files need ran, continue running the tests against the
  //program
  int score = 0;
  vector<string> performance;
  string currentProg;


  // QQQ!!! Alex : foreach program (edited to end of main)
  for (int h = 0; h < STUDENTVECTOR.size(); h+=1)
  {
    score = 0;
    // and for each test case
    for(int i=0;i<TESTCASES.size();i++)
    {
/* QQQ!!! Alex: deprecating this and reworking runtests to return 0 fail, 1 pass 

    //running the test and capturing results
    string results = runtests(progname, testcases.at(i));
    
    //making sure the runtests() function executed successfully 
    if(results != "files did not open for comparison\n")
    {)
      //storing the results of the test into finaloutfilecontents
      finaloutfilecontents.push_back(results);
    }
*/
      int result = runtests(STUDENTVECTOR[h], TESTCASES.at(i));
      string current = TESTCASES.at(i);
      // failure on critical test
      if (result == 0 && current.substr(current.length() - 8)
                                .find("crit.tst") != -1 )
      {
        score = -1;
        break; // stop tests
      }
      if (result == 1)
      {
        score += 1;
        TOTALPASSED +=1;
      }
      writeindividualreport(STUDENTVECTOR[h], TESTCASES.at(i), result);
    }
    // QQQ!!! Alex : get report on this program
    currentProg = Generate_Performance_Report(STUDENTVECTOR[h], score, TESTCASES.size());
    finaloutfilecontents.push_back(currentProg);
  }

  //writing all of the results to the .out file
  writefinaloutfile(finaloutfilecontents);//QQQ!!! Alex : progname, finaloutfilecontents);  
  
  // clean up globals
  cleanup();
  //deleting the temp file
  //remove("temp.txt");
  
  //exit program
  return 0;
}
/********************************** END main **********************************/

/************************Generate_Performance_Report***************************/
// QQQ!!! Alex: built to write in the specifics of success and failures per cpp
/******************************************************************************/
string Generate_Performance_Report(string file, int score, int total)
{
  int lastDir = file.rfind("/");
  string report = file.substr(lastDir + 1);
  if (score == -1)
  {
    return report + ":  FAILED";
  }
  
  stringstream temp("");
  double percent = ((double) score / total) * 100;
  temp << percent;
  return report + ":  " + temp.str() + "%";
}

/******************************generatetestcases*****************************/
//QQQ!!! Alex : testcase builder starts here
/****************************************************************************/
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
  double inValue;
  unsigned short drandSeed = 128;

  // if max and min are both max (none selected, use 0,1 as it leaves rand)

  // seed random
  srand (time(NULL));
  seed48(&drandSeed);

  // if doubles
  if (doubles)
  {
   // make each file
    for (int i = 0 ; i < filesToMake; i +=1)
    {
      // make string of file name
      temp.str("");
      temp << i;
      file = "./tests/GeneratedTestCase" + temp.str() + ".tst";
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
        if (max != 1)
        {
          inValue = (min + (rand() % (int)max) + 1) * drand48();
        }
        else
        {
          inValue = (MININT * rand()) + (rand() + rand()) * drand48();
        }
         
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
        inserts = rand() % (amountToGenerate * 5 + amountToGenerate);
        // if amount... run again
        if (inserts == amountToGenerate)
        {
          inserts = rand() % (amountToGenerate * 5 + amountToGenerate);
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
        //int inValue = (int) (min + ((double) rand() / RAND_MAX) * (max - min));
        if (max != 1 && min != 0)
        {
          inValue = (int (min) + rand()) % int(max) + 1;
        }
        else
        {
          inValue = MININT + rand() + rand(); // Max at randmax, min at LONG_MIN
        }        

        fout << (int) inValue << endl;
      }

      fout.close();
    }

  }

}


/****************************generatetestcasemenu****************************/
//QQQ!!! Alex : testcase builder menu is here... long...
/****************************************************************************/
void generatetestcasesmenu(bool &doubles, bool &lesserThanAmount, 
                           bool &greaterThanAmount, double &min, double &max, 
                           int &amountToGenerate, int &filesToMake)
{
  string input = "";
  bool range;

  // defaults for min and max
  min = 0;
  max = 1;

  //make welcome menu
  cout << "\nWelcome to the test-case generator!\n" << endl;
  cout << "\033[1;34mbold Press 'x' and enter at any time to leave.\033[0m\n";

  while(1)
  {
    // inquire about cleaning old tests or overwrite?
    cout << "\nRemove old generated tests (y) or just overwrite as needed (n)?" << endl;
    cin >> input;
    transform( input.begin(), input.end(), input.begin(), ::tolower);
    if (!input.compare("y") || !input.compare("yes"))
    {
      cout << "\n\tClearing..." << endl;
      pregenerateclean();
      break;
    }
    else if (!input.compare("n") || !input.compare("no"))
    {
      break;
    }
    cout << "I didn't understand.  Please try again." << endl;
  }

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
      cout << "Exiting.  Please run again." << endl;
      exit(0); // change of heart
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
      cout << "Exiting.  Please run again." << endl;
      exit(0); // change of heart
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
      cout << "Exiting.  Please run again." << endl;
      exit(0); // change of heart
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
        cout << "Exiting.  Please run again." << endl;
        exit(0); // change of heart
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
    else if (!input.compare("x") || !input.compare("X"))
    {
      cout << "Exiting.  Please run again." << endl;
      exit(0); // change of heart
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
    else if (!input.compare("x") || !input.compare("X"))
    {
      cout << "Exiting.  Please run again." << endl;
      exit(0); // change of heart
    }
    else if (input.compare("y") == 0 || input.compare("yes") == 0 )
    {
      range = true;
      break;
    }
  }
  while (1);
  
  if (range)
  {
    cout << "What minimum value should I work with?"  << endl;
    cin >> input;
    if (!input.compare("x") || !input.compare("X"))
    {
      cout << "Exiting.  Please run again." << endl;
      exit(0); // change of heart
    }
    min = atof(input.c_str());

    cout << "What max value should I work with?"  << endl;
    cin >> input;
    if (!input.compare("x") || !input.compare("X"))
    {
      cout << "Exiting.  Please run again." << endl;
      exit(0); // change of heart
    }
    max = atof(input.c_str());
  }
  
}

/******************************find_students********************************/
// QQQ!!! Alex : built to directory crawl and find all student files and 
// avoid duplicates and the golden cpp (praesumo presumo)
/****************************************************************************/
void find_students(string directory, int level)
{
  string temp(directory);
  DIR *dir = opendir(temp.c_str()); // open the current directory
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
        if ( (length > 4 && (temp.substr(length-4) == ".cpp") 
             || temp.substr(length-2) == ".C")
             && level > 0 )
        {
          // because I'm getting duplicates
          string insert = directory + '/' + temp;
          if (find(STUDENTVECTOR.begin(), 
              STUDENTVECTOR.end(), 
              insert) == STUDENTVECTOR.end() 
              && level != 0) // if not found
          {
            STUDENTVECTOR.push_back(insert);
          }
          else if (find(STUDENTVECTOR.begin(),
              STUDENTVECTOR.end(),
              insert) == STUDENTVECTOR.end() 
              && GOLDCPP.empty() 
              && level == 0)
          {
            GOLDCPP = directory + '/' + temp;
          }
        }
        else
        {
          if (GOLDCPP.empty()
              && ((temp.substr(length-4) == ".cpp")
              || temp.substr(length-2) == ".C")
              && level == 0)
          {
            GOLDCPP = directory + '/' + temp;
          }

          find_students(directory + '/' + temp, level + 1);
        }
      }
    }
  }

  closedir(dir);

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
    for(int i=0;i<tstfilelist.size();i++)
    {
      tstfilelist.at(i).replace(tstfilelist.at(i).end()-1,
      tstfilelist.at(i).end(),"");
    }

    // QQQ!!! Alex: iterate over tstfilelist and put all crit.tst in the 
    // front of list
    for(int i=0;i<tstfilelist.size();i++)
    {
      string temp = tstfilelist.at(i);
      if (!temp.substr(temp.length() - 8).compare("crit.tst"))
      {
        tstfilelist.erase(tstfilelist.begin() + i);
        tstfilelist.insert(tstfilelist.begin(), temp);
      }
    }
    
    return tstfilelist;
}

/******************************* END find_tsts ********************************/ 



/********************************** runtests **********************************/
// Runs all of the .tst test cases against the program one at a time
//  and returns the results of that particlar test, stored in a string

// QQQ!!! Alex : modified here to compile and execute each test with each 
// specified test case as part of the "foreach" loop in main.
/******************************************************************************/
//string runtests(string progname, string specifictestcase)
int runtests(string prog, string specifictestcase)
{ 
  string testresult;
  
  // compile each
  string prog_cpp = prog;
  string progname = prog_cpp.substr(0,prog_cpp.find("."));
  string progcomp = "g++ -o " + progname  + " " + prog_cpp;
  size_t found = prog_cpp.find_last_of("/\\");

  //compile program to be tested
  system(progcomp.c_str());

  //temporary file used to compare results
  string tempfile = "temp.txt";
  
  /*building the string to run the program, adds the appropriate .tst file for
   input to the program and the temp#.txt file as the output of the program 
   being tested*/
  string progrun = progname+" < "+specifictestcase+" > "+tempfile;
  
  //running the program
  system(progrun.c_str());
  
  //building string for the .ans file name to compare against
  string testcaseans = specifictestcase;
  testcaseans.replace(testcaseans.end()-4, testcaseans.end(),".ans"); 
    
  //comparing answer to the correct answer and storing the results
  int nodifference = filesequal(testcaseans, tempfile);
  if(nodifference == 0)
  {
    // QQQ!!! Alex : changing... testresult = "Result: pass    Case: " + specifictestcase; 
    return 1;
  }
  else if(nodifference ==1)
  {
    // QQQ!!! Alex : changing... testresult = "Result: fail    Case: " + specifictestcase;
    return 0;
  }
  else
  {
    // QQQ!!! Alex : changing... testresult = "files did not open for comparison";
    return -1;
  }
      
  // QQQ!!! Alex : changing... return testresult;
}
/******************************* END runtests *********************************/



/********************************* filesequal *********************************/
// compares two files and returns 1 if not equal, 0 if equal

// QQQ!!! Alex : this was causing huge performance hits. Reworked to a dif check
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
  if (!file2){
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
void writefinaloutfile(vector<string> finaloutfilecontents)//QQQ!!! Alex : commented out new processing method
//                               string progname, vector<string> finaloutfilecontents)
{  
  //getting current date and time for filename
  time_t rawtime;
  struct tm * timeinfo;
  char buffer [16];
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,16,"%m_%d_%H:%M:%S",timeinfo);
  //string to hold the final output file name
  string outfilename (buffer); //QQQ!!! Alex : just log and time //+ logprogname+"_"+buffer+".log";
  
  outfilename = "log " + outfilename;

  //opening final output file
  ofstream fout;
  fout.open(outfilename.c_str());

  fout << "\nTest Summary:\n\n";

  
  //writing the contents to the output file
  for(int i=0;i<finaloutfilecontents.size();i++)
  {      
    fout << finaloutfilecontents.at(i) << endl;
  }
  
  //printing out the final summary of the tests to the output file
  fout <<  
  "     Total # of tests ran:    " << 
  TESTCASES.size()*STUDENTVECTOR.size() << 
  "\n     Total # of tests passed: " << TOTALPASSED <<
  "\n              Percent passed: " << 
  (((float)TOTALPASSED)/(TESTCASES.size()*STUDENTVECTOR.size())) * 100 << 
  "%" << endl;
  
  
  //closing the output file
  fout.close();
}
/*************************** END writefinaloutfile ****************************/

/***********************************cleanup**********************************/
// QQQ!!! Alex : cleans up the globals
/****************************************************************************/
void cleanup()
{
  STUDENTVECTOR.erase(STUDENTVECTOR.begin(), STUDENTVECTOR.end());
  TESTCASES.erase(TESTCASES.begin(), TESTCASES.end());  
}

/****************************pregenerateclean********************************/
// QQQ!!! Alex : removes old generated test cases
/****************************************************************************/
void pregenerateclean()
{
  system("rm ./tests/GeneratedTestCase*");
}

/******************************writeindividualreport*************************/
// QQQ!!! Alex : writes the specifics of each program's performance with a 
// given test
/****************************************************************************/

void writeindividualreport(string program, string testcase, int success)
{
  string file = program + ".log";
  ofstream fout(file.c_str(), fstream ::app); // append
  if (success > 0) // if passed
  {
    fout << "passed: " << testcase << endl;
  }
  else
  {
    fout << "failed: " << testcase << endl;
  }
  fout.close();
}

/*******************************generateanswers******************************/
// QQQ!!! Alex : needed to generate answers to new tests using the golden cpp.
/****************************************************************************/

void generateanswers()
{
  // compile golden cpp
  string cmd = "g++ -o " 
               + GOLDCPP.substr(0,GOLDCPP.rfind(".cpp")) 
               + " " 
               + GOLDCPP;
  system(cmd.c_str());
  string programName = GOLDCPP.substr(GOLDCPP.rfind("/") + 1);
  programName = programName.substr(0,programName.find(".cpp"));

  // look at all test cases
  for (int i = 0 ; i < TESTCASES.size() ; i +=1)
  {
    // if it's a generated test case
    if (TESTCASES[i].find("GeneratedTestCase") != -1)
    {
      cmd = "./" + programName 
            + " < " + TESTCASES[i].substr(2) + " > " 
            + " " + TESTCASES[i].substr(2,TESTCASES[i].rfind(".tst") -2)
            + ".ans";
      system(cmd.c_str());
    }
  }
}
