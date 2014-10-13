/****************************************************************************
 * dictionary.h
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Declares a dictionary's functionality.
 ***************************************************************************/

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>
#include <strings.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define ALPHABET_SIZE (27)
 
typedef struct node
{
    unsigned int value;
    struct node *children[ALPHABET_SIZE];
}node_t;
 

typedef struct trie
{
    struct node *root;
    int count;
}trie_t;
 
node_t *newNode(void);
void initialize(trie_t *pTrie);
void insert(trie_t *pTrie, char* key);
int query(trie_t *pTrie, const char* key);
void deleteNode(node_t* ptr);
void deleteTrie(trie_t* ptr);
int char2Index(char ch);


// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word);

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

#endif // DICTIONARY_H
