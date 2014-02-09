/*************************************************************************//**
 * @file 
 *
 * @mainpage Program 1 - The Proc File system
 * 
 * @section course_section CSC 465
 *
 * @author Alex Wulff
 * 
 * @date January 27, 2014
 * 
 * @par Professor: 
 *         Dr. Christer Karlsson
 * 
 * @par Course: 
 *         CSC 456 - M001 - 2:00pm
 * 
 * @par Location: 
 *         McLaury - 313
 *
 * @section program_section Program Information 
 * 
 * @details This program simulates signals sent to various process id's,
 * command name recognition for the linux /proc directory, and system
 * specific build information interpretation.  This is used as a stepping
 * stone to further understand the complexity and introcate nature of 
 * Operating Systems.
 *
 * @section compile_section Compiling and Usage 
 *
 * @par Compiling Instructions: 
 *      g++ -o dsh dsh.C -g or make 
 * 
 * @par Usage: 
   @verbatim  
   ./dsh
   @endverbatim 
 *
 * @section todo_bugs_modification_section Todo, Bugs, and Modifications
 * 
 * @bug None Discovered 
 * 
 * @todo Make text entry behave like scripts - i.e. handle arrows as well as 
 *  ctrl + l and such.
 * 
 * @par Modifications and Development Timeline: 
   @verbatim 
   Date          Modification 
   ------------  -------------------------------------------------------------- 
   Jan 20, 2014    Started frame work (created the header, function prototypes, etc.).

   Jan 23, 2014    Set head method as discussed in class.

   Jan 25, 2014    Converted to C++ as string manipulation got tiresome.

   Jan 27, 2014    Set up Signal, Systat and Cmdnm methods.  Did Documentation.

   Jan 28, 2014    Added pid command and error checking for signals > 64.

   Jan 30, 2014    Realized I couldn't do system() function calls, so re-worked.
   @endverbatim
 *
 *****************************************************************************/

/*************************************************************************//**
*********************************INCLUDES*************************************
******************************************************************************/
 
#include <iostream>
#include <string>
#include <stdlib.h>
#include <csignal>
#include <sstream>
#include <fstream>
#include <dirent.h>

/*************************************************************************//**
*********************************NAMESPACE************************************
******************************************************************************/

using namespace std;

/*************************************************************************//**
********************************PROTOTYPES************************************
******************************************************************************/

void Command_Name(int pid);
int Command_Parse(string in, int *type, int *pid, 
                  int *signal, string &command);
int Error_Check(int argc, char** argv);
int Loop();
void Main_Menu();
void ProcessLookup(int flag, string cmd);
void Signal(int signal, int pid);
void Signaled(int signal);
string SigType(int a);
void Systat(int p);
void Usage_Menu();
void WriteFile(string s);
void WriteProcCmd(int pid);
void WriteCmdProc(string s);


struct dirent *item;


/**************************************************************************//**
 * @author Alex Wulff
 *
 * @par Description:
 * This is the starting point to the program.  It simply subscribs to signal
 * notifications and checks for errors at the command line.  Should one be 
 * encountered, an error message will be given to the user encouraging 
 * another attempt.
 *
 * @param[in] argc - the number of arguments from the command prompt.
 * @param[in] argv - a 2d array of characters containing the arguments.
 *
 * @returns 0 - upon normal completion.
 * @returns 1 - failed input arguments on the command line.
 *
 *****************************************************************************/

int main(int argc, char ** argv)
{
  signal(SIGINT, Signaled);
  if (!Error_Check(argc, argv))
  {
    Usage_Menu();
    return 1;
  }
  while (Loop());

  return 0;
}

/**************************************************************************//**
 * @author Alex Wulff
 *
 * @par Description:
 * This function uses the bash head command to find the run command for a
 * particular command as specified by pid.
 *
 * @param[in] pid - process id as specified by the user.
 *
 * @returns none.
 *
 *****************************************************************************/

void Command_Name(int pid)
{
  cout << endl;
  //cout <<"cmdnm"<< endl << endl;
  stringstream p;
  p << pid;
  //string s = "head /proc/" + p.str() + "/cmdline";
  //system(s.c_str());
  WriteProcCmd(pid);
  cout << endl;
}

/**************************************************************************//**
 * @author Alex Wulff
 *
 * @par Description:
 * This function determines the behavior as specified by the user at the
 * menu prompt.  It will after decyphering return any applicable arguments
 * needed for later calculations.
 *
 * @param[in] in - string command with both command and any other arguments.
 * @param[out] type - int used for later enumeration.
 * @param[out] pid - int used for process id.
 * @param[out] signal - int used for specific signal.
 * @param[out] command - string used for pid command lookup.
 *
 * @returns 0 - if the string in has a bad structure.
 * @returns 1 - in string is correctly processable.
 *
 *****************************************************************************/

