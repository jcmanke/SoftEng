//  Standard Deviation - use of array
//
//  CSC 150   V Manes   10/4/02


#include <iostream>
#include <cmath>

using namespace std;

const int NUM_SCORES = 10;

int main()
{
    double	scores[NUM_SCORES];
    double	avg = 0.0, sum = 0.0, stDev = 0.0;
    int		i;

    //enter the data to be evaluated
    for ( i = 0; i < NUM_SCORES; i++)
        cin >> scores[i];

    //compute sum and then average of the scores
    for ( i = 0; i < NUM_SCORES; i++)
        sum += scores[i];

    avg = sum / NUM_SCORES;

    //now work on the standard deviation calculation
    sum = 0;
    for ( i = 0; i < NUM_SCORES; i++)
        sum += pow(avg - scores[i], 2);

    stDev = sqrt( sum / (NUM_SCORES-1) );

    cout << "The average score is: " << avg << endl;
    cout << "The Standard Deviation is: " << stDev << endl;

    return 0;
}

