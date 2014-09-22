/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include <stdio.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false. Binary Search.
 */
bool search(int value, int values[], int n)
{
    // a searching algorithm using binary search. Assuming the array ia already sorted.
    if ( n < 1)
        return false;
    int low = 0;
    int high = n-1;
    int mid = 0;
    
    while(low <= high)
    {
        mid = (low+high)/2;
        if (value == values[mid])
            return true;
        else if (value > values[mid])
            low = mid + 1;
        else if (value < values[mid])
            high = mid - 1;
    }
    return false;
}

/**
 * Sorts array of n values using selection sort. 
 */
void sort_select(int values[], int n)
{
    // TODO: implement an Selection Sort O(n^2) sorting algorithm .
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

/**
 * Sorts array of n values using bubble sort. 
 */
void sort(int values[], int n)
{
    // TODO: implement an Bubble Sort O(n^2) sorting algorithm .
    for ( int i = 0; i < n ; i++)
    {
        for ( int j = 0 ; j < n; j++)
        {
            if (values[j] > values[j+1])
            {
                int temp;
                temp = values[j];
                values[j] = values[j+1];
                values[j+1] = temp;
            }
        }
    }      
    return;
}

/**
 * Sorts array of n values using insertion sort. 
 */
void sort(int values[], int n)
{
    // TODO: implement an Insertion Sort O(n^2) sorting algorithm .
    for ( int i = 0; i < n ; i++)
    {
        for ( int j = 0 ; j < n; j++)
        {

        }
    }      
    return;
}

