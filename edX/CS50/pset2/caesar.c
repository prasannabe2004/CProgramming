#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main (int argc, char** argv)
{
    if (argc !=2 )
    {
        printf("Invalid input\n");
        return 1;
    }
    
    int key = atoi(argv[1]);
    string plainText = GetString();
    key= key%26;
    
    for (int i = 0, n = strlen(plainText); i < n; i++)
    {
        if (isalpha(plainText[i]))
        {
            int temp = (int)plainText[i] + key;
            if (islower(plainText[i]))
            {
                if(temp > 122)
                {
                    printf("%c",temp - 26);
                }
                else
                {
                    printf("%c",temp);
                }
            }
            else
            {
                if(temp > 90)
                {
                    printf("%c",temp - 26);
                }          
                else
                {
                    printf("%c",temp);
                }
            }
        }
        else
            printf("%c",plainText[i]);
    }
    printf("\n");
    return 0;
}
