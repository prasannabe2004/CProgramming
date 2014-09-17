/**
* mario.c
*
* Prints half pyramid.
*
*/

#include <cs50.h>
#include <stdio.h>

/**
*
* This function is used to check the user input is within the range.
*
*/
int CheckInput(int input_number)
{
    if(input_number >=0 && input_number <=23)
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
    int n;
    int spaces = n-1,pounds = 2;
    int space_counter = 0, pound_counter = 0;
    do
    {
        // Get the user input
        printf("Height: ");
        n = GetInt();
        
        // Validate the user input
        if(CheckInput(n) == 1)
            continue;
        
        // Initial values for paces and pounds    
        spaces = n-1;
        pounds = 2;
        
        // Lets Start
        while(n>0)
        {
            for (space_counter=spaces;space_counter>0;space_counter--)
            {
                printf(" ");
            }
            spaces--;
            
            for (pound_counter=0;pound_counter<pounds;pound_counter++)
            {
                printf("#");   
            }
            pounds++;
            
            printf("\n");
            n--;
        }
    }while(n < 0 || n > 23);
    
    // Good Bye
    return 0;
}

