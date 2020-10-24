/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/

#include "stack.h"

stack* createStack(){
    stack* temp = (stack*)malloc(sizeof(stack));
    temp->size = 0;
    temp->top = NULL;
    return temp;
}

stack_node* newNode(char *str){
    stack_node* temp = (stack_node*)malloc(sizeof(stack_node));
    temp->tok = str;
    temp->next = NULL;
    return temp;
}

int isEmpty(stack* s){
    return (s->size == 0)? 1:0;
}

void push(stack* s,char* str){
    if(s->top == NULL){
        s->top = newNode(str);
        ++(s->size);
        return;
    }

    stack_node* temp = newNode(str);
    temp->next = s->top;
    s->top = temp;
    ++(s->size);
}

stack_node* pop(stack* s){
    if(isEmpty(s))
        return NULL;

    stack_node* temp = s->top;
    s->top = temp->next;
    --(s->size);
    return temp;
}

stack_node* top(stack* s){
    return s->top;
}

