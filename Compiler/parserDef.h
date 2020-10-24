/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/

#include "stack.h"
#include "tree.h"
#include "firstFollow.h"

typedef struct ParseTableNode{
    token_node* tn;
    int rule_num;
}ParseTableNode;

int isCorrect;

ParseTableNode ParseTable[NO_OF_NON_TERMINALS][NO_OF_TERMINALS-1];
//token_node* ParseTable[NO_OF_NON_TERMINALS][NO_OF_TERMINALS-1];
tree* parseTree;


