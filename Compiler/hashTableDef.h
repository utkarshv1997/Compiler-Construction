/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/

#include <stdlib.h>

typedef struct hashTableNode{
    int index;
    char* str;
    struct hashTableNode* next;
}hashTableNode;

typedef struct HashTable{
    hashTableNode* head;
    int size_of_table;
}HashTable;