int Command_Parse(string in, int &type, int &pid, int &signal, string &command)
{
  int target;
  int index;
  string s(in);
  //determine command use as exclusive
  // cmdnm
  if (s.find("cmdnm") != -1 && s.find("signal") == -1 && 
      s.find("systat") == -1 && s.find("exit") == -1 && 
      s.find("pid") == -1)
  {
    // save type
    type = 0;
    // get pid
    target = s.find(' ');
                         // watch for command-space-null strings
    if ( target != -1 && (target + 1) < s.length())
    {
      index = target + 1;
      // make substring
      string s2 = s.substr(index);
      // get next space if exists
      target = s2.find( ' ');
      // if there was a space and it's not at the end of the input, error
      if (target == -1 && (target + 1) == s.length())
      {
        return 0;
      }
      // if no space
      if (target == -1)
      {
        pid = atoi(s2.c_str());
        return 1;
      }
      index = target;
      // get number
      s2.substr(0,index);
      // null term on space
      pid = atoi(s2.c_str());
      return 1;
    }
    else // bad line
    return 0;
  }
  // signal
  else if (s.find("cmdnm") == -1 && s.find("signal") != -1 && 
           s.find("systat") == -1 && s.find("exit") == -1 && 
           s.find("pid") == -1)
  {
    // save type
    type = 1;
    // get signal
    target = s.find(' ');
                         // watch for command-space-null strings
    if ( target != -1 && (target + 1) < s.length())
    {
      index = target + 1;
      // make substring
      string s2 = s.substr(index);
      // get next space if exists
      target = s2.find(' ');
      // if there was a space and it's not at the end of the input, error
      if (target == -1 && (target + 1) == s.length())
      {
        return 0;
      }
      index = target;
      // get signal
      signal = atoi(s2.substr(0,index).c_str());
      // now get pid
      if (index >= s2.length() || signal > 64)
      {
	return 0;
      }
      string s3 = s2.substr(index);
      index = s3.find(' ');
      if (index != -1 && index != 0 && index != s3.length())
      {
	return 0;
      }
      pid = atoi(s3.c_str());
      return 1;
    }
    else // bad line
    return 0;
  }
  // systat
  else if (s.find("cmdnm") == -1 && s.find("signal") == -1 &&
           s.find("systat") != -1 && s.find("exit") == -1 && 
           s.find("pid") == -1)
  {
    pid = 0;
    type = 2;
    // check for process request
    target = s.find(' ');
                         // watch for command-space-null strings
    if ( target != -1 && (target + 1) < s.length())
    {
      index = target + 1;
      // make substring
      string s2 = s.substr(index);
      // get next space if exists
      if (s2.find('p') != -1)
      {
	pid = 1;  
      }	  
      return 1;
    }
     return 1;
  }

  // exit
  else if (s.find("cmdnm") == -1 && s.find("signal") == -1 &&
           s.find("systat") == -1 && s.find("exit") != -1 && 
           s.find("pid") == -1)
  {
    type = 3;
    return 1;
  }
  // Process ID Request
  else if (s.find("cmdnm") == -1 && s.find("signal") == -1 &&
           s.find("systat") == -1 && s.find("exit") == -1 &&
           s.find("pid") != -1)
  {
    type = 4;
    pid = 0;
    // check for process request
    target = s.find(' ');
                         // watch for command-space-null strings
    if ( target != -1 && (target + 1) < s.length())
    {
      index = target + 1;
      // get command substring
      string s2 = s.substr(index);      
      // check for name request rather than command line launch

      // no anything after name (?[n])
      if(s2.find(" ") == -1)
      {
        command = s2;
        return 1;
      }
      // get left overs
      string s3 = s2.substr(s2.find(" "));
      // save command
      command = s2.substr(0,s2.find(" "));
      // should only contain a space and a n if requested...
      if (s3.find(" n") != -1)
      {
        // so flag pid and return true
        pid = 1;
        return 1;
      }
      // else bad extension, will return 0;
    }
  }

  return 0;
}

/**************************************************************************//**
 * @author Alex Wulff
 *
 * @par Description:
 * This function determines the validity of command line arguments.
 *
 * @param[in] argc - the number of arguments from the command prompt.
 * @param[in] argv - a 2d array of characters containing the arguments.
 *
 * @returns 0 - bad arguments
 * @returns 1 - if arguments are of the correct count... only program
 *
 *****************************************************************************/

int Error_Check(int argc, char** argv)
{
  if (argc > 1)
  {
    return 0;
  }
  return 1;
}

