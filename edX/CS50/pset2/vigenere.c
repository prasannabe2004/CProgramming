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
    
    string key = (argv[1]);
    
    for (int i =0;i<strlen(key);i++)
    {
        if (! isalpha(key[i]))
        {
            printf("Keyword must only contain letters A-Z and a-z\n");
            return 1;
        }
    }        
    string plainText = GetString();
    int j=0;
    int n = strlen(plainText);
    for (int i = 0; i < n; i++)
    {
        if (isalpha(plainText[i]))
        {
                if(islower(key[j]))
                {    
                    int temp = (int)key[j] - 97 + plainText[i];
                    if (temp >122)
                        printf("%c", temp-26);
                    else
                        printf("%c", temp);
                }
                if(isupper(key[j]))
                {
                    int temp = (int)key[j] - 65 + plainText[i];
                    if (temp >90)
                        printf("%c", temp-26);
                    else
                        printf("%c", temp);
                }
                j = (++j)%strlen(key);
        }
        else
            printf("%c",plainText[i]);
    }
    printf("\n");
    return 0;
}
