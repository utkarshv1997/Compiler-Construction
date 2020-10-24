/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/
  
#include "codegen.h"
#include <stdlib.h>

symbolTableNode* currSym;
int sid = 193;

char* eop = "=";
char* minop = "-";
char* mulop = "*";
char* plusop = "+";
char* divop = "/";
int counter = 1;
int lab_counter = 1;

char* getTempVariable(){
	char *str1 = (char*)malloc(sizeof(char)*10);
	sprintf(str1,"t%d",counter);
	++counter;
	return str1;
}

char* getLabel(){
	char *str1 = (char*)malloc(sizeof(char)*10);
	sprintf(str1,"l%d",lab_counter);
	++lab_counter;
	return str1;
}

IdHashTableNode* getIdNode(char* id){
	IdHashTable* idtabletemp = currSym->idTable;
	int inx = hashVal(id) % sid;
	IdHashTableNode* idtemp = idtabletemp[inx].head;
	while(idtemp != NULL){
		if(strcmp(idtemp->id,id) == 0){
			return idtemp;
		}
		idtemp = idtemp->next;	
	}
	return NULL;
}

quadruple* generateQuad(char* op,IdHashTableNode* left,IdHashTableNode* right,IdHashTableNode* tvar){
	quadruple* temp = (quadruple*)malloc(sizeof(quadruple));
	temp->left = left;
	temp->right = right;
	temp->tvar = tvar;
	temp->op = op;
	temp->next = NULL;
	temp->num_val = -1;
	temp->str_val = NULL;
	temp->mat_offset = -1;
	temp->matid = NULL;
	temp->label = NULL;
	temp->go_label = NULL;
	return temp;
}

int getTypes(char* str){
    if(strcmp(str,"INT") == 0 || strcmp(str,"NUM") == 0)
        return 1;
    else if(strcmp(str,"REAL") == 0 || strcmp(str,"RNUM") == 0)
        return 2;
    else if(strcmp(str,"STRING") == 0 || strcmp(str,"STR") == 0)
        return 3;
    else if(strcmp(str,"MATRIX") == 0)
        return 4;
    else
        return -1;
}

quadruple* augmentCode(quadruple* q1,quadruple* q2,quadruple* q3){
	if(q1 == NULL && q2 == NULL){
		return q3;
	}
	
	if(q1 == NULL){
		quadruple* temp = q2;
		while(temp->next != NULL)
			temp = temp->next;
		temp->next = q3;
		return q2;
	}
	
	if(q2 == NULL){
		quadruple* temp = q1;
		while(temp->next != NULL)
			temp = temp->next;
		temp->next = q3;
		return q1;
	}
	
	quadruple* temp = q1;
	while(temp->next != NULL)
		temp = temp->next;
	temp->next = q2;
	while(temp->next != NULL)
		temp = temp->next;
	temp->next = q3;	
	return q1;	
}

int calType(tree_node* root){
	if(root == NULL){
		printf("NO TYPE FOUND..\n");
		return -1;
	}

	if(root->firstChild == NULL){
		if(strcmp(root->lexeme,"NUM") == 0)
			return 1;
			
		else if(strcmp(root->lexeme,"RNUM") == 0)
			return 2;
			
		else if(strcmp(root->lexeme,"STR") == 0)	
			return 3;
			
		int temp = getIdNode(root->actual_lex)->type;
		if(temp == 4 && root->nextSibling->firstChild != NULL){
			return 1;
		}
		else{
			return temp;
		}
	}	
	calType(root->firstChild);	
}

void printCode(quadruple* head){
	quadruple* temp = head;
	while(temp != NULL){
		if(temp->mat_offset != -1 && temp->matid != NULL){
			if(temp->str_val != NULL)
				printf("%s[%d] = %s\n",temp->matid,temp->mat_offset,temp->str_val);
			else
				printf("%s = %s[%d] //%d is the offset wrt the matrix.\n",temp->tvar->id,temp->matid,temp->mat_offset,temp->mat_offset);
		}
		
		else if(temp->mat_offset != -1)
			printf("%s = %s[%d] //%d is the offset wrt the matrix.\n", temp->tvar->id,temp->matid,temp->mat_offset,temp->mat_offset);
		
		else if(temp->num_val != -1 && temp->right == NULL)
			printf("%s = %0.2f\n",temp->tvar->id,temp->num_val);
		
		else if(temp->str_val != NULL && temp->right == NULL)
			printf("%s = %s\n",temp->tvar->id,temp->str_val);
		
		else if(temp->right == NULL)
			printf("%s = %s\n",temp->tvar->id,temp->left->id);
		
		else if(temp->left == NULL)
			printf("%s = %s %s\n",temp->tvar->id,temp->op,temp->right->id);
			
		else
			printf("%s = %s %s %s\n",temp->tvar->id,temp->left->id,temp->op,temp->right->id);	
		
		temp = temp->next;
	}	
}

