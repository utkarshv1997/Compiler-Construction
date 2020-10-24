/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/

#include "tree.h"

tree* createTree(){
    tree* temp = (tree*)malloc(sizeof(tree));
    temp->root = NULL;
    return temp;
}

tree_node* newTreeNode(char* lex,int isTerm){
	++number_of_parse_tree_nodes;
	size_of_parse_tree += sizeof(tree_node);
    tree_node* temp = (tree_node*)malloc(sizeof(tree_node));
    temp->is_terminal = isTerm;
    temp->lexeme = lex;
    temp->line_num = -1;
    temp->is_num = 0;
    temp->firstChild = temp->nextSibling = temp->parent = temp->addr = NULL;
    temp->type = -1;
    temp->code = NULL;
    temp->tempVar = NULL;
    return temp;
}

void calAST(tree_node* root){
    if(root == NULL)
        return;
        
    ++number_of_AST_nodes;
    size_of_AST += sizeof(tree_node);

    if(root->firstChild != NULL || root->addr!= NULL){
        tree_node* temp = root->addr;

        if(temp == NULL){
            temp = root->firstChild;
        }
        else{
            ++number_of_AST_nodes;
            size_of_AST += sizeof(tree_node);
            temp = temp->nextSibling;
        }

        while(temp != NULL){
            temp = temp->nextSibling;
        }


        if(root->addr == NULL)
            calAST(root->firstChild);
        else
            calAST(root->addr);
    }

    calAST(root->nextSibling);
}

void printAST(tree_node* root){
    if(root == NULL)
        return;

    if(root->firstChild != NULL || root->addr!= NULL){
        printf("%s => ",root->lexeme);
        tree_node* temp = root->addr;

        if(temp == NULL){
            temp = root->firstChild;
        }
        else{
            printf("%s ",temp->lexeme);
            temp = temp->nextSibling;
        }

        while(temp != NULL){
            printf("%s ",temp->lexeme);
            temp = temp->nextSibling;
        }

        printf("\n");

        if(root->addr == NULL)
            printAST(root->firstChild);
        else
            printAST(root->addr);
    }

    printAST(root->nextSibling);
}

void printParseTreeInSequence(tree_node* root){
    if(root == NULL)
        return;

    if(root->is_terminal == 0){
        printf("(%d)%s => ",root->rule_num,root->lexeme);
        tree_node* temp = root->firstChild;

        while(temp != NULL){
            printf("%s ",temp->lexeme);
            temp = temp->nextSibling;
        }
        printf("\n");
        printParseTreeInSequence(root->firstChild);
    }

    printParseTreeInSequence(root->nextSibling);
}

void printParseTreeInOrder(tree_node* root){
    if(root == NULL)
        return;

    printParseTreeInOrder(root->firstChild);

    if(root->is_terminal && root->line_num != -1){
        //printf("Token = %s , Line-Number(If a terminal) = %d\n",root->lexeme,root->line_num);
        if(root->is_num == 1)
            printf("lexemeCurrentNode => %s , Line-Num=> %d , Token=> %s , valueifNum => %s , parent => %s , isLeafNode => yes , Node-symbol = ---\n",root->actual_lex,root->line_num,root->lexeme,root->actual_lex,root->parent->lexeme);

        else
            printf("lexemeCurrentNode => %s , Line-Num=> %d , Token=> %s , valueifNum => --- , parent => %s , isLeafNode => yes , Node-symbol = ---\n",root->actual_lex,root->line_num,root->lexeme,root->parent->lexeme);
    }
    else{
        //printf("Token = %s\n",root->lexeme);
        if(root->parent != NULL)
            printf("lexemeCurrentNode => --- , Line-Num=> --- , Token=> --- , valueifNum => --- , parent => %s , isLeafNode => no , Node-symbol = %s\n",root->parent->lexeme,root->lexeme);

        else
            printf("lexemeCurrentNode => --- , Line-Num=> --- , Token=> --- , valueifNum => --- , parent => ROOT , isLeafNode => no , Node-symbol = %s\n",root->lexeme);
    }
    if(root->firstChild == NULL)
        return;

    tree_node* temp = root->firstChild->nextSibling;

    while(temp != NULL){
        printParseTreeInOrder(temp);
        temp = temp->nextSibling;
    }
}




