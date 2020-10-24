/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/

#include "set.h"
#include "lexer.h"

#define set uint64_t
#define POS_OF_EPS 41
#define NO_OF_TERMINALS 41 // including eps and $
#define NO_OF_NON_TERMINALS 46  // including startProgram
#define NO_OF_RULES 88

typedef struct token_node{
    char token[30];
	struct token_node* next;
}token_node;

typedef struct node{
	token_node* head;
    int non_terminal_num;
    int rule_num;
    set rhs_FIRST;
    token_node* last;
}node;

typedef node* GRAMMAR;

HashTable* term_hash;
HashTable* non_term_hash;

set FirstSet[NO_OF_NON_TERMINALS];
set FollowSet[NO_OF_NON_TERMINALS];

int getIndexNonTerminal(char* token);
int getIndexTerminal(char* token);
void getFirstSet(GRAMMAR gr);
void getFollowSet(GRAMMAR gr);

