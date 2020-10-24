/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/

#include <stdlib.h>

typedef struct stack_node{
    char* tok;
    struct stack_node* next;
}stack_node;

typedef struct stack{
    stack_node* top;
    int size;
}stack;

stack* createStack();
stack_node* newNode(char *str);
int isEmpty(stack* s);
void push(stack* s,char* str);
stack_node* pop(stack* s);
stack_node* top(stack* s);



