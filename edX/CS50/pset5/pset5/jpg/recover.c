/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t  BYTE;

int main(int argc, char* argv[])
{
    
    // open input file 
    FILE* inptr = fopen("card.raw", "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", "card.raw");
        return 2;
    }

    BYTE buffer[512];
    
    int count = 0;
    
    FILE* outptr = NULL;
    
    while(!feof(inptr))
    {      
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3]== 0xe0 || buffer[3]== 0xe1))
        {
            char filename[16];
            sprintf(filename,"%03d.jpg", count);
            if(outptr != NULL)
                fclose(outptr);
            outptr = fopen(filename, "w");
            if (outptr == NULL)
            {
                printf("Could not open %s.\n", filename);
                return 2;
            }
            fwrite(&buffer, 512, 1, outptr);  
            count++;      
        }
        else if(count != 0)
        {
            fwrite(&buffer, 512, 1, outptr);
        }
        fread(&buffer, 512, 1, inptr); 
    }
    // close infile
    fclose(inptr);

    // that's all folks
    return 0;
}
