/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include "dictionary.h"

rb_red_blk_tree* tree = NULL;
unsigned int words_in_dict = 0 ;

int StrComp(const void* a,const void* b) 
{
    /* Compare the Domain name and return the value to RB Tree. */
    if(strcasecmp((char *)a,(char *)b) > 0 ) 
    {
        return(1); /* String "a" is greater than string "b" */
    }
    else if(strcasecmp((char *)a,(char *)b) < 0)
    {
        return (-1); /* String "b" is greater than string "a" */
    }
    /* Both the strings are equal. */
    return(0);
}

void InfoPrint(void* a) 
{
    rb_red_blk_node* newNode = (rb_red_blk_node*)a;
    if(newNode)
        printf("%s->", (char *)newNode->key);
}

void IntPrint(const void* a) 
{
    printf("%s",(char*)a);
}

void IntDest(void* a) 
{
    char* key = (char*)a;
    if(key)
        free(key);
}

void InfoDest(void *a)
{
    ;
}


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    rb_red_blk_node* newNode;
    if ( ( newNode = RBExactQuery(tree, (char *)word) ) ) 
    { 
        return true;
    } 
    else 
    {
        return false;
    }
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    if(tree == NULL)
        tree = RBTreeCreate(StrComp,IntDest,InfoDest,IntPrint,InfoPrint);
    
    FILE* inptr = fopen(dictionary, "r");
    char* newString;
    
    if(inptr != NULL)
    {
        char buffer[46];
        while(fscanf(inptr, "%s\n", buffer) != EOF)
        {
            newString=(char*) malloc(strlen(buffer)+1);
            strcpy(newString,buffer);
            RBTreeInsert(tree,newString,0);
            words_in_dict = words_in_dict + 1;
        }
    }
    else
    {
        return false;
    }
    fclose(inptr);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return words_in_dict;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    RBTreeDestroy(tree);
    return true;
}
