//this program should print out a rectangle, using the
//height and width entered by the user.

#include <iostream> 
using namespace std;
int main() 
{ 
   int iHeight = 0;
	int iWidth = 0;
	int iHeightCount = 1;
	int iWidthCount = 1;

	cerr << "Enter the height of the rectangle: "; 
	cin >> iHeight;
	cerr << "Enter the width of the rectangle: "; 
	cin >> iWidth;
   cerr << endl; 

   while (iHeightCount <= iHeight) 
   { 
      while (iWidthCount <= iWidth) 
      { 
         cout << "*"; 
         ++iWidthCount; 
      } 
		cout << endl; 
      ++iHeightCount; 
      iWidthCount = 1;
   } 
   cerr << endl; 
	return 0;
} 
