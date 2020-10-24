/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AST.h"


unsigned long hashVal(char *str);
FunHashTable* createFunTable(int size);
void insertInFunTable(FunHashTable* ht,char* fid,symbolTableNode* symTable,int lno);
IdHashTable* createIdTable(int size);
IdHashTableNode* insertInIdTable(IdHashTable* ht,char* id,int type,int lno,int offset);
