#include <iostream>
using namespace std;
//------------------------------------------
//  The purpose of this program is to 
//  determine the letter grade a student
//  earns based on his overall average
//  The program is implemted 2 ways - with
//  nested if statements and with compund if
//  statements.
//
//  Note : we cannot do this problem with a switch
//  statement because it involves ranges.
//-------------------------------------------
int main()
 {
  float average;
 
  cerr << "Enter your overall average : ";
  cin >> average;

  if ( average >= 90.0)
     cout << "A" << endl;
  else if ( average > 80.0)
     cout << "B" << endl;
  else if (average > 70.0)
     cout << "C" << endl;
  else if (average >= 60.0)
     cout << "D" << endl;
  else
     cout << "F" << endl;

}
