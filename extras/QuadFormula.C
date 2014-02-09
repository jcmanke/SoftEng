#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

int main()
{
    int a,b,c;
    int root1 = 0;
    int root2 = 0;
    /*ifstream fin;

    fin.open("infile.txt");
    
    fin >> a;
    fin >> b;
    fin >> c;*/
    
    a = 1;
    b = 0;
    c = -1;

    root1 = (-b + sqrt(b * b - 4 * a * c) ) / (2*a);
    root2 = (-b - sqrt(b * b - 4 * a * c) ) / (2*a);

    cout << root1 << " and " << root2 << endl;
    return 0;
}
