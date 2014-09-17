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

/**
*
* This function is used to check the user input is within the range.
*
*/
int CheckInput(float input_number)
{
    if(input_number >= 0)
        return 0;
    else
        return 1;
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
        if(CheckInput(input) == 1)
        {
            continue;
        }
        // Convert to cents
        input_cents = (float)round(input * 100);
        
        // Lets Start
        
        int number_of_coins = 0;
        
        while(input_cents > 0.0)
        {
            //printf("input_cents=%f\n",input_cents);
            if(input_cents >= 25)
            {
                input_cents = input_cents - 25;
                number_of_coins++;  
            }
            else if(input_cents < 25 && input_cents >=10)
            {
                input_cents = input_cents -10;
                number_of_coins++; 
            }
            else if(input_cents < 10 && input_cents >=5)
            {
                input_cents = input_cents -5;
                number_of_coins++; 
            }
            else
            {
                number_of_coins = number_of_coins + input_cents;
                input_cents = 0;
            }
        }
        printf("%d\n", number_of_coins);
        break;

    }while(input < 0.0);
    
    // Good Bye
    return 0;
}

