/*************************************************************************//**
 * @file 
 *
 * @mainpage Program 3 - XML Directory
 * 
 * @section course_section Course Information 
 *
 * @author Joe Manke
 * 
 * @date April 5, 2012
 * 
 * @par Professor: 
 *         Roger Schrader
 * 
 * @par Course: 
 *         CSC250 - M002 -  1:00 pm
 * 
 * @par Location: 
 *         McLaury - McLaury 313
 *
 * @section program_section Program Information 
 * 
 * @details This program creates an XML file detailing a computer system 
 *  directory. The user passes in two or three command line arguments 
 *  specifying what they want included in the XML file. The first possible
 *  argument is an option for which timestamps they want included. The options
 *  are the creation date, the modification date, or both. The next argument
 *  is the directory to create the XML file for. The final argument is the
 *  pattern of files to search for. The program will only make tags for files
 *  that match the pattern.
 *
 * The first thing the program does is check the command line arguments for
 *  errors, outputting an error message and a usage statement if any errors are
 *  found. Next, it opens the XML file, named "dir.xml", for output. If the
 *  file open is not successful, the program will output an error message and 
 *  close. Then, it will prepare to write the XML file by changing to the 
 *  directory as given by the user. If this fails, the program will output the
 *  usage statement and exit. 
 *
 * If the directory is changed successfully, the program enters a function that
 *  finds all the sub-folders and files within the directory. If a sub-folder
 *  is found, the function is called recursively. Each folder is given a folder
 *  tag. After finding all the sub-folders, another function is called to 
 *  create tags for all the files within the directories. It is in this 
 *  function where the option is dealed with. After all the tags are created,
 *  the program outputs a message to the screen informing the user of success,
 *  and then ends.
 *
 * @section compile_section Compiling and Usage 
 *
 * @par Compiling Instructions: 
 *      N/A
 * 
 * @par Usage: 
   @verbatim  
   c:\> prog3.exe [option] <directory> <pattern>
   d:\> c:\bin\prog3.exe [option] <directory> <pattern>
   @endverbatim 
 *
 * @section todo_bugs_modification_section Todo, Bugs, and Modifications
 * 
 * @bug N/A
 * 
 * @todo 
 * 
 * @par Modifications and Development Timeline: 
   @verbatim 
   Date          Modification 
   ------------  -------------------------------------------------------------- 
   Mar 24, 2012  Created project, wrote check_errors and usage
   Mar 26, 2012  Wrote open_output, started write_xml
   Mar 27, 2012  Worked on write_xml
   Mar 28, 2012  Fixed an if statement in check_errors, moved file_tag out of
                    write_xml into its own function
   Mar 29, 2012  Added recursion, tried to fix bugs
   Apr 03, 2012  Fixed recursion/bugs
   Apr 05, 2012  Finished documentation, fixed bugs
   @endverbatim
 *
 *****************************************************************************/

//#include <direct.h>
#include <io.h>
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;

int check_errors(int argc, char *argv[]);
void usage();
int open_output(ofstream &fout);
void write_xml(ofstream &fout, char option[], char pattern[]);
void file_tag(ofstream &fout, char option[], char pattern[]);

/**************************************************************************//** 
 * @author Joe Manke
 * 
 * @par Description: 
 * Runs the program. Starts by calling for error-checking on the command line
 *  arguments, then makes sure the output stream opens correctly. It sets the
 *  initial directory while making sure it is valid, then calls the write_xml
 *  function to actually create the XML file. After this is completed, main
 *  closes the output stream and ends the program.
 * 
 * @param[in]      argc - the number of command line arguments
 * @param[in]      argv - a character array holding the command line arguments
 * 
 * @returns 0 program ran successful.
 * @returns 1 incorrect number of command arguments.
 * @returns 2 invalid option.
 * @returns 3 invalid directory.
 * @returns 4 failure opening XML file.
 * 
 *****************************************************************************/
int main(int argc, char *argv[])
{
    //declare variables
    int error = 0;
    ofstream fout;
    char option[3] = {NULL};
    //check command line for errors
    error = check_errors(argc, argv);
    if (error != 0)
        return error;
    //open XML file
    error = open_output(fout);
    if (error != 0)
        return error;
    //set directory
    if( _chdir( argv[argc-2] )  != 0 )
    {
        cout << "Unable to change to the directory " << argv[argc-2] << endl;
        usage();
        return 3;
    }
    //store option for use in write_xml
    if(argc == 4)
        strcpy(option, argv[1]);
    //write XML file
    write_xml(fout, option, argv[argc-1]);
    //exit program
    fout.close();
    cout << "XML file written successfully." << endl;
    return 0;
}

/**************************************************************************//** 
 * @author Joe Manke
 * 
 * @par Description: 
 * Checks the command line arguments for errors.
 * 
 * @param[in]      argc - the number of command line arguments
 * @param[in]      argv - a character array holding the command line arguments
 * 
 * @returns 0 no errors.
 * @returns 1 incorrect number of command arguments.
 * @returns 2 invalid option.
 * 
 *****************************************************************************/
int check_errors(int argc, char *argv[])
{
    //check number of arguments
    if(argc < 3 || argc > 4)
    {
        cout << "Incorrect number of command line arguments.\n";
        usage();
        return 1;
    }
    //check for valid option
    if( argc == 4 && strcmp(argv[1], "-B") != 0 && strcmp(argv[1], "-C") != 0
        && strcmp(argv[1], "-M") != 0 )
    {
        cout << "Invalid option.\n";
        usage();
        return 2;
    }
    //no errors
    return 0;
}