void handleIfExpr(tree_node* root){
	if(root == NULL)
		return;
	
	if(root->is_terminal == 0)
		return;
			
	handleIfExpr(root->firstChild);
	
	if(root->is_num == 1){
		char* tv = getTempVariable();
		int val,type;
		if(strcmp(root->lexeme,"NUM") == 0){
			val = atoi(root->actual_lex);
			type = 1;
		}
		
		else{
			val = atof(root->actual_lex);
			type = 2;
		}
		
		IdHashTableNode* tvar = insertInIdTable(currSym->idTable,tv,type,root->line_num,-1);
		quadruple* qtemp = generateQuad(eop,NULL,NULL,tvar);
		qtemp->num_val = val;
		qtemp->next = NULL;
		qtemp->left = qtemp->right = NULL;	
		qtemp->label = NULL;
		qtemp->go_label = NULL;
		qtemp->str_val = NULL;
		qtemp->mat_offset = -1;
		qtemp->matid = NULL;
		root->tempVar = tvar;
		root->code = qtemp;
	}
	
	else if(root->firstChild == NULL){
		root->code = NULL;
		root->tempVar = getIdNode(root->actual_lex);
	}
	
	else{
		char* tv = getTempVariable();
		int type = root->firstChild->tempVar->type;
		IdHashTableNode* tvar = insertInIdTable(currSym->idTable,tv,type,root->line_num,-1);
		root->tempVar = tvar;
		tree_node* right;
		right = root->firstChild->nextSibling;
		if(right != NULL){
			root->code = generateQuad(root->actual_lex,root->firstChild->tempVar,right->tempVar,tvar);	
			root->code = augmentCode(root->firstChild->code,right->code,root->code);
		}
		else{
			root->code = generateQuad(root->actual_lex,NULL,root->firstChild->tempVar,tvar);
			root->code = augmentCode(root->firstChild->code,NULL,root->code);	
		}
		
	}
	
	handleIfExpr(root->nextSibling);
}

void handleArithmeticExpr(tree_node* root,int type){
	if(root == NULL)
		return;
	
	if(root->is_terminal == 0){
		handleArithmeticExpr(root->nextSibling,type);
		return;
	}
				
	handleArithmeticExpr(root->firstChild,type);
	
	if(root->is_num == 1){
		char* tv = getTempVariable();
		IdHashTableNode* tvar = insertInIdTable(currSym->idTable,tv,type,root->line_num,-1);
		float val;
		if(strcmp(root->lexeme,"NUM") == 0){
			val = atoi(root->actual_lex);
		}
		else{
			val = atof(root->actual_lex);
		}
		quadruple* qtemp = generateQuad(eop,NULL,NULL,tvar);
		qtemp->num_val = val;
		qtemp->next = NULL;
		qtemp->left = qtemp->right = NULL;
		qtemp->label = NULL;
		qtemp->go_label = NULL;
		qtemp->str_val = NULL;
		qtemp->mat_offset = -1;
		qtemp->matid = NULL;
		root->tempVar = tvar;
		root->code = qtemp;
	}
	
	else if(strcmp(root->lexeme,"STR") == 0){
		char* tv = getTempVariable();
		IdHashTableNode* tvar = insertInIdTable(currSym->idTable,tv,type,root->line_num,-1);
		char* val = root->actual_lex;
		quadruple* qtemp = generateQuad(eop,NULL,NULL,tvar);
		qtemp->str_val = val;
		qtemp->next = NULL;
		qtemp->left = qtemp->right = NULL;
		qtemp->label = NULL;
		qtemp->go_label = NULL;
		qtemp->mat_offset = -1;
		qtemp->matid = NULL;
		root->tempVar = tvar;
		root->code = qtemp;
	}
	
	else if(root->firstChild == NULL){
		if(root->nextSibling->addr != NULL){
			IdHashTableNode* idnode = getIdNode(root->actual_lex);
			int cols = idnode->col_size;
			int r = atoi(root->nextSibling->addr->actual_lex);
			int c = atoi(root->nextSibling->addr->nextSibling->actual_lex);
			int off = (r*cols + c)*2;
			char* tv = getTempVariable();
			IdHashTableNode* tvar = insertInIdTable(currSym->idTable,tv,1,root->line_num,-1);
			quadruple* qtemp = generateQuad(eop,NULL,NULL,tvar);
			qtemp->next = NULL;
			qtemp->left = qtemp->right = NULL;
			qtemp->mat_offset = off;
			qtemp->matid = root->actual_lex;
			qtemp->label = NULL;
			qtemp->go_label = NULL;
			root->tempVar = tvar;
			root->code = qtemp;
		}
		else{
			root->code = NULL;
			root->tempVar = getIdNode(root->actual_lex);
		}
	}
	
	else{
		char* tv = getTempVariable();
		IdHashTableNode* tvar = insertInIdTable(currSym->idTable,tv,type,root->line_num,-1);
		root->tempVar = tvar;
		tree_node* right;
		if(root->firstChild->nextSibling->is_terminal == 0){
			right = root->firstChild->nextSibling->nextSibling;
		}
		else
			right = root->firstChild->nextSibling;
		root->code = generateQuad(root->actual_lex,root->firstChild->tempVar,right->tempVar,tvar);	
		root->code = augmentCode(root->firstChild->code,right->code,root->code);
	}
	
	handleArithmeticExpr(root->nextSibling,type);
}