/**************************************************************************//**
 * @author Alex Wulff
 *
 * @par Description:
 * This function is the basic looping mechanism to cycle until an exit is
 * requested.
 *
 *
 * @returns 0 - exit requested.
 * @returns 1 - other commands (including bad ones) requested, so loop.
 *
 *****************************************************************************/

int Loop()
{
  string input, cmdnm;
  int command, pid, sigNum;
  // print instructions
  Main_Menu();
  // get and switch on response
  getline(cin, input);
  if (!Command_Parse(input, command, pid, sigNum, cmdnm))
  {
    cout <<("\nBad arguments in your command.\n") << endl;
  }
  else
  {
    switch(command)
    {
      // cmdnm
      case 0:
        Command_Name(pid);
        break;
      // signal
      case 1:
        Signal(sigNum, pid);
        break;
      // systat
      case 2:
        Systat(pid);
        break;
      // exit
      case 3:
        return 0;
      case 4:
         ProcessLookup(pid,cmdnm);
    }
  }
  return 1;
}

/**************************************************************************//**
 * @author Alex Wulff
 *
 * @par Description:
 * This function is a simple usage output of how to use the system.
 *
 *
 * @returns none.
 *
 *****************************************************************************/

void Main_Menu()
{
  cout << endl << endl;
  cout << "***********************************FUNCTIONS*************************************";
  cout << "\n\ncmdnm: Usage - cmdnm <pid>\n";
  cout << "\tExplanation: Provides the command string for this ";
  cout << "started process.\n\n";
  cout << "signal: Usage - signal <signal_num> <PID>\n";
  cout << "\tExplanation: sends specified signal to specified ";
  cout << "process id.\n\n";
  cout << "systat: Usage - systat [p]\n";
  cout << "\tExplanation: Provides system information.\n";
  cout << "\tUse the p flag to show process info... WARNING could be\n";
  cout << "\ta LOT of output (>> to file?).\n\n";
  cout << "pid : Usage - pid <command> [n]\n";
  cout << "\tExplanation: provides the process id's for a given command.\n";
  cout << "\tThe optional n flag indicates process name (ps form) vs process\n";
  cout << "\tcommand line execution command (/proc form).\n\n";
  cout << "exit: Usage - exit\n";
  cout << "\tExplanation: exits system.\n\n";
  cout << "*********************************************************************************";
  cout << endl << endl;
}

/**************************************************************************//**
 * @author Alex Wulff
 *
 * @par Description:
 * This function gets a list of all current running processes with the 
 * specified name and presents them to the user.
 *
 * @param[in] flag - int to determine which output form.
 * @param[in] cmd - string for pid lookup.
 *
 * @returns none.
 *
 *****************************************************************************/
void ProcessLookup(int flag, string cmd)
{
  string output;
  cout << "All about " <<cmd << ":" << endl << endl;
  if (flag)
  {
    // make headder so this makes sense
    cout << "F S   UID   PID  PPID  C PRI  NI ADDR ";
    cout << "SZ WCHAN  TTY          TIME CMD" << endl;

    // call out to get processes
    output = "ps -el| grep \"" + cmd + "\"";
    system(output.c_str());
  }
  else
  {
    // do the fun request.
    //output = "head /proc/[0-9]*/cmdline | grep -B 1 -A 1 -a \"" + cmd  +  "\"";
    WriteCmdProc(cmd);
  }

  // flush
  cout << endl << endl; 
}

/**************************************************************************//**
 * @author Alex Wulff
 *
 * @par Description:
 * This function is for a human readable version of the signals received.
 *
 *
 * @returns various strings per signal received... seems to only catch 2.
 *
 *****************************************************************************/

string SigType(int a) 
{
  switch (a)
  {
    case 1: 
      return (string)("SIGHUP");
    case 2: 
      return (string)("SIGINT");
    case 3: 
      return (string)("SIGQUIT");
    case 4: 
      return (string)("SIGILL");
    case 5: 
      return (string)("SIGTRAP");
    case 6: 
      return (string)("SIGABRT");    
    case 7: 
      return (string)("SIGBUS");     
    case 8: 
      return (string)("SIGFPE");     
    case 9: 
      return (string)("SIGKILL");   
    default: 
      return (string) "other";
  }
}

/**************************************************************************//**
 * @author Alex Wulff
 *
 * @par Description:
 * This function is a simple signal call to a specified process id at a
 * specified signal level.
 * 
 * @param[in] signal - signal desired to send.
 * @param[in] pid - target of signal.
 *
 *
 * @returns none.
 *
 *****************************************************************************/

void Signal(int signal, int pid)
{
  //cout <<"Signal" << endl << endl;
  kill(pid, signal);
}

