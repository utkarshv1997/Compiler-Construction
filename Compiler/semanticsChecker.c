/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/

#include "semanticsChecker.h"

int INT_SIZE = 2;
int REAL_SIZE = 4;
int CHAR_SIZE = 1;

int size_of_idtable1 = 193;
int size_of_funtable1 = 51;

symbolTableNode* createSymbolTable(char* fid,int nlev){
    symbolTableNode* temp = (symbolTableNode*) malloc(sizeof(symbolTableNode));
    temp->funId = fid;
    temp->nest_level = nlev;
    temp->idTable = createIdTable(size_of_idtable1);
    temp->funTable = createFunTable(size_of_funtable1);
    temp->input_list = temp->output_list = NULL;
    temp->parent = temp->firstChild = temp->nextSibling = NULL;
    return temp;
}

int getType(char* str){
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

void addDec(tree_node* root,symbolTableNode* currSym,int *offset){
    int t = getType(root->lexeme);
    root->type = t;
  	int inc_off = 0;
  	if(t == 1)
  		inc_off = 2;
  	else if(t == 2)
  		inc_off = 4;
  		
    tree_node* temp = root->nextSibling;
    while(temp != NULL){
        temp->type = t;
        insertInIdTable(currSym->idTable,temp->actual_lex,t,temp->line_num,*offset);
        *offset += inc_off;
        temp = temp->nextSibling;
    }			
}

void addParameterList(tree_node* root,IdHashTable* table,int *offset){
	tree_node* temp = root;
	int t;
	while(temp != NULL){
		t = getType(temp->lexeme);
		int inc_off = 0;
	  	if(t == 1)
	  		inc_off = 2;
	  	else if(t == 2)
	  		inc_off = 4;
	  	else if(t == 3)
	  		inc_off = strlen(temp->actual_lex);
	  			
		temp = temp->nextSibling;
		insertInIdTable(table,temp->actual_lex,t,temp->line_num,*offset);
		*offset += inc_off;
		temp = temp->nextSibling;
	}
}

symbolTableNode* addFunDec(tree_node* root,symbolTableNode* currSym,int *offset){
    symbolTableNode* funChildSymTable = createSymbolTable(root->actual_lex,currSym->nest_level + 1);
    symbolTableNode* temp = currSym->firstChild;
    if(temp == NULL)
    	currSym->firstChild = funChildSymTable;
    else{
    	while(temp->nextSibling != NULL){
    		temp = temp->nextSibling;
    	}
    	temp->nextSibling = funChildSymTable;
    }
    funChildSymTable->parent = currSym;
    addParameterList(root->firstChild,funChildSymTable->idTable,offset);
    addParameterList(root->firstChild->firstChild,funChildSymTable->idTable,offset);
    insertInFunTable(currSym->funTable,root->actual_lex,funChildSymTable,root->line_num);	
    return funChildSymTable;
}

int getTypeFromSymTable(char* id,symbolTableNode* currSym){
	if(currSym == NULL)
		return -1;	
		
	IdHashTable* temp = currSym->idTable;
	int inx = hashVal(id) % size_of_idtable1;
	IdHashTableNode* idtemp = temp[inx].head;
	while(idtemp != NULL){
		if(strcmp(idtemp->id,id) == 0)
			return idtemp->type;
		idtemp = idtemp->next;	
	}

	return getTypeFromSymTable(id,currSym->parent);
}

IdHashTableNode* getIdTableNode(char* id,symbolTableNode* currSym){
	if(currSym == NULL)
		return NULL;
		
	IdHashTable* idtabletemp = currSym->idTable;
	int inx = hashVal(id) % size_of_idtable1;
	IdHashTableNode* idtemp = idtabletemp[inx].head;
	while(idtemp != NULL){
		if(strcmp(idtemp->id,id) == 0){
			return idtemp;
		}
		idtemp = idtemp->next;	
	}
	
	return getIdTableNode(id,currSym->parent);
}

void addSizeOfMat(char* id,symbolTableNode* currSym,int rsize,int csize,int *offset){
	if(currSym == NULL)
		return;
			
	IdHashTable* temp = currSym->idTable;
	int inx = hashVal(id) % size_of_idtable1;
	IdHashTableNode* idtemp = temp[inx].head;
	while(idtemp != NULL){
		if(strcmp(idtemp->id,id) == 0){
			idtemp->row_size = rsize;
			idtemp->col_size = csize;
			idtemp->offset = *offset;
			*offset += INT_SIZE*rsize*csize;
			return;
		}
		idtemp = idtemp->next;	
	}
	
	addSizeOfMat(id,currSym->parent,rsize,csize,offset);
}

void checkForStrings(tree_node* root,symbolTableNode* currSym,int *strlength){
	if(root == NULL)
		return;
	
	if(root->is_terminal == 1 && strcmp(root->lexeme,"STR") == 0){
		*strlength += strlen(root->actual_lex);
		//printf("len = %d\n",*strlength);
		//return;
	}
		
	else if(root->is_terminal == 1 && (strcmp(root->actual_lex,"*") == 0 || strcmp(root->actual_lex,"/") == 0 || strcmp(root->actual_lex,"-") == 0)){
		semantic_flag = 0; ++number_of_semantic_errors;
		printf("String Multiplication/Division/Subtraction not allowed in line number %d.\n",root->line_num);
	}
	
	else if(root->nextSibling != NULL && root->nextSibling->is_terminal == 0){
		IdHashTableNode* temp = getIdTableNode(root->actual_lex,currSym);
		if(temp != NULL){	
			int l = temp->string_size;

			if(l == -1){
				printf("String %s not defined in line number %d.\n",root->actual_lex,root->line_num);
				semantic_flag = 0; ++number_of_semantic_errors;
			}
			
			else{
				*strlength += l;	
			}
			//printf("al = %s\n",root->actual_lex,strlength);
		}
	}
	
	checkForStrings(root->firstChild,currSym,strlength);
	checkForStrings(root->nextSibling,currSym,strlength);
}

void checkForMatrices(tree_node* root,symbolTableNode* currSym,int *row,int *col){
	if(root == NULL)
		return;
	
	if(root->is_terminal == 1 && (strcmp(root->actual_lex,"*") == 0 || strcmp(root->actual_lex,"/") == 0)){
		semantic_flag = 0; ++number_of_semantic_errors;
		printf("Matrix Multiplication/Division not allowed in line number %d.\n",root->line_num);
	}
		
	if(root->nextSibling != NULL && root->nextSibling->is_terminal == 0){
		IdHashTableNode* temp = getIdTableNode(root->actual_lex,currSym);
		if(temp != NULL){	
			int r = temp->row_size;
			int c = temp->col_size;

			if(r == -1 || c == -1){
				printf("Matrix %s not defined or size incompatible size in line number %d.\n",root->actual_lex,root->line_num);
				semantic_flag = 0; ++number_of_semantic_errors;
			}
			if(*row == 0 && r != -1)
				*row = r;
				
			if(*col == 0 && c != -1)
				*col = c;
				
			if(*row != r || *col != c){
				printf("Incompatible sizes of matrices in line number %d.\n",root->line_num);		
				semantic_flag = 0; ++number_of_semantic_errors;
			}
		}
	}
	
	checkForMatrices(root->firstChild,currSym,row,col);
	checkForMatrices(root->nextSibling,currSym,row,col);
}

int checkArithmeticExpressiontype1(tree_node* root,symbolTableNode* currSym){
	if(root == NULL)
		return -1;

	if(strcmp(root->lexeme,"ID") == 0){
		if(root->nextSibling->is_terminal == 1 || (root->nextSibling->is_terminal == 0 && root->nextSibling->addr == NULL)){
			int temp = getTypeFromSymTable(root->actual_lex,currSym);	
			if(temp == -1){
				printf("Variable %s not declared in line number %d.\n",root->actual_lex,root->line_num);
				semantic_flag = 0; ++number_of_semantic_errors;
			}
			return temp;
		}
		else if(root->nextSibling->is_terminal == 0 && root->nextSibling->addr != NULL){
			int temp = getTypeFromSymTable(root->actual_lex,currSym);	
			if(temp == -1){
				semantic_flag = 0; ++number_of_semantic_errors;
				printf("Variable %s not declared in line number %d.\n",root->actual_lex,root->line_num);
				return temp;
			}
			else if(temp != 4){
				semantic_flag = 0; ++number_of_semantic_errors;
				printf("Expected a type Matrix for %s in line number %d.\n",root->actual_lex,root->line_num);
				return -1;
			}
			
			IdHashTableNode* idtemp = getIdTableNode(root->actual_lex,currSym);
			
			if(idtemp == NULL){
				semantic_flag = 0; ++number_of_semantic_errors;
				printf("Variable %s not declared in line number %d.\n",root->actual_lex,root->line_num);
			}
			
			else if(idtemp->row_size <= atoi(root->nextSibling->addr->actual_lex) || idtemp->col_size <= atoi(root->nextSibling->addr->nextSibling->actual_lex)){
				semantic_flag = 0; ++number_of_semantic_errors;
				printf("%d,%d\n",idtemp->row_size,atoi(root->nextSibling->addr->actual_lex));
				printf("Index out of Bounds Error in matrix element %s in line number %d.\n",root->actual_lex,root->line_num);
				return 1;
			}
			
			else 
				return 1;
		}
	}

	else if(strcmp(root->lexeme,"NUM") == 0)
		return 1;

	else if(strcmp(root->lexeme,"RNUM") == 0)
		return 2;

	else if(strcmp(root->lexeme,"STR") == 0)
		return 3;

	int tLeft,tRight;

	tLeft = checkArithmeticExpressiontype1(root->firstChild,currSym);
	
	if(root->firstChild->nextSibling->is_terminal == 0)
		tRight = checkArithmeticExpressiontype1(root->firstChild->nextSibling->nextSibling,currSym);
	else
		tRight = checkArithmeticExpressiontype1(root->firstChild->nextSibling,currSym);
		
	if(tLeft == -1 && tRight == -1)
		return -1;

	if(tLeft == -1)
		return tRight;

	else if(tRight == -1)
		return tLeft;

	if(tLeft != tRight){
		semantic_flag = 0; ++number_of_semantic_errors;
		printf("Incompatible Operands to operator %s in line %d.\n",root->actual_lex,root->line_num);		
		return -1;
	}	

	return tLeft;
}

symbolTableNode* getFunctionSymbolTable(char* fid,symbolTableNode* currSym){
	if(currSym == NULL)
		return NULL;
		
	FunHashTable* temp = currSym->funTable;
	int inx = hashVal(fid) % size_of_funtable1;
	FunHashTableNode* ftemp = temp[inx].head;
	while(ftemp != NULL){
		if(strcmp(ftemp->fId,fid) == 0)
			return ftemp->funSymbolTable;
		ftemp = ftemp->next;	
	}
	
	return getFunctionSymbolTable(fid,currSym->parent);
}

void checkAssignmentStmtType2(tree_node* lhsptr,tree_node* rhsptr,symbolTableNode* currSym){
	if(rhsptr->rule_num == 35){
		int t2 = getTypeFromSymTable(rhsptr->addr->nextSibling->actual_lex,currSym);
		if(t2 == -1){
			semantic_flag = 0; ++number_of_semantic_errors;
			printf("Variable %s not declared in line number %d.\n",rhsptr->addr->nextSibling->actual_lex,rhsptr->addr->nextSibling->line_num);
		}
		else if(t2 == 3){
			if(lhsptr->nextSibling->rule_num == 0){
				printf("Expected a single identifier in LHS in line number %d, found more than one.\n",lhsptr->line_num);
				semantic_flag = 0; ++number_of_semantic_errors;
			}
			else{
				int t1 = getTypeFromSymTable(lhsptr->actual_lex,currSym);
				if(t1 == -1){
					printf("Variable %s not declared in line number %d.\n",lhsptr->actual_lex,lhsptr->line_num);
					semantic_flag = 0; ++number_of_semantic_errors;
				}
				else if(t1 != 1){
					semantic_flag = 0; ++number_of_semantic_errors;
					printf("Type mismatch error, expected integer on lhs of assignment statement in line number %d.\n",lhsptr->line_num);
				}
			}
		}
		
		else if(t2 == 4){
			if(lhsptr->nextSibling->is_terminal == 0){
				printf("Expected two identifiers in LHS of assignment statement in line number %d, got less.\n",lhsptr->line_num);
				semantic_flag = 0; ++number_of_semantic_errors;	
			}
			else if(lhsptr->nextSibling->nextSibling->is_terminal == 1){
				semantic_flag = 0; ++number_of_semantic_errors;
				printf("Expected two identifiers in LHS of assignment statement in line number %d, got more.\n",lhsptr->line_num);
			}	
			else{
				int t1 = getTypeFromSymTable(lhsptr->actual_lex,currSym);
				if(t1 == -1){
					semantic_flag = 0; ++number_of_semantic_errors;
					printf("Variable %s not declared in line number %d.\n",lhsptr->actual_lex,lhsptr->line_num);
				}
				else if(t1 != 1){
					semantic_flag = 0; ++number_of_semantic_errors;
					printf("Type mismatch error, expected integer on lhs of assignment statement in line number %d.\n",lhsptr->line_num);
				}
				int t2 = getTypeFromSymTable(lhsptr->nextSibling->actual_lex,currSym);
				if(t2 == -1){
					printf("Variable %s not declared in line number %d.\n",lhsptr->nextSibling->actual_lex,lhsptr->line_num);
					semantic_flag = 0; ++number_of_semantic_errors;
				}
				else if(t2 != 1){
					printf("Type mismatch error, expected integer on lhs of assignment statement in line number %d.\n",lhsptr->line_num);
					semantic_flag = 0; ++number_of_semantic_errors;
				}
			}
		}
		
		else{
			printf("Invalid arguement to Operator SIZE of %s in line number %d, expected a string or matrix.\n",rhsptr->addr->nextSibling->actual_lex,rhsptr->addr->nextSibling->line_num);
			semantic_flag = 0; ++number_of_semantic_errors;
		}
	}
	
	else if(rhsptr->rule_num == 43){
		symbolTableNode* symbol_table = getFunctionSymbolTable(rhsptr->addr->actual_lex,currSym);
		if(symbol_table == NULL){
			printf("Function cannot be called before it's definition, function %s() not in scope.\n",rhsptr->addr->actual_lex);
			semantic_flag = 0; ++number_of_semantic_errors;
			return;
		}
		
		tree_node* output_param = symbol_table->output_list;
		tree_node* root = rhsptr->addr;
		
		while(lhsptr->is_terminal == 1){
			if(output_param == NULL){
				printf("Number of output parameters in function call of %s is more than expected in line number %d.\n",root->actual_lex,root->line_num);
				semantic_flag = 0; ++number_of_semantic_errors;
				return;
			}
			
			int t1 = getTypeFromSymTable(lhsptr->actual_lex,currSym);
			
			if(t1 == -1){
				printf("The output argument %s is not defined in function call %s in line number %d.\n",lhsptr->actual_lex,root->actual_lex,root->line_num);	
				semantic_flag = 0; ++number_of_semantic_errors;
			}
				
			int t2 = getType(output_param->lexeme);
			
			if(t1 != t2){
				printf("Type Mismatch error of output parameter %s in function call %s in line number %d.\n",lhsptr->actual_lex,root->actual_lex,root->line_num);
				semantic_flag = 0; ++number_of_semantic_errors;
			}
			
			lhsptr = lhsptr->nextSibling;
			output_param = output_param->nextSibling->nextSibling;	
		}
		
		if(output_param != NULL){
			printf("Number of output parameters in function call of %s is lesser than expected in line number %d.\n",root->actual_lex,root->line_num);
			semantic_flag = 0; ++number_of_semantic_errors;
		}
		
		tree_node* input_param = symbol_table->input_list;
		tree_node* temp = rhsptr->addr->firstChild;
		 
		while(temp != NULL){
			if(input_param == NULL){
				semantic_flag = 0; ++number_of_semantic_errors;
				printf("Number of input parameters in function call of %s is more than expected in line number %d.\n",root->actual_lex,root->line_num);
				return;
			}
		
			int t1;
		
			if(strcmp(temp->lexeme,"NUM") == 0)
				t1 = 1;
			else if(strcmp(temp->lexeme,"RNUM") == 0)
				t1 = 2;
			else if(strcmp(temp->lexeme,"STR") == 0)
				t1 = 3;		
			else if(strcmp(temp->lexeme,"MATRIX") == 0)
				t1 = 4;
			else 	
				t1 = getTypeFromSymTable(temp->actual_lex,currSym);	

			int t2 = getType(input_param->lexeme);
		
			if(t1 != t2){
				printf("Type Mismatch error of parameter %s in function call %s in line number %d.\n",temp->actual_lex,root->actual_lex,root->line_num);
				semantic_flag = 0; ++number_of_semantic_errors;
			}
			
			temp = temp->nextSibling;
			input_param = input_param->nextSibling->nextSibling;
		}
	
		if(input_param != NULL){
			printf("Number of input parameters in function call of %s is lesser than expected in line number %d.\n",root->actual_lex,root->line_num);
			semantic_flag = 0; ++number_of_semantic_errors;
		}
	}
}

void checkFuncCall(tree_node* root,symbolTableNode* currSym,int lhsType){
	
	symbolTableNode* symbol_table = getFunctionSymbolTable(root->actual_lex,currSym);
	if(symbol_table == NULL){
		printf("Function cannot be called before it's definition, function %s() not in scope.\n",root->actual_lex);
		semantic_flag = 0; ++number_of_semantic_errors;
		return;
	}
	tree_node* temp = root->firstChild;
	tree_node* output_param = symbol_table->output_list;
	
	if(lhsType != -1){
		if(output_param->nextSibling->nextSibling != NULL){
			printf("More Number of output parameters for function call %s expected in line number %d.\n",root->actual_lex,root->line_num);
			semantic_flag = 0; ++number_of_semantic_errors;
		}
		else{	
			if(getType(output_param->lexeme) != lhsType){
				printf("Type Mismatch Error in the LHS value and return value of %s call in line number %d.\n",root->actual_lex,root->line_num);	
				semantic_flag = 0; ++number_of_semantic_errors;
			}
		}
	}
	 
	tree_node* input_param = symbol_table->input_list;
	while(temp != NULL){
		if(input_param == NULL){
			printf("Number of input parameters in function call of %s is more than expected in line number %d.\n",root->actual_lex,root->line_num);
			semantic_flag = 0; ++number_of_semantic_errors;
			return;
		}
		int t1;
		
		if(strcmp(temp->lexeme,"NUM") == 0)
			t1 = 1;
		else if(strcmp(temp->lexeme,"RNUM") == 0)
			t1 = 2;
		else if(strcmp(temp->lexeme,"STR") == 0)
			t1 = 3;		
		else if(strcmp(temp->lexeme,"MATRIX") == 0)
			t1 = 4;
		else 	
			t1 = getTypeFromSymTable(temp->actual_lex,currSym);
		
		if(t1 == -1){
			printf("The input argument %s is not defined in function call %s in line number %d.\n",temp->actual_lex,root->actual_lex,root->line_num);
			semantic_flag = 0; ++number_of_semantic_errors;	
		}

		int t2 = getType(input_param->lexeme);
		
		if(t1 != t2){
			printf("Type Mismatch error of parameter %s in function call %s in line number %d.\n",temp->actual_lex,root->actual_lex,root->line_num);
			semantic_flag = 0; ++number_of_semantic_errors;
		}
		
		temp = temp->nextSibling;
		input_param = input_param->nextSibling->nextSibling;
	}
	
	if(input_param != NULL){
		printf("Number of input parameters in function call of %s is lesser than expected in line number %d.\n",root->actual_lex,root->line_num);	
		semantic_flag = 0; ++number_of_semantic_errors;	
	}
}

void checkifCond(tree_node* root,symbolTableNode* currSym){
	if(root == NULL)
		return; 
	
	if(root->is_terminal == 0)
		return;
	
	int isID1=0,isID2=0;
		
	if(root->firstChild == NULL){
		int t1,t2; 
		if(strcmp(root->lexeme,"ID") == 0){
			t1 = getTypeFromSymTable(root->actual_lex,currSym);
			isID1 = 1;	
		}
		else
			t1 = getType(root->lexeme);
			
		if(strcmp(root->nextSibling->lexeme,"ID") == 0){
			t2 = getTypeFromSymTable(root->nextSibling->actual_lex,currSym);
			isID2 = 1;
		}
		else
			t2 = getType(root->nextSibling->lexeme);

		if(t1 == -1 || t2 == -1){
			if(t1 == -1)
				printf("Variable %s not declared in if statement line number %d.\n",root->actual_lex,root->line_num);
			if(t2 == -1)
				printf("2nd var Variable %s not declared in if statement line number %d.\n",root->nextSibling->actual_lex,root->nextSibling->line_num);
			semantic_flag = 0; ++number_of_semantic_errors;
			return;
		}
		
		if(t1 != t2){
			if(isID1 == 1 && isID2 == 1)
				printf("Type Mismatch error with %s & %s in if statement in line number %d.\n",root->actual_lex,root->nextSibling->actual_lex,root->line_num);
			else if(isID1 == 1)
				printf("Type Mismatch error with %s & %s in if statement in line number %d.\n",root->actual_lex,root->nextSibling->lexeme,root->line_num);
			else if(isID2 == 1)
				printf("Type Mismatch error with %s & %s in if statement in line number %d.\n",root->lexeme,root->nextSibling->actual_lex,root->line_num);
			else
				printf("Type Mismatch error with %s & %s in if statement in line number %d.\n",root->lexeme,root->nextSibling->lexeme,root->line_num);
			semantic_flag = 0; ++number_of_semantic_errors;
		}
		return;
	}
	
	checkifCond(root->firstChild,currSym);
	checkifCond(root->nextSibling,currSym);
}

void handleMatrixDef(tree_node* root,symbolTableNode* currSym,int *col_size,int *row_size){
	int tempcol;
	tree_node* tempR = root;
	tree_node* tempC = root;
	while(tempR != NULL){
		++*row_size;
		tempcol = 0;
		tempC = tempR;
		while(tempC != NULL){
			++tempcol;
			tempC = tempC->firstChild;
		}
		if(*col_size != 0 && tempcol != *col_size){
			printf("Inconsistent number of elements in columns in matrix definition in line number %d.\n",root->line_num);
			*col_size = -1;
			*row_size = -1;
			semantic_flag = 0; ++number_of_semantic_errors;
			break;
		}
		else
			*col_size = tempcol;
		
		tempR = tempR->nextSibling;
	}
	
	if(*row_size > 10){
		printf("Number of Rows in matrix assignment is greater than 10 in line number %d.\n",root->line_num);
		semantic_flag = 0; ++number_of_semantic_errors;
	}
	if(*col_size > 10){
		printf("Number of Columns in matrix assignment is greater than 10 in line number %d.\n",root->line_num);	
		semantic_flag = 0; ++number_of_semantic_errors;
	}
}

void checkIfStmts(tree_node* root,symbolTableNode* currSym,int *offset){
	if(root == NULL)
		return;
		
	tree_node* temp = root;
	tree_node* tree_temp;
	int t1,t2,inx;    
	while(temp != NULL){
        switch(temp->rule_num){
            case 22:
                addDec(temp->addr,currSym,offset);
                break;
			
			case 26:
				t1 = getTypeFromSymTable(temp->addr->firstChild->actual_lex,currSym);	
				if(t1 == -1){
					printf("Variable %s not declared in line number %d.\n",temp->addr->firstChild->actual_lex,temp->addr->firstChild->line_num);	
					semantic_flag = 0; ++number_of_semantic_errors;
				}		
				if(temp->addr->firstChild->nextSibling->rule_num == 48){								
					if(temp->addr->firstChild->nextSibling->addr->is_terminal == 0){
						int col_size = 0,row_size = 0;
						if(t1 != 4  && t1 != -1){
							printf("Invalid LHS type, expecting MATRIX in line number %d.\n",temp->addr->firstChild->line_num);
							semantic_flag = 0; ++number_of_semantic_errors;
						}
						handleMatrixDef(temp->addr->firstChild->nextSibling->addr->addr,currSym,&col_size,&row_size);
						
						if(t1 == 4 && row_size != -1)
							addSizeOfMat(temp->addr->firstChild->actual_lex,currSym,row_size,col_size,offset);
					}
					else {
						t2 = checkArithmeticExpressiontype1(temp->addr->firstChild->nextSibling->addr,currSym);								
						if(t1 != t2 && (t1 != -1 || t2 != -1)){
							printf("Incompatible Operands to operator %s in line %d.\n",temp->addr->actual_lex,temp->addr->line_num);
							semantic_flag = 0; ++number_of_semantic_errors;
						}
						if(t2 == 4){
							int temp_r = 0, temp_c =0;
							checkForMatrices(temp->addr->firstChild->nextSibling->addr,currSym,&temp_r,&temp_c);
							if(t1 != -1){
								IdHashTable* tempidtable = currSym->idTable;
								int inx = hashVal(temp->addr->firstChild->actual_lex) % size_of_idtable1;
								IdHashTableNode* idtemp = tempidtable[inx].head;
								while(idtemp != NULL){
									if(strcmp(idtemp->id,temp->addr->firstChild->actual_lex) == 0)
										idtemp->offset = *offset;
										*offset += temp_r*temp_c*INT_SIZE;
										break;
								}
							}
						}
					}
				}
				
				else if(temp->addr->firstChild->nextSibling->rule_num == 35){
					t2 = getTypeFromSymTable(temp->addr->firstChild->nextSibling->addr->nextSibling->actual_lex,currSym);		
					if(t2 == -1){
						printf("Variable %s not declared in line number %d.\n",temp->addr->firstChild->nextSibling->addr->nextSibling->actual_lex,temp->addr->firstChild->nextSibling->addr->nextSibling->line_num);	
						semantic_flag = 0; ++number_of_semantic_errors;
					}
					else if(t2 != 3){
						printf("String Variable Expected, got some other type in line number %d.\n",temp->addr->line_num);			
						semantic_flag = 0; ++number_of_semantic_errors;
					}
				}
				
				else if(temp->addr->firstChild->nextSibling->rule_num == 43){
					if(strcmp(temp->addr->firstChild->nextSibling->addr->actual_lex,currSym->funId) == 0){
            			printf("Recursive Functions or Function Overloading is not allowed in line number %d.\n",temp->addr->firstChild->nextSibling->addr->line_num);
            			semantic_flag = 0; ++number_of_semantic_errors;
            		}
            		else 
						checkFuncCall(temp->addr->firstChild->nextSibling->addr,currSym,t1);
				}
				break;
		
			case 41:
				t1 = getTypeFromSymTable(temp->addr->nextSibling->actual_lex,currSym);
				if(t1 == -1){
					printf("Variable %s not declared in line number %d.\n",temp->addr->nextSibling->actual_lex,temp->addr->nextSibling->line_num);
					semantic_flag = 0; ++number_of_semantic_errors;
				}
				else if(t1 != 1 && t1 != 2){
					semantic_flag = 0; ++number_of_semantic_errors;
					printf("Incompatible Arguments to Function read in line number %d.\n",temp->addr->line_num);
				}
				break;

			case 42:
				t1 = getTypeFromSymTable(temp->addr->nextSibling->actual_lex,currSym);
				if(t1 == -1){
					printf("Variable %s not declared in line number %d.\n",temp->addr->nextSibling->actual_lex,temp->addr->nextSibling->line_num);
					semantic_flag = 0; ++number_of_semantic_errors;
				}
				break;
            	
            case 43:
            	if(strcmp(temp->addr->actual_lex,currSym->funId) == 0){
            		printf("Recursive Functions or Function Overloading is not allowed in line number %d.\n",temp->addr->line_num);
            		semantic_flag = 0; ++number_of_semantic_errors;
            	}
            	else
            		checkFuncCall(temp->addr,currSym,-1);
            	break;
           
           	case 36:
            	checkifCond(temp->addr,currSym);
            	checkIfStmts(temp->addr->nextSibling,currSym,offset);
            	break;		
            	
            case 37:
            	checkIfStmts(temp->addr,currSym,offset);
            	break;
            
            case 27:
				tree_temp = temp->addr->firstChild;
				while(tree_temp->is_terminal == 1){
					tree_temp = tree_temp->nextSibling;
				}
				
				checkAssignmentStmtType2(temp->addr->firstChild,tree_temp,currSym);
				break;
            	
            default:
                break;
        }
        temp = temp->nextSibling;
    }
}

void setAssigned(char* id,symbolTableNode* currSym){
	IdHashTable* temp = currSym->idTable;
	int inx = hashVal(id) % size_of_idtable1;
	IdHashTableNode* idtemp = temp[inx].head;
	while(idtemp != NULL){
		if(strcmp(idtemp->id,id) == 0)
			idtemp->is_assigned = 1;
		idtemp = idtemp->next;	
	}
}

void checkForAssignmentOfOutputParams(tree_node* root,char* fid,symbolTableNode* currSym){
	tree_node* temp = root;
	while(root != NULL && root->nextSibling != NULL){
		IdHashTable* temp = currSym->idTable;
		int inx = hashVal(root->nextSibling->actual_lex) % size_of_idtable1;
		IdHashTableNode* idtemp = temp[inx].head;
		while(idtemp != NULL){
			if(strcmp(idtemp->id,root->nextSibling->actual_lex) == 0)
				if(idtemp->is_assigned == 0){
					printf("Output Parameter %s is not assigned any value throughout the function definition %s, in line number %d.\n",root->nextSibling->actual_lex,fid,root->nextSibling->line_num);
					semantic_flag = 0; ++number_of_semantic_errors;
					break;
				}	
			idtemp = idtemp->next;	
		}
		root = root->nextSibling->nextSibling;
	}
}

void checkNewFunction(tree_node* root,symbolTableNode* currSym){
	currSym->output_list = root->firstChild;
	currSym->input_list = root->firstChild->firstChild;
	tree_node* temp = root->firstChild->firstChild->firstChild;
    symbolTableNode* funChildSymTable;
	tree_node* tree_temp;
	int t1,t2,inx,offset = 0;    
	int fun_offset = 0;
	while(temp != NULL){
		//printf("function : %d\n",temp->rule_num);
        switch(temp->rule_num){
            case 22:
                addDec(temp->addr,currSym,&offset);
                break;
			
			case 26:
				t1 = getTypeFromSymTable(temp->addr->firstChild->actual_lex,currSym);	
				if(t1 == -1){
					printf("Variable %s not declared in line number %d.\n",temp->addr->firstChild->actual_lex,temp->addr->firstChild->line_num);	
					semantic_flag = 0; ++number_of_semantic_errors;
				}
				setAssigned(temp->addr->firstChild->actual_lex,currSym);
									
				if(temp->addr->firstChild->nextSibling->rule_num == 48){								
					if(temp->addr->firstChild->nextSibling->addr->is_terminal == 0){
						int col_size = 0,row_size = 0;
						if(t1 != 4  && t1 != -1){
							printf("Invalid LHS type, expecting MATRIX in line number %d.\n",temp->addr->firstChild->line_num);
							semantic_flag = 0; ++number_of_semantic_errors;
						}
						handleMatrixDef(temp->addr->firstChild->nextSibling->addr->addr,currSym,&col_size,&row_size);
						
						if(t1 == 4 && row_size != -1)
							addSizeOfMat(temp->addr->firstChild->actual_lex,currSym,row_size,col_size,&offset);
					}
					else {
						t2 = checkArithmeticExpressiontype1(temp->addr->firstChild->nextSibling->addr,currSym);								
						if(t1 != t2 && (t1 != -1 || t2 != -1)){
							printf("Incompatible Operands to operator %s in line %d.\n",temp->addr->actual_lex,temp->addr->line_num);
							semantic_flag = 0; ++number_of_semantic_errors;
						}
						if(t2 == 4){
							int temp_r = 0, temp_c =0;
							checkForMatrices(temp->addr->firstChild->nextSibling->addr,currSym,&temp_r,&temp_c);
							if(t1 != -1){
								IdHashTable* tempidtable = currSym->idTable;
								int inx = hashVal(temp->addr->firstChild->actual_lex) % size_of_idtable1;
								IdHashTableNode* idtemp = tempidtable[inx].head;
								while(idtemp != NULL){
									if(strcmp(idtemp->id,temp->addr->firstChild->actual_lex) == 0)
										idtemp->offset = offset;
										offset += temp_r*temp_c*INT_SIZE;
										break;
								}
							}
						}
					}
				}
				
				else if(temp->addr->firstChild->nextSibling->rule_num == 35){
					t2 = getTypeFromSymTable(temp->addr->firstChild->nextSibling->addr->nextSibling->actual_lex,currSym);		
					if(t2 == -1){
						printf("Variable %s not declared in line number %d.\n",temp->addr->firstChild->nextSibling->addr->nextSibling->actual_lex,temp->addr->firstChild->nextSibling->addr->nextSibling->line_num);	
						semantic_flag = 0; ++number_of_semantic_errors;
					}
					else if(t2 != 3){
						printf("String Variable Expected, got some other type in line number %d.\n",temp->addr->line_num);			
						semantic_flag = 0; ++number_of_semantic_errors;
					}
				}
				
				else if(temp->addr->firstChild->nextSibling->rule_num == 43){
					if(strcmp(temp->addr->firstChild->nextSibling->addr->actual_lex,currSym->funId) == 0){
            			printf("Recursive Functions or Function Overloading is not allowed in line number %d.\n",temp->addr->firstChild->nextSibling->addr->line_num);
            			semantic_flag = 0; ++number_of_semantic_errors;
            		}
            		else 
						checkFuncCall(temp->addr->firstChild->nextSibling->addr,currSym,t1);
				}
				break;
		
			case 41:
				t1 = getTypeFromSymTable(temp->addr->nextSibling->actual_lex,currSym);
				if(t1 == -1){
					printf("Variable %s not declared in line number %d.\n",temp->addr->nextSibling->actual_lex,temp->addr->nextSibling->line_num);
					semantic_flag = 0; ++number_of_semantic_errors;
				}	 
					 
				else if(t1 != 1 && t1 != 2){
					printf("Incompatible Arguments to Function read in line number %d.\n",temp->addr->line_num);
					semantic_flag = 0; ++number_of_semantic_errors;
				}
				break;

			case 42:
				t1 = getTypeFromSymTable(temp->addr->nextSibling->actual_lex,currSym);
				if(t1 == -1){
					 printf("Variable %s not declared in line number %d.\n",temp->addr->nextSibling->actual_lex,temp->addr->nextSibling->line_num);
					semantic_flag = 0; ++number_of_semantic_errors;
				}
				break;
            
            case 14:
            	fun_offset = 0;
            	funChildSymTable = addFunDec(temp->addr,currSym,&fun_offset);
            	checkNewFunction(temp->addr,funChildSymTable);
            	break;
            	
            case 43:
            	if(strcmp(temp->addr->actual_lex,currSym->funId) == 0){
            		printf("Recursive Functions or Function Overloading is not allowed in line number %d.\n",temp->addr->line_num);
            		semantic_flag = 0; ++number_of_semantic_errors;
            	}
            	else 
            		checkFuncCall(temp->addr,currSym,-1);
            	break;
           
           	case 36:
           		checkifCond(temp->addr,currSym);
            	checkIfStmts(temp->addr->nextSibling,currSym,&offset);
            	break;		
            	
            case 27:
				tree_temp = temp->addr->firstChild;
				while(tree_temp->is_terminal == 1){
					setAssigned(tree_temp->actual_lex,currSym);
					tree_temp = tree_temp->nextSibling;
				}
				
				checkAssignmentStmtType2(temp->addr->firstChild,tree_temp,currSym);
				break;
            	
            default:
                break;
        }
        temp = temp->nextSibling;
    }
    checkForAssignmentOfOutputParams(currSym->output_list,root->actual_lex,currSym);
}

char* getStringFromType(int type){
	char* s1 = "integer";
	char* s2 = "real";
	char* s3 = "string";
	char* s4 = "matrix";
	switch(type){
		case 1: 
			return s1;
			break;
		case 2:
			return s2;
			break;
		case 3:
			return s3;
			break;
		case 4:
			return s4;
			break;
		default:
			return NULL;
			break;
	}
	
	return NULL;
}

void semanticCheck(tree_node* root){
	semantic_flag = 1;
	number_of_semantic_errors = 0;
    tree_node* temp = root->addr;
    char* str = "MAIN";
    symRoot = createSymbolTable(str,1);
    symbolTableNode* currSym = symRoot;
    symbolTableNode* funChildSymTable;
    tree_node* tree_temp;
	int t1,t2,inx;
	main_offset = 0;   
	int fun_offset = 0; 
	while(temp != NULL){
		//printf("main : %d\n",temp->rule_num);
        switch(temp->rule_num){
            case 22:
                addDec(temp->addr,currSym,&main_offset);
                break;
			
			case 26:
				t1 = getTypeFromSymTable(temp->addr->firstChild->actual_lex,currSym);	
				if(t1 == -1){
					printf("Variable %s not declared in line number %d.\n",temp->addr->firstChild->actual_lex,temp->addr->firstChild->line_num);	
					semantic_flag = 0; ++number_of_semantic_errors;			
				}					
				if(temp->addr->firstChild->nextSibling->rule_num == 48){								
					if(temp->addr->firstChild->nextSibling->addr->is_terminal == 0){
						int col_size = 0,row_size = 0;
						if(t1 != 4  && t1 != -1){
							printf("Invalid LHS type, expecting MATRIX in line number %d.\n",temp->addr->firstChild->line_num);
							semantic_flag = 0; ++number_of_semantic_errors;
						}
						
						handleMatrixDef(temp->addr->firstChild->nextSibling->addr->addr,currSym,&col_size,&row_size);
						
						if(t1 == 4 && row_size != -1)
							addSizeOfMat(temp->addr->firstChild->actual_lex,currSym,row_size,col_size,&main_offset);
					}
					else {
						t2 = checkArithmeticExpressiontype1(temp->addr->firstChild->nextSibling->addr,currSym);								
						if(t1 != t2 && (t1 != -1 || t2 != -1)){
							printf("Incompatible Operands to operator %s in line %d.\n",temp->addr->actual_lex,temp->addr->line_num);
							semantic_flag = 0; ++number_of_semantic_errors;	
						}
						if(t2 == 4){
							int temp_r = 0, temp_c =0;
							checkForMatrices(temp->addr->firstChild->nextSibling->addr,currSym,&temp_r,&temp_c);
							if(t1 != -1){
								IdHashTable* tempidtable = currSym->idTable;
								int inx = hashVal(temp->addr->firstChild->actual_lex) % size_of_idtable1;
								IdHashTableNode* idtemp = tempidtable[inx].head;
								while(idtemp != NULL){
									if(strcmp(idtemp->id,temp->addr->firstChild->actual_lex) == 0){
										idtemp->offset = main_offset;
										main_offset += temp_r*temp_c*INT_SIZE;
										break;
									}
									idtemp = idtemp->next;
								}
							}
						}
						else if(t2 == 3){
							int temp_len_str = 0;
							checkForStrings(temp->addr->firstChild->nextSibling->addr,currSym,&temp_len_str);

							if(temp_len_str > 20){
								printf("Assignment leads to size of string %s greater than 20(it equals %d)  in line number %d.\n",temp->addr->firstChild->actual_lex,temp_len_str,temp->addr->firstChild->line_num);
								semantic_flag = 0; ++number_of_semantic_errors;
							}
							else if(t1 != -1){
								IdHashTableNode* idtemp = getIdTableNode(temp->addr->firstChild->actual_lex,currSym);
								if(idtemp != NULL){
									idtemp->offset = main_offset;
									idtemp->string_size = temp_len_str;
									main_offset += temp_len_str*CHAR_SIZE;
									break;
								}
							}
						}
					}	
				}
				
				else if(temp->addr->firstChild->nextSibling->rule_num == 35){
					t2 = getTypeFromSymTable(temp->addr->firstChild->nextSibling->addr->nextSibling->actual_lex,currSym);		
					if(t2 == -1){
						printf("Variable %s not declared in line number %d.\n",temp->addr->firstChild->nextSibling->addr->nextSibling->actual_lex,temp->addr->firstChild->nextSibling->addr->nextSibling->line_num);
						semantic_flag = 0; ++number_of_semantic_errors;
					}	
					else if(t2 != 3){
						semantic_flag = 0; ++number_of_semantic_errors;
						printf("String Variable Expected, got some other type in line number %d.\n",temp->addr->line_num);			
					}
				}
				
				else if(temp->addr->firstChild->nextSibling->rule_num == 43){
					if(strcmp(temp->addr->firstChild->nextSibling->addr->actual_lex,currSym->funId) == 0){
            			printf("Recursive Functions or Function Overloading is not allowed in line number %d.\n",temp->addr->firstChild->nextSibling->addr->line_num);
            			semantic_flag = 0; ++number_of_semantic_errors;
            		}
            		else 
						checkFuncCall(temp->addr->firstChild->nextSibling->addr,currSym,t1);
				}
				break;
		
			case 41:
				t1 = getTypeFromSymTable(temp->addr->nextSibling->actual_lex,currSym);
				if(t1 == -1){
					 printf("Variable %s not declared in line number %d.\n",temp->addr->nextSibling->actual_lex,temp->addr->nextSibling->line_num);
					 semantic_flag = 0; ++number_of_semantic_errors;
				}	 
					 
				else if(t1 != 1 && t1 != 2){
					printf("Incompatible Arguments to Function read in line number %d.\n",temp->addr->line_num);
					semantic_flag = 0; ++number_of_semantic_errors;
				}
				break;

			case 42:
				t1 = getTypeFromSymTable(temp->addr->nextSibling->actual_lex,currSym);
				if(t1 == -1){
					 printf("Variable %s not declared in line number %d.\n",temp->addr->nextSibling->actual_lex,temp->addr->nextSibling->line_num);
					 semantic_flag = 0; ++number_of_semantic_errors;
				}
				break;
            
            case 14:
            	fun_offset = 0;
            	funChildSymTable = addFunDec(temp->addr,currSym,&fun_offset);
            	checkNewFunction(temp->addr,funChildSymTable);
            	break;
            	
            case 43:
            	checkFuncCall(temp->addr,currSym,-1);
            	break;
            	
            case 36:
            	checkifCond(temp->addr,currSym);
            	checkIfStmts(temp->addr->nextSibling,currSym,&main_offset);
            	break;	
            	
            case 27:
				tree_temp = temp->addr->firstChild;
				while(tree_temp->is_terminal == 1){
					tree_temp = tree_temp->nextSibling;
				}
				checkAssignmentStmtType2(temp->addr->firstChild,tree_temp,currSym);
				break;
            	
            default:
                break;
        }
        temp = temp->nextSibling;
    }
    
    if(semantic_flag == 1)
    	printf("Compilation Completed with 0 Semantical Errors.\n");
    else
       	printf("\nCompilation Terminated with %d Semantical Errors.\n",number_of_semantic_errors);
}

int getwidth(IdHashTableNode* idtemp,int type){
	switch(type){
		case 1:
			return 2;
			
		case 2:
			return 4;
			
		case 3:
			return 1;
			
		case 4:
			return idtemp->row_size*idtemp->col_size*2;
	}
	
	return -1;
}

void printIdTable(symbolTableNode* currSym){
	IdHashTable* temp = currSym->idTable;
	int inx = 0;
	IdHashTableNode* idtemp;
	for(inx=0;inx<size_of_idtable1;++inx){
		idtemp = temp[inx].head;
		while(idtemp != NULL){
			printf("ID : %s, scope : %s, type : %s, width : %d, offset : %d\n",idtemp->id,currSym->funId,getStringFromType(idtemp->type),getwidth(idtemp,idtemp->type),idtemp->offset);
			idtemp = idtemp->next;
		}
	}
}

void printSymbolTable(symbolTableNode* root){
    if(root == NULL)
        return;
	
	if(root->parent == NULL)
    	printf("Function : %s ; Static Parent : --- ; nesting_level : %d\n",root->funId,root->nest_level);
    else
    	printf("Function : %s ; Static Parent : %s; nesting_level : %d\n",root->funId,root->parent->funId,root->nest_level);
	
	printf("-------------------------------------------------------------\n");
	
	printIdTable(root);
	
	printf("-------------------------------------------------------------\n");
	
    printSymbolTable(root->firstChild);	
	
    printSymbolTable(root->nextSibling);

}

