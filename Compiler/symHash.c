/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/

#include "symHash.h"


int size_of_idtable = 193;
int size_of_funtable = 51;

unsigned long hashVal(char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++) != 0)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

FunHashTableNode* newFunHashNode(char* fid,symbolTableNode* symTable){
    FunHashTableNode* temp = (FunHashTableNode*)malloc(sizeof(FunHashTableNode));
    temp->fId = fid;
    temp->funSymbolTable = symTable;
    temp->next = NULL;
    return temp;
}

FunHashTable* createFunTable(int size){
    FunHashTable* temp = (FunHashTable*)malloc(sizeof(FunHashTable)*size);
    temp->size_of_table = 0;
    int i = 0;
    for(i=0;i<size;++i)
        temp[i].head = NULL;
    return temp;
}

void insertInFunTable(FunHashTable* ht,char* fid,symbolTableNode* symTable,int lno){
    int inx = hashVal(fid) % size_of_funtable;
    if(ht[inx].head == NULL){
        ht[inx].head = newFunHashNode(fid,symTable);
        ++ht->size_of_table;
        return;
    }
    FunHashTableNode* temp = ht[inx].head;

	if(strcmp(temp->fId,fid) == 0){
		semantic_flag = 0;
		printf("Function Overloading i.e. Multiple declarations of Function %s, not allowed in line %d.\n",fid,lno);
		return;
	}

    while(temp->next != NULL){
        temp = temp->next;
		if(strcmp(temp->fId,fid) == 0){
		semantic_flag = 0;
			printf("Function Overloading i.e. Multiple declarations of Function %s, not allowed in line %d.\n",fid,lno);
			return;
		}
	}

    temp->next = newFunHashNode(fid,symTable);
}

IdHashTableNode* newIdHashNode(char* id,int type,int offset){
    IdHashTableNode* temp = (IdHashTableNode*)malloc(sizeof(IdHashTableNode));
    temp->id = id;
    temp->type = type;
    temp->row_size = temp->col_size = -1;
    temp->string_size = -1;
    temp->next = NULL;
    temp->is_assigned = 0;
    temp->offset = offset;
    return temp;
}

IdHashTable* createIdTable(int size){
    IdHashTable* temp = (IdHashTable*)malloc(sizeof(IdHashTable)*size);
    temp->size_of_table = 0;
    int i = 0;
    for(i=0;i<size;++i)
        temp[i].head = NULL;
    return temp;
}

IdHashTableNode* insertInIdTable(IdHashTable* ht,char* id,int type,int lno,int offset){
    int inx = hashVal(id) % size_of_idtable;   
	if(ht[inx].head == NULL){
        ht[inx].head = newIdHashNode(id,type,offset);
        ++ht->size_of_table;
        return ht[inx].head;
    }

    IdHashTableNode* temp = ht[inx].head;
	
	if(strcmp(temp->id,id) == 0){
		semantic_flag = 0;
		printf("Multiple declarations of id(%s) not allowed in line number %d.\n",id,lno);
		return temp;
	}

    while(temp->next != NULL){
        temp = temp->next;
		if(strcmp(temp->id,id) == 0){
			semantic_flag = 0;
			printf("Multiple declarations of id(%s) not allowed in line number %d.\n",id,lno);
			return temp;
		}
	}

    return temp->next = newIdHashNode(id,type,offset);
}
