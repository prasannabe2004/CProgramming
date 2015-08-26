#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main (int argc, char** argv)
{
    string plainText = GetString();
    char final[50]={'\0'};

    final[0] = toupper(plainText[0]);
    for (int i = 1,j = 1, n = strlen(plainText); i < n; i++)
    {
        if(isspace(plainText[i]))
        {
            final[j] = toupper(plainText[i+1]);
            j++;
        }
    }
    printf("%s\n",final);
    return 0;
}
