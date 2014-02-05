#include <iostream>
#include <cmath>
#include <fstream>

int main()
{
    int a,b,c;
    ifstream fin;

    fin.open("infile.txt");
    
    fin >> a;
    fin >> b;
    fin >> c;

    root1 = (-b + sqrt(b * b - 4 * a * c) ) / (2*a);
    root2 = (-b - sqrt(b * b - 4 * a * c) ) / (2*a);

    cout << root1 << " and " << root2 << endl;
    return 0;
}
