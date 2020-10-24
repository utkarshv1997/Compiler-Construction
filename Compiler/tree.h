/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/

#include <stdio.h>
#include <stdlib.h>

float number_of_parse_tree_nodes;
float size_of_parse_tree;
float number_of_AST_nodes;
float size_of_AST;

struct symbolTableNode;
typedef struct tree_node tree_node;

int semantic_flag;
int number_of_semantic_errors;

typedef struct IdHashTableNode{
    struct IdHashTableNode* next;
    int type;
    char* id;
    int row_size,col_size;
    int int_val;
    int string_size;
    int is_assigned;
    int offset;
}IdHashTableNode;

typedef struct IdHashTable{
    IdHashTableNode* head;
    int size_of_table;
}IdHashTable;

typedef struct FunHashTableNode{
    struct FunHashTableNode* next;
    struct symbolTableNode* funSymbolTable;
    char* fId;
}FunHashTableNode;

typedef struct FunHashTable{
    FunHashTableNode* head;
    int size_of_table;
}FunHashTable;

typedef struct symbolTableNode{
    IdHashTable* idTable;
    char *funId;
    int nest_level;
    FunHashTable* funTable;
    tree_node* input_list;
    tree_node* output_list;
    struct symbolTableNode* parent;
    struct symbolTableNode* nextSibling;
    struct symbolTableNode* firstChild;
}symbolTableNode;

typedef struct quadruple{
	IdHashTableNode* tvar;
	IdHashTableNode* left;
	IdHashTableNode* right;
	char* op;
	struct quadruple* next;
	float num_val;
	char* str_val;
	int mat_offset;
	char* matid;
	char* label;
	char* go_label;
	int lab;
	int go;
}quadruple;

typedef struct tree_node{
    char *lexeme;
    char *actual_lex;
    int line_num;
    int is_num;
    struct tree_node* nextSibling;
    struct tree_node* firstChild;
    struct tree_node* parent;
    int is_terminal;
    int rule_num;
    struct tree_node* addr;
    int type;
    IdHashTableNode* tempVar;
    quadruple* code;
}tree_node;

typedef struct tree{
    tree_node* root;
}tree;

tree* createTree();
tree_node* newTreeNode(char* lex,int isTerm);
void printParseTreeInSequence(tree_node* root);
void printParseTreeInOrder(tree_node* root);
void printAST(tree_node* root);
void calAST(tree_node* root);