void handleMatrix(){};	

int calculateStringLength(tree_node* root){
	if(root == NULL)
		return 0;
			
	if(root->is_terminal == 0){
		return calculateStringLength(root->nextSibling);
	}
		
	int temp = 0;	
	if(strcmp(root->lexeme,"STR") == 0)
		temp = strlen(root->actual_lex);
		
	else if(strcmp(root->lexeme,"ID") == 0)
		temp = getIdNode(root->actual_lex)->string_size;
		
	return temp + calculateStringLength(root->firstChild) + calculateStringLength(root->nextSibling);
}

void handleIfStmts(tree_node* root);


void handleElsePart(tree_node* root){
	tree_node* temp = root->addr;
	tree_node* tempnode;
	IdHashTableNode* idnodetemp;
	if(temp == NULL)
		return;
		
	while(temp != NULL){
	//printf("hep:%d\n",temp->rule_num);
		switch(temp->rule_num){
			case 26:
				if(temp->addr->firstChild->nextSibling->rule_num == 48){
					if(temp->addr->firstChild->nextSibling->addr->is_terminal == 1){
						int cal=calType(temp->addr->firstChild->nextSibling->addr);
						handleArithmeticExpr(temp->addr->firstChild->nextSibling->addr,cal);
						tree_node* left = temp->addr->firstChild->nextSibling->addr;
						temp->code = generateQuad(eop,left->tempVar,NULL,getIdNode(temp->addr->firstChild->actual_lex));	
						temp->code = augmentCode(left->code,NULL,temp->code);
						if(cal == 3){
							int str_size = calculateStringLength(temp->addr->firstChild->nextSibling->addr);
							IdHashTableNode* idnode = getIdNode(temp->addr->firstChild->actual_lex);
							idnode->string_size = str_size;
						}
						if(cal == 4){
							IdHashTableNode* idnode = getIdNode(temp->addr->firstChild->actual_lex);
							tree_node* idtemp = temp->addr->firstChild->nextSibling->addr;
							if(strcmp(idtemp->lexeme,"ID") != 0){
								idtemp = idtemp->firstChild;
							}
							IdHashTableNode* idnodetemp = getIdNode(idtemp->actual_lex);
							idnode->row_size = idnodetemp->row_size;
							idnode->col_size = idnodetemp->col_size;
						}
					}
					else{
						handleMatrix();
					}
				}
				else if(temp->addr->firstChild->nextSibling->rule_num == 35){
					quadruple *qtemp = (quadruple*)malloc(sizeof(quadruple));
					qtemp->next = NULL;
					qtemp->left = qtemp->right = qtemp->tvar = NULL;
					qtemp->tvar = getIdNode(temp->addr->firstChild->actual_lex);	
					qtemp->op = eop;
					IdHashTableNode* idnode = getIdNode(temp->addr->firstChild->nextSibling->addr->nextSibling->actual_lex);
					qtemp->num_val = idnode->string_size;
					qtemp->mat_offset = -1;
					qtemp->str_val = NULL;
					qtemp->go_label = NULL;
					qtemp->label = NULL;
					qtemp->matid = NULL;
					temp->code = qtemp;
				}
				break;
				
			case 41:
			case 42:
				idnodetemp = getIdNode(temp->addr->nextSibling->actual_lex);
				if(idnodetemp->row_size != -1){
					int col_size = idnodetemp->col_size;
					int row_size = idnodetemp->row_size;
					int offset = 0;
					int i = 0,j = 0;
					IdHashTableNode* idnode = getIdNode(temp->addr->nextSibling->actual_lex);
					while(i < row_size){
						j = 0;
						while(j < col_size){
							offset = (i*col_size + j)*2; 
							quadruple* qtemp = generateQuad(eop,NULL,NULL,idnode);
							qtemp->mat_offset = offset;
							qtemp->matid = idnode->id;
							qtemp->str_val = temp->addr->actual_lex;
							qtemp->label = NULL;
							qtemp->go_label = NULL;
							temp->code = augmentCode(temp->code,NULL,qtemp);
							++j;
						}
						++i;
					}	
				}
				else{
					temp->code = generateQuad(eop,NULL,NULL,getIdNode(temp->addr->nextSibling->actual_lex)); 
					temp->code->str_val = temp->addr->actual_lex;
				}	
				break;
			
			case 36:
				handleIfExpr(temp->addr);
				temp->code = temp->addr->code;
				handleIfStmts(temp->addr->nextSibling);
				tempnode = temp->addr;
				int flag = 0;
				char* lab = getLabel();
				char* golab = getLabel();
				while(tempnode->nextSibling != NULL){
					quadruple* qtemp = tempnode->code;
					while(qtemp->next != NULL)
						qtemp = qtemp->next;
					if(flag == 0){
						qtemp->go_label = lab;
						flag = 1;
					}
					if(tempnode->nextSibling->nextSibling == NULL){
						qtemp->go_label = golab; 
						qtemp->label = lab;
					}
					qtemp->next = tempnode->nextSibling->code;		
					tempnode = tempnode->nextSibling;
				}	
				if(tempnode->nextSibling == NULL){
					quadruple* qtemp = tempnode->code;
					while(qtemp != NULL && qtemp->next != NULL)
						qtemp = qtemp->next;
					if(qtemp != NULL)
						qtemp->label = golab;
				}
				break;		
			
			case 27:
				if(temp->addr->firstChild->nextSibling->is_terminal == 0){
					quadruple *qtemp = (quadruple*)malloc(sizeof(quadruple));
					qtemp->next = NULL;
					qtemp->left = qtemp->right = qtemp->tvar = NULL;
					qtemp->tvar = getIdNode(temp->addr->firstChild->actual_lex);	
					qtemp->op = eop;
					IdHashTableNode* idnode = getIdNode(temp->addr->firstChild->nextSibling->addr->nextSibling->actual_lex);
					qtemp->num_val = idnode->string_size;
					qtemp->mat_offset = -1;
					qtemp->str_val = NULL;
					qtemp->label = NULL;
					qtemp->go_label = NULL;
					qtemp->matid = NULL;
					temp->code = qtemp;
					printCode(temp->code);
				}
				else{
					quadruple *qtemp = (quadruple*)malloc(sizeof(quadruple));
					qtemp->next = NULL;
					qtemp->left = qtemp->right = qtemp->tvar = NULL;
					qtemp->tvar = getIdNode(temp->addr->firstChild->actual_lex);	
					qtemp->op = eop;
					IdHashTableNode* idnode = getIdNode(temp->addr->firstChild->nextSibling->nextSibling->addr->nextSibling->actual_lex);
					qtemp->num_val = idnode->row_size;
					qtemp->mat_offset = -1;
					qtemp->str_val = NULL;
					qtemp->label = NULL;
					qtemp->go_label = NULL;
					qtemp->matid = NULL;
					temp->code = qtemp;
					
					quadruple *qtemp1 = (quadruple*)malloc(sizeof(quadruple));
					qtemp1->next = NULL;
					qtemp1->left = qtemp1->right = qtemp1->tvar = NULL;
					qtemp1->tvar = getIdNode(temp->addr->firstChild->nextSibling->actual_lex);	
					qtemp1->op = eop;
					idnode = getIdNode(temp->addr->firstChild->nextSibling->nextSibling->addr->nextSibling->actual_lex);
					qtemp1->num_val = idnode->col_size;
					qtemp1->mat_offset = -1;
					qtemp1->str_val = NULL;
					qtemp1->label = NULL;
					qtemp1->matid = NULL;
					qtemp1->go_label = NULL;
					temp->code->next = qtemp1;
				}
				break;
				
			default:
				break;
		}
		temp = temp->nextSibling;
	}
	root->code = root->addr->code;
	tree_node* else_temp = root->addr;
	while(else_temp->nextSibling != NULL){
		quadruple* qtemp = else_temp->code;
		while(qtemp->next != NULL)
			qtemp = qtemp->next;
		qtemp->next = else_temp->nextSibling->code;
		else_temp = else_temp->nextSibling;
	}
}

