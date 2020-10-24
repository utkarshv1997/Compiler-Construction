/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/

#include "hashTable.h"

unsigned long hash(char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++) != 0)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

hashTableNode* newHashNode(int inx, char* str){
    hashTableNode* temp = (hashTableNode*)malloc(sizeof(hashTableNode));
    temp->str = str;
    temp->index = inx;
    temp->next = NULL;
    return temp;
}

HashTable* createHashTable(int size){
    HashTable* temp = (HashTable*)malloc(sizeof(HashTable)*size);
    temp->size_of_table = 0;
    int i = 0;
    for(i=0;i<size;++i)
        temp[i].head = NULL;
    return temp;
}

void insertInHashTable(HashTable* ht,int inx,int index,char* str){
    if(ht[inx].head == NULL){
        ht[inx].head = newHashNode(index,str);
        ++ht->size_of_table;
        return;
    }

    hashTableNode *temp = ht[inx].head;

    while(temp->next != NULL)
        temp = temp->next;

    temp->next = newHashNode(index,str);
}