void Signaled(int signal)
{
  char ans;
  cout << "been signaled with " << signal << " which means: ";
  cout << SigType(signal) << endl << endl;
  cout << "Terminate? y/n" << endl;
  cin >> ans;
  if (ans == 'y' || ans == 'Y')
  {
    exit(signal);
  }
}

/**************************************************************************//**
 * @author Alex Wulff
 *
 * @par Description:
 * This function is a simple series of syscalls through bash to present
 * the statistics of the currently running .
 *
 *
 * @returns none.
 * 
 *****************************************************************************/

void Systat(int p)
{
  cout <<"Core Info:"<< endl;
  cout << "*********************************************************************************" << endl;
  // output 
  WriteFile("/proc/version");
  cout << "*********************************************************************************" << endl;
  cout << "CPU Details:" << endl;
  cout << "*********************************************************************************" << endl;
  WriteFile("/proc/cpuinfo");
  cout << "*********************************************************************************" << endl;
  cout << "Memory Info" << endl;
  cout << "*********************************************************************************" << endl;
  WriteFile("/proc/meminfo");
  cout << "*********************************************************************************" << endl;
  if (p)
  {
    cout << "Process Info"<<endl;
    cout << "*********************************************************************************" << endl;
    system("ls /proc/[0-9]*/*");
    cout << "*********************************************************************************" << endl;
  }
  cout << "Extras" << endl;
  cout << "*********************************************************************************" << endl;
  system("lspci");
  system("lsusb");
  cout << "*********************************************************************************" << endl;
  cout << endl << endl;
}

/**************************************************************************//**
 * @author Alex Wulff
 *
 * @par Description:
 * This function is a simple message maker to encourage a user to try using
 * this program again, should the command be of the wrong structure at the
 * start.
 *
 *
 * @returns none.
 * 
 *****************************************************************************/

void Usage_Menu()
{
  cout <<"Start by running \"./dsh\".  Then comes the exciting stuff.\n";
}

/**************************************************************************//**
 * @author Alex Wulff
 *
 * @par Description:
 * This function is a simple output of what is in /proc's files and prints 
 * various files as specified in the systat function has specified as a string.
 *
 * @param[in] s - file name.
 *
 * @returns none.
 *
 *****************************************************************************/

void WriteFile(string s)
{
  ifstream fin(s.c_str());
  string in;
  while (getline(fin,in))
  {
    cout << in << endl;
  }
  fin.close();
}

/**************************************************************************//**
 * @author Alex Wulff
 *
 * @par Description:
 * This function is a simple output of what is in /proc and prints what the
 * user has specified as a pid, should there be a match.
 *
 * @param[in] pid - process id to search for.
 *
 * @returns none.
 *
 *****************************************************************************/

void WriteProcCmd(int pid)
{
  ifstream fin;
  string dir = "/proc/";
  DIR *proc;
  bool found = false;
  
  // try to get to /proc/
  if ((proc = opendir(dir.c_str())))
  {
    // read items and check if number (pid) and that that number is pid
    while (item = readdir (proc))
  {
    string name = item->d_name;
    // if true, print the cmdline
    if (atoi(name.c_str()) == pid)
    {
      string temp = dir + item->d_name + "/cmdline";
      fin.open(temp.c_str());
      getline(fin,temp);
      cout << item->d_name << ":" << endl;
      cout << '\t' << temp << endl;
      fin.close();
      found = true;
    }
  }

  }
  else
  {
    cout << "ERROR WITH DIR" << endl << endl;
  }
  if (!found)
  {
    cout << "Could not find specified pid." << endl;
  }
  
}

/**************************************************************************//**
 * @author Alex Wulff
 *
 * @par Description:
 * This function is a simple output of what is in /proc and prints what the
 * user has specified as a process name, should there be a match.
 *
 * @param[in] s - process name to search for.
 *
 * @returns none.
 *
 *****************************************************************************/

void WriteCmdProc(string s)
{
  ifstream fin;
  string dir = "/proc/";
  DIR *proc;
  bool found = false;
 
  // try to get to /proc/
  if ((proc = opendir(dir.c_str())))
  {
    // read items and check if number (pid)
    while (item = readdir (proc))       
  {
    string name = item->d_name;
    // if true, print the cmdline

    string temp = dir + item->d_name + "/cmdline";
    fin.open(temp.c_str());
    getline(fin,temp);
    // now check if the string is what is desired in the input
    if (temp.find(s) != -1)
    {
      // if match print pid and cmd
      cout << item->d_name << ": " << temp << endl;
      found = true;
    }
    fin.close();
  }

  }
  else
  {
    cout << "ERROR WITH DIR" << endl << endl;
  }
  if (!found)
  {
    cout << "Unable to find " << s << endl;
  }
}