void handleIfStmts(tree_node* root){
	tree_node* temp = root;
	tree_node* tempnode;
	IdHashTableNode* idnodetemp;
	while(temp->nextSibling != NULL){
		printf("%d\n",temp->rule_num);
		switch(temp->rule_num){
			case 26:
				if(temp->addr->firstChild->nextSibling->rule_num == 48){
					if(temp->addr->firstChild->nextSibling->addr->is_terminal == 1){
						int cal=calType(temp->addr->firstChild->nextSibling->addr);
						handleArithmeticExpr(temp->addr->firstChild->nextSibling->addr,cal);
						tree_node* left = temp->addr->firstChild->nextSibling->addr;
						temp->code = generateQuad(eop,left->tempVar,NULL,getIdNode(temp->addr->firstChild->actual_lex));	
						temp->code = augmentCode(left->code,NULL,temp->code);
						if(cal == 3){
							int str_size = calculateStringLength(temp->addr->firstChild->nextSibling->addr);
							IdHashTableNode* idnode = getIdNode(temp->addr->firstChild->actual_lex);
							idnode->string_size = str_size;
						}
						if(cal == 4){
							IdHashTableNode* idnode = getIdNode(temp->addr->firstChild->actual_lex);
							tree_node* idtemp = temp->addr->firstChild->nextSibling->addr;
							if(strcmp(idtemp->lexeme,"ID") != 0){
								idtemp = idtemp->firstChild;
							}
							IdHashTableNode* idnodetemp = getIdNode(idtemp->actual_lex);
							idnode->row_size = idnodetemp->row_size;
							idnode->col_size = idnodetemp->col_size;
						}
					}
					else{
						handleMatrix();
					}
				}
				else if(temp->addr->firstChild->nextSibling->rule_num == 35){
					quadruple *qtemp = (quadruple*)malloc(sizeof(quadruple));
					qtemp->next = NULL;
					qtemp->left = qtemp->right = qtemp->tvar = NULL;
					qtemp->tvar = getIdNode(temp->addr->firstChild->actual_lex);	
					qtemp->op = eop;
					IdHashTableNode* idnode = getIdNode(temp->addr->firstChild->nextSibling->addr->nextSibling->actual_lex);
					qtemp->num_val = idnode->string_size;
					qtemp->mat_offset = -1;
					qtemp->str_val = NULL;
					qtemp->label = NULL;
					qtemp->matid = NULL;
					qtemp->go_label = NULL;
					temp->code = qtemp;
					printCode(temp->code);	
				}
				break;
				
			case 41:
			case 42:
				idnodetemp = getIdNode(temp->addr->nextSibling->actual_lex);
				if(idnodetemp->row_size != -1){
					int col_size = idnodetemp->col_size;
					int row_size = idnodetemp->row_size;
					int offset = 0;
					int i = 0,j = 0;
					IdHashTableNode* idnode = getIdNode(temp->addr->nextSibling->actual_lex);
					while(i < row_size){
						j = 0;
						while(j < col_size){
							offset = (i*col_size + j)*2; 
							quadruple* qtemp = generateQuad(eop,NULL,NULL,idnode);
							qtemp->mat_offset = offset;
							qtemp->matid = idnode->id;
							qtemp->str_val = temp->addr->actual_lex;
							qtemp->label= NULL;
							qtemp->go_label = NULL;
							temp->code = augmentCode(temp->code,NULL,qtemp);
							++j;
						}
						++i;
					}	
				}
				else{
					temp->code = generateQuad(eop,NULL,NULL,getIdNode(temp->addr->nextSibling->actual_lex)); 
					temp->code->str_val = temp->addr->actual_lex;
				}	
				break;
			
			case 36:
				handleIfExpr(temp->addr);
				temp->code = temp->addr->code;
				handleIfStmts(temp->addr->nextSibling);
				tempnode = temp->addr;
				int flag = 0;
				char* lab = getLabel();
				char* golab = getLabel();
				while(tempnode->nextSibling != NULL){
					quadruple* qtemp = tempnode->code;
					while(qtemp->next != NULL)
						qtemp = qtemp->next;
					if(flag == 0){
						qtemp->go_label = lab;
						flag = 1;
					}
					if(tempnode->nextSibling->nextSibling == NULL){
						qtemp->go_label = golab; 
						qtemp->label = lab;
					}
					qtemp->next = tempnode->nextSibling->code;		
					tempnode = tempnode->nextSibling;
				}	
				if(tempnode->nextSibling == NULL){
					quadruple* qtemp = tempnode->code;
					while(qtemp != NULL && qtemp->next != NULL)
						qtemp = qtemp->next;
					if(qtemp != NULL)
						qtemp->label = golab;
				}
				break;	
			
			case 27:
				if(temp->addr->firstChild->nextSibling->is_terminal == 0){
					quadruple *qtemp = (quadruple*)malloc(sizeof(quadruple));
					qtemp->next = NULL;
					qtemp->left = qtemp->right = qtemp->tvar = NULL;
					qtemp->tvar = getIdNode(temp->addr->firstChild->actual_lex);	
					qtemp->op = eop;
					IdHashTableNode* idnode = getIdNode(temp->addr->firstChild->nextSibling->addr->nextSibling->actual_lex);
					qtemp->num_val = idnode->string_size;
					qtemp->mat_offset = -1;
					qtemp->str_val = NULL;
					qtemp->label = NULL;
					qtemp->go_label = NULL;
					qtemp->matid = NULL;
					temp->code = qtemp;
					printCode(temp->code);
				}
				else{
					quadruple *qtemp = (quadruple*)malloc(sizeof(quadruple));
					qtemp->next = NULL;
					qtemp->left = qtemp->right = qtemp->tvar = NULL;
					qtemp->tvar = getIdNode(temp->addr->firstChild->actual_lex);	
					qtemp->op = eop;
					IdHashTableNode* idnode = getIdNode(temp->addr->firstChild->nextSibling->nextSibling->addr->nextSibling->actual_lex);
					qtemp->num_val = idnode->row_size;
					qtemp->mat_offset = -1;
					qtemp->str_val = NULL;
					qtemp->label = NULL;
					qtemp->go_label = NULL;
					qtemp->matid = NULL;
					temp->code = qtemp;
					
					quadruple *qtemp1 = (quadruple*)malloc(sizeof(quadruple));
					qtemp1->next = NULL;
					qtemp1->left = qtemp1->right = qtemp1->tvar = NULL;
					qtemp1->tvar = getIdNode(temp->addr->firstChild->nextSibling->actual_lex);	
					qtemp1->op = eop;
					idnode = getIdNode(temp->addr->firstChild->nextSibling->nextSibling->addr->nextSibling->actual_lex);
					qtemp1->num_val = idnode->col_size;
					qtemp1->mat_offset = -1;
					qtemp1->str_val = NULL;
					qtemp1->label = NULL;
					qtemp1->go_label = NULL;
					qtemp1->matid = NULL;
					temp->code->next = qtemp1;
					printCode(temp->code);
				}
				break;
				
			default:
				break;
		}
		temp = temp->nextSibling;
	}
	handleElsePart(temp);
}

