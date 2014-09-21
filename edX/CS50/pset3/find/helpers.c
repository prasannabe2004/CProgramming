/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    if ( n < 1)
        return false;
    int low = 0;
    int high = n-1;
    int mid = (low+high)/2;;
    
    while(low == mid)
    {
        if (value == values[mid])
            return true;
        else if (value > values[mid])
            low = mid;
        else if (value < values[mid])
            high = mid;
        mid = (low+high)/2;
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    for ( int i = 0; i < n -1; i++)
    {
        for ( int j = i + 1; j < n; j++)
        {
            if (values[i] > values[j])
            {
                
                int temp;
                temp = values[i];
                values[i] = values[j];
                values[j] = temp;
            }
        }
    }      
    return;
}
