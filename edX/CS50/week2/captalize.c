#include <stdio.h>
#include <string.h>

int main()
{
    char str[128];
    printf("Enter the string:");
    scanf("%s", str);
    
    printf("Input=%s\n",str);
    
    for(int i=0;i<strlen(str);i++)
    {
        if(str[i] >= 'a' && str[i] <= 'z')
        {
            str[i] = str[i] - ('a'-'A');
        }
    }
    printf("Output=%s\n",str);
    return 0;
}