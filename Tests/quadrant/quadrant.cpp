#include <iostream>
using namespace std;

//--------------------------------------------
//  This program uses compound if statements
//  to determine if a point lies on the x axis,
//  the y axis, at the origin, or in one of 
//  the 4 quadrants.
//--------------------------------------------

int main()
  {
  float x;
  float y;

  cerr << "Enter the x value : ";
  cin >> x;

  cerr << "Enter the y value : ";
  cin >> y;

  if ((x == 0) && (y ==0))
    cout << "The point is the origin" << endl;
  
  if ((x == 0) && (y != 0))
    cout << "The point lies on the y axis " << endl;

  if ((x != 0) && (y == 0))
    cout << "The point lies on the x axis " << endl;

  if ((x > 0) && (y > 0))
    cout << "The point lies in the first quadrant " << endl;  
 
  if ((x <= 0) && (y > 0))
    cout << "The point lies in the second quadrant " << endl; 

  if ((x < 0) && (y < 0))
    cout << "The point lies in the third quadrant " << endl; 

  if ((x >= 0) && (y < 0))
    cout << "The point lies in the fourth quadrant " << endl; 
  }