/**************************************************************************//** 
 * @author Joe Manke
 * 
 * @par Description: 
 * Outputs a usage statement if there was a problem with the command line 
 * arguments.
 * 
 * @returns none
 * 
 *****************************************************************************/
void usage()
{
    cout << "Usage Statement: \n\n";
    cout << "Command Line Syntax: \n";
    cout << "C:\\prog3.exe [option] <directory> <pattern> \n\n";
    cout << "Options: \n";
    cout << "-C \t\t show creation date \n";
    cout << "-M \t\t show modification date \n";
    cout << "-B \t\t show both dates \n";
    cout << "No option \t no date shown \n\n";
    cout << "<directory> is the starting point for the XML file creation. \n";
    cout << "\tPlease put the directory name in quotes. \n\n";
    cout << "<pattern> filters which files will be outputted to the XML file";
    cout << endl << "\t\"*.*\" will output all files in the directory. \n";
    cout << "\t\"*.<extension>\" will output all files with the extension "
        << "given. \n";
    cout << "\t\"<name>.*\" will output all files with the name given. \n";
    cout << "\t\"<string>*.*\" will output all files with names starting with "
        << "the given \n\t\t string of characters. \n\n";
    cout << "Exiting program.\n";
}

/**************************************************************************//** 
 * @author Joe Manke
 * 
 * @par Description: 
 * Opens the XML file for output, and checks for a successful open.
 * 
 * @param[in,out]      fout - the ofstream to be used for output
 * 
 * @returns 0 XML file opened successfully.
 * @returns 4 failure opening XML file.
 * 
 *****************************************************************************/
int open_output(ofstream &fout)
{
    //open file
    fout.open("dir.xml");
    //check for successful open
    if(!fout)
    {
        cout << "Error opening \"dir.xml\". \n";
        return 4;
    }
    else
    {
        fout << "<?xml version= \"1.0\"?>\n";
        return 0;
    }
}

/**************************************************************************//** 
 * @author Joe Manke
 * 
 * @par Description: 
 * Writes an XML file directory by recursively creating folder tags, and then 
 *  calling another function to write the file tags within the folders.
 * 
 * @param[in,out]      fout - the ofstream to be used for output
 * @param[in]          option - the option for which timestamp(s) are included
 *                      in the tag
 * @param[in]          pattern - the pattern of filenames to create tags for
 * 
 * @returns none
 * 
 *****************************************************************************/
void write_xml(ofstream &fout, char option[], char pattern[])
{
    //declare variables
    _finddata_t a_file;     
    intptr_t  dir_handle;
    char *buffer;
    //set directory
    dir_handle = _findfirst("*.*", &a_file);
    if( dir_handle == -1 )         
        return;
    //output folder tag
    buffer = _getcwd( NULL, 0 );
    fout << "<folder name=\"" << buffer << "\">" << endl;
    //make tags
    do     
    {   
        if( strcmp(a_file.name, ".") && strcmp(a_file.name, "..") )
        {
            //check if folder
            if( a_file.attrib & _A_SUBDIR ) 
            {
                _chdir(a_file.name);
                write_xml(fout,option,pattern);
                _chdir("..");
            }
        }
    }while( _findnext( dir_handle, &a_file ) == 0 );
    //make file tags
    file_tag(fout, option, pattern);
    fout << "</folder>" << endl;
    //free stuff up
    _findclose( dir_handle );
    delete buffer;
}

/**************************************************************************//** 
 * @author Joe Manke
 * 
 * @par Description: 
 * Outputs XML tags for files within a folder
 * 
 * @param[in,out]      fout - the ofstream to be used for output
 * @param[in]          option - the option for which timestamp(s) are included
 *                      in the tag
 * @param[in]          pattern - the user-specified pattern to search for; only
 *                      makes tags for files which fit the pattern
 * 
 * @returns none
 * 
 *****************************************************************************/
void file_tag(ofstream &fout, char option[], char pattern[])
{ 
    //declare variables
    intptr_t  dir_handle;
    _finddata_t a_file;
    char *time = NULL;
    //set directory
    dir_handle = _findfirst(pattern, &a_file);
    if( dir_handle == -1 )
        return;
    //find files
    do     
    {   
        if( (a_file.attrib & _A_SUBDIR) != _A_SUBDIR )
        {   
            //output file tag
            fout << "<file name=\"" << a_file.name << "\"";
            //deal with options
            //creation date
            if(strcmp(option, "-C") == 0 || strcmp(option, "-B") == 0)
            {
                time = asctime(localtime(&a_file.time_create));
                time[strlen(time)-1] = NULL;
                fout << " DateCreated=\"" << time << "\"";
            }
            //modification date
            if(strcmp(option, "-M") == 0 || strcmp(option, "-B") == 0)
            {
                time = asctime(localtime(&a_file.time_write));
                time[strlen(time)-1] = NULL;
                fout << " DateModified=\"" << time << "\"";
            }
            fout << " />" << endl; 
        }
    }while( _findnext( dir_handle, &a_file ) == 0 );
    _findclose( dir_handle );
}
