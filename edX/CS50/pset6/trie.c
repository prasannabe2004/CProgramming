#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "trie.h"

/** This function finds the index for the  given character. It returns 26 for apostrope..
 * @param[in] ch Character to be indexed
 * @author Prasanna
 * @date 10/10/2014
 * @see trie.h
 **/

int char2Index(char ch)
{
    if(isalpha(ch))
    {
        // We got an alphabet
        if(islower(ch))
        {
            return (int)ch - (int)'a';
        }
        else
        {
            return (int)ch - (int)'A'; 
        }
    }
    else
    {
        // This space is for apostrope.
        return 26;
    }
    return ch;
}

/** This function deletes a node from the trie
 * @author Prasanna
 * @date 10/10/2014
 * @see trie.h
 **/
void deleteNode(node_t* ptr)
{
    for(int i=0;i < ALPHABET_SIZE;i++)
    {
        if(ptr->children[i] != NULL)
        {
            deleteNode(ptr->children[i]);
            ptr->children[i] = NULL;
        }
    }
    free(ptr);
}

/** This function deletes the trie
 * @author Prasanna
 * @date 10/10/2014
 * @see trie.h
 **/
void deleteTrie(trie_t* ptr)
{
    if(ptr)
    {
        if(ptr->root)
        {
            deleteNode(ptr->root);
        }
    }
    //free(ptr);
}

/** Allocate memory for a new node
 * @author Prasanna
 * @date 10/10/2014
 * @see trie.h
 **/
node_t *newNode(void)
{
    node_t *pNode = NULL;
 
    pNode = (node_t *)malloc(sizeof(node_t));
 
    if( pNode )
    {
        int i;
 
        pNode->value = 0;
        // Initialize all the children to NULL
        for(i = 0; i < ALPHABET_SIZE; i++)
        {
            pNode->children[i] = NULL;
        }
    }
 
    return pNode;
}
 
/** This function initialize the trie.
 * @author Prasanna
 * @date 10/10/2014
 * @see trie.h
 **/
void initialize(trie_t *pRoot)
{
    pRoot->root = newNode();
    pRoot->count = 0;
}
 
/** This function inserts a new node into the trie.
 * @author Prasanna
 * @date 10/10/2014
 * @see trie.h
 **/
void insert(trie_t *pRoot, char* key)
{
    int level;
    int length = strlen(key);
    int index = -1 ;
    node_t *ptr;
 
    pRoot->count++;
    ptr = pRoot->root;
 
    for( level = 0; level < length; level++ )
    {
        index = char2Index(key[level]);
        if( !ptr->children[index] )
        {
            ptr->children[index] = newNode();
        }
 
        ptr = ptr->children[index];
    }
    ptr->value = pRoot->count;
}
 
/** This function queries the key in trie.
 * @author Prasanna
 * @date 10/10/2014
 * @see trie.h
 **/
int query(trie_t *pRoot, const char* key)
{
    int level;
    int length = strlen(key);

    int index = -1 ;
    node_t *ptr;
 
    ptr = pRoot->root;
    
    for( level = 0; level < length; level++ )
    {
        index = char2Index(key[level]);
        if( !ptr->children[index] )
        { 
            // reached the end, still not found
            return 0;
        }
        ptr = ptr->children[index];
    }

    if(ptr != NULL && ptr->value != 0)
        return 1;
    else
        return 0;
}

