/**
* greedy.c
*
* Calculates number of coins to be owed.
*
*/

#include <cs50.h>
#include <stdio.h>
#include <math.h>

#define QUATAR 25
#define DIME 10
#define NICKEL 5
#define PENNY 1

#define TRUE 0
#define FALSE 1

/**
*
* This function is used to check the user input is within the range.
*
*/
int CheckInput(float input_number)
{
    if(input_number >= 0)
        return TRUE;
    else
        return FALSE;
}

/**
*
* Main function of this program.
*
*/
int main()
{
    float input;
    float input_cents;

    do
    {
        // Get the user input
        printf("O hai! How much change is owed?\n");
        input = GetFloat();
        
        // Validate the user input
        if(CheckInput(input) == FALSE)
        {
            continue;
        }
        
    }while(input < 0.0);
    // Convert to cents
    input_cents = (float)round(input * 100);
    
    // Lets Start
    
    int number_of_coins = 0;
    
    while(input_cents > 0.0)
    {
        //printf("input_cents=%f\n",input_cents);
        if(input_cents >= QUATAR)
        {
            // This means the input is can be subtracted by a QUATAR
            input_cents = input_cents - QUATAR;
            number_of_coins++;  
        }
        else if(input_cents < QUATAR && input_cents >= DIME)
        {
            // This means the input is can be subtracted by a DIME
            input_cents = input_cents - DIME;
            number_of_coins++; 
        }
        else if(input_cents < DIME && input_cents >= NICKEL)
        {
            // This means the input is can be subtracted by a NICKEL
            input_cents = input_cents - NICKEL;
            number_of_coins++; 
        }
        else
        {
            // This means remaining are PENNIES. So just add as many number of pennies.
            number_of_coins = number_of_coins + input_cents;
            input_cents = 0;
        }
    }
    printf("%d\n", number_of_coins);

    
    // Good Bye
    return 0;
}