void printIfCode(quadruple* head){
	quadruple* temp = head;
	while(temp != NULL){
		if(temp->mat_offset != -1 && temp->matid != NULL){
			if(temp->str_val != NULL)
				printf("%s[%d] = %s\n",temp->matid,temp->mat_offset,temp->str_val);
			else
				printf("%s = %s[%d] //%d is the offset wrt the matrix.\n",temp->tvar->id,temp->matid,temp->mat_offset,temp->mat_offset);
		}
		
		else if(temp->mat_offset != -1)
			printf("%s = %s[%d] //%d is the offset wrt the matrix.\n", temp->tvar->id,temp->matid,temp->mat_offset,temp->mat_offset);
		
		
		else if(temp->num_val != -1 && temp->right == NULL)
			printf("%s = %0.2f\n",temp->tvar->id,temp->num_val);
				
		else if(temp->str_val != NULL && temp->right == NULL)
			printf("%s = %s\n",temp->tvar->id,temp->str_val);
		
		else if(temp->right == NULL)
			printf("%s = %s\n",temp->tvar->id,temp->left->id);
		
		else if(temp->left == NULL)
			printf("%s = %s %s\n",temp->tvar->id,temp->op,temp->right->id);
			
		else
			printf("%s = %s %s %s\n",temp->tvar->id,temp->left->id,temp->op,temp->right->id);	
		
		if(temp->go_label != NULL)
			printf("go to %s\n",temp->go_label);
		
		if(temp->label != NULL)
			printf("%s : \n",temp->label);
			
		temp = temp->next;
	}
}

