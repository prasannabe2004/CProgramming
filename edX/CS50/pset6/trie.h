 
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

