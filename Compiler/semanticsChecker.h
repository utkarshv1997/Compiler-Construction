/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/

#include "symHash.h"

symbolTableNode* symRoot;

int main_offset;

void semanticCheck(tree_node* root);
void printSymbolTable(symbolTableNode* symRoot);
int getwidth(IdHashTableNode* idtemp,int type);