void assignOffToTemps(){
	IdHashTable* temp = currSym->idTable;
	int inx = 0;
	IdHashTableNode* idtemp;
	for(inx=0;inx<sid;++inx){
		idtemp = temp[inx].head;
		while(idtemp != NULL){
			if(idtemp->offset == -1){
				idtemp->offset = main_offset;
				main_offset += getwidth(idtemp,idtemp->type);
			}
			idtemp = idtemp->next;
		}		
	}
}

void generateIntCode(tree_node* root){
	currSym = symRoot;
	tree_node* temp = root->addr;
	IdHashTableNode* idnodetemp;
	tree_node* tempnode;
	while(temp != NULL){
		//printf("%d\n",temp->rule_num);
		switch(temp->rule_num){
			case 26:
				if(temp->addr->firstChild->nextSibling->rule_num == 48){
					if(temp->addr->firstChild->nextSibling->addr->is_terminal == 1){
						int cal=calType(temp->addr->firstChild->nextSibling->addr);
						handleArithmeticExpr(temp->addr->firstChild->nextSibling->addr,cal);
						tree_node* left = temp->addr->firstChild->nextSibling->addr;
						temp->code = generateQuad(eop,left->tempVar,NULL,getIdNode(temp->addr->firstChild->actual_lex));	
						temp->code = augmentCode(left->code,NULL,temp->code);
						printCode(temp->code);
						if(cal == 3){
							int str_size = calculateStringLength(temp->addr->firstChild->nextSibling->addr);
							IdHashTableNode* idnode = getIdNode(temp->addr->firstChild->actual_lex);
							idnode->string_size = str_size;
						}
						if(cal == 4){
							IdHashTableNode* idnode = getIdNode(temp->addr->firstChild->actual_lex);
							tree_node* idtemp = temp->addr->firstChild->nextSibling->addr;
							if(strcmp(idtemp->lexeme,"ID") != 0){
								idtemp = idtemp->firstChild;
							}
							IdHashTableNode* idnodetemp = getIdNode(idtemp->actual_lex);
							idnode->row_size = idnodetemp->row_size;
							idnode->col_size = idnodetemp->col_size;
						}
					}
					else{
						handleMatrix();
					}
				}
				else if(temp->addr->firstChild->nextSibling->rule_num == 35){
					quadruple *qtemp = (quadruple*)malloc(sizeof(quadruple));
					qtemp->next = NULL;
					qtemp->left = qtemp->right = qtemp->tvar = NULL;
					qtemp->tvar = getIdNode(temp->addr->firstChild->actual_lex);	
					qtemp->op = eop;
					IdHashTableNode* idnode = getIdNode(temp->addr->firstChild->nextSibling->addr->nextSibling->actual_lex);
					qtemp->num_val = idnode->string_size;
					qtemp->mat_offset = -1;
					qtemp->str_val = NULL;
					qtemp->label = NULL;
					qtemp->go_label = NULL;
					qtemp->go_label = NULL;
					qtemp->matid = NULL;
					temp->code = qtemp;
					printCode(temp->code);
				}
				break;
				
			case 41:
			case 42:
				idnodetemp = getIdNode(temp->addr->nextSibling->actual_lex);
				if(idnodetemp->row_size != -1){
					int col_size = idnodetemp->col_size;
					int row_size = idnodetemp->row_size;
					int offset = 0;
					int i = 0,j = 0;
					IdHashTableNode* idnode = getIdNode(temp->addr->nextSibling->actual_lex);
					while(i < row_size){
						j = 0;
						while(j < col_size){
							offset = (i*col_size + j)*2; 
							quadruple* qtemp = generateQuad(eop,NULL,NULL,idnode);
							qtemp->mat_offset = offset;
							qtemp->matid = idnode->id;
							qtemp->str_val = temp->addr->actual_lex;
							qtemp->label = NULL;
							qtemp->go_label = NULL;
							qtemp->go_label = NULL;
							temp->code = augmentCode(temp->code,NULL,qtemp);
							++j;
						}
						++i;
					}	
					
				}
				else{
					temp->code = generateQuad(eop,NULL,NULL,getIdNode(temp->addr->nextSibling->actual_lex)); 
					temp->code->str_val = temp->addr->actual_lex;
				}	
				printCode(temp->code);
				break;
			
			case 36:
				handleIfExpr(temp->addr);
				temp->code = temp->addr->code;
				handleIfStmts(temp->addr->nextSibling);
				tempnode = temp->addr;
				int flag = 0;
				char* lab = getLabel();
				char* golab = getLabel();
				while(tempnode->nextSibling != NULL){
					quadruple* qtemp = tempnode->code;
					while(qtemp->next != NULL)
						qtemp = qtemp->next;
					if(flag == 0){
						qtemp->go_label = lab;
						flag = 1;
					}
					if(tempnode->nextSibling->nextSibling == NULL){
						qtemp->go_label = golab; 
						qtemp->label = lab;
					}
					qtemp->next = tempnode->nextSibling->code;		
					tempnode = tempnode->nextSibling;
				}	
				if(tempnode->nextSibling == NULL){
					quadruple* qtemp = tempnode->code;
					while(qtemp != NULL && qtemp->next != NULL)
						qtemp = qtemp->next;
					if(qtemp != NULL)
						qtemp->label = golab;
				}
				printIfCode(temp->code);
				break;	
			
			case 27:
				if(temp->addr->firstChild->nextSibling->is_terminal == 0){
					quadruple *qtemp = (quadruple*)malloc(sizeof(quadruple));
					qtemp->next = NULL;
					qtemp->left = qtemp->right = qtemp->tvar = NULL;
					qtemp->tvar = getIdNode(temp->addr->firstChild->actual_lex);	
					qtemp->op = eop;
					IdHashTableNode* idnode = getIdNode(temp->addr->firstChild->nextSibling->addr->nextSibling->actual_lex);
					qtemp->num_val = idnode->string_size;
					qtemp->mat_offset = -1;
					qtemp->str_val = NULL;
					qtemp->label = NULL;
					qtemp->go_label = NULL;
					qtemp->matid = NULL;
					temp->code = qtemp;
					printCode(temp->code);
				}
				else{
					quadruple *qtemp = (quadruple*)malloc(sizeof(quadruple));
					qtemp->next = NULL;
					qtemp->left = qtemp->right = qtemp->tvar = NULL;
					qtemp->tvar = getIdNode(temp->addr->firstChild->actual_lex);	
					qtemp->op = eop;
					IdHashTableNode* idnode = getIdNode(temp->addr->firstChild->nextSibling->nextSibling->addr->nextSibling->actual_lex);
					qtemp->num_val = idnode->row_size;
					qtemp->mat_offset = -1;
					qtemp->str_val = NULL;
					qtemp->label = NULL;
					qtemp->go_label = NULL;
					qtemp->matid = NULL;
					temp->code = qtemp;
					
					quadruple *qtemp1 = (quadruple*)malloc(sizeof(quadruple));
					qtemp1->next = NULL;
					qtemp1->left = qtemp1->right = qtemp1->tvar = NULL;
					qtemp1->tvar = getIdNode(temp->addr->firstChild->nextSibling->actual_lex);	
					qtemp1->op = eop;
					idnode = getIdNode(temp->addr->firstChild->nextSibling->nextSibling->addr->nextSibling->actual_lex);
					qtemp1->num_val = idnode->col_size;
					qtemp1->mat_offset = -1;
					qtemp1->str_val = NULL;
					qtemp1->label = NULL;
					qtemp1->go_label = NULL;
					qtemp1->matid = NULL;
					temp->code->next = qtemp1;
					printCode(temp->code);
				}
				break;
				
			default:
				break;
		}
		temp = temp->nextSibling;
	}
	assignOffToTemps();
}





