/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/

#include "parser.h"
#include <string.h>

tree_node* curr;

char* terminal[] = {"ASSIGNOP","FUNID","ID","NUM","RNUM","STR","END","INT","REAL","STRING","MATRIX","MAIN","SQO","SQC","OP","CL","SEMICOLON","COMMA","IF","ELSE","ENDIF","READ","PRINT","FUNCTION","PLUS","MINUS","MUL","DIV","SIZE","AND","OR","NOT","LT","LE","EQ","GT","GE","NE","COMMENT","$","EPS"};
char* non_terminal[] = {"<startProgram>","<mainFunction>","<stmtsAndFunctionDefs>","<moreStmtsAndFunctionDefs>","<stmtOrFunctionDef>","<stmt>","<functionDef>","<parameter_list>","<type>","<remainingList>","<declarationStmt>","<var_list>","<more_ids>","<assignmentStmt_type1>","<assignmentStmt_type2>","<leftHandSide_singleVar>","<leftHandSide_listVar>","<rightHandSide_type1>","<rightHandSide_type2>","<sizeExpression>","<ifStmt>","<elsePart>","<otherStmts>","<ioStmt>","<funCallStmt>","<inputParameterList>","<listVar>","<arithmeticExpression>","<moreArithmeticExpressions>","<arithmeticTerm>","<moreArithmeticTerms>","<factor>","<operator_lowPrecedence>","<operator_highPrecedence>","<booleanExpression>","<constrainedVars>","<var>","<matOrId>","<matrix>","<rows>","<moreRows>","<row>","<remainingColElements>","<matrixElement>","<logicalOp>","<relationalOp>"};

token_node* getTokenNode(char *token){
	token_node *temp = (token_node*)malloc(sizeof(token_node));
	strcpy(temp->token,token);
	temp->next=NULL;
	return temp;
}

void getGrammar(GRAMMAR gr){
	FILE *filePtr = fopen("grammar.txt","r");
	char str[200],*delim=" ";
	int i = 0;
	while(fgets(str,200,filePtr)!=NULL)
	{
	    int rnum = 1;
		int rule_length = strlen(str);
		str[rule_length - 1] = 0;			//remove ending newline character
		char *token;
		token=strtok(str,delim);
		gr[i].non_terminal_num = getIndexNonTerminal(token);
		gr[i].rule_num = rnum;
		token=strtok(NULL,delim);				//===> found
		token=strtok(NULL,delim);
		gr[i].head = getTokenNode(token);	//first token_node of first rule
		token_node *temp = gr[i].head;
		token=strtok(NULL,delim);
		while(token!=NULL)
		{
            temp->next = getTokenNode(token);
            temp = temp->next;
			token=strtok(NULL,delim);
		}
		gr[i].last = temp;
		++i;
		++rnum;
	}
}

void printGrammar(GRAMMAR gr){
	int i = 0;
	for (i = 0; i < NO_OF_RULES; ++i){
		printf("%s(%d) ===> ", non_terminal[gr[i].non_terminal_num],gr[i].non_terminal_num);
        token_node* next_token = gr[i].head;
        while(next_token != NULL){
            printf("%s ", next_token->token);
            next_token = next_token->next;
        }
        printf("\n");
	}
}

void populateParseTableWithRhs(token_node* head,int inx_nt,set first_of_rhs,int rnum){
    int i=0;
    set temp = first_of_rhs;
    for(i=0;i<NO_OF_TERMINALS;++i){
        if(temp % 2 == 1){
            ParseTable[inx_nt][i].tn = head;
            ParseTable[inx_nt][i].rule_num = rnum;
        }
        temp = temp >> 1;
    }
}

void populateParseTable(GRAMMAR gr){
    int i=0,j=0;

    for(i=0;i<NO_OF_NON_TERMINALS;++i)
        for(j=0;j<NO_OF_TERMINALS;++j){
            ParseTable[i][j].tn = NULL;
            ParseTable[i][j].rule_num = -1;
        }

    for(i=0;i<NO_OF_RULES;++i){
        if(isInSet(gr[i].rhs_FIRST,POS_OF_EPS)){
            set ff_union = unionSet(FollowSet[gr[i].non_terminal_num],deleteEPSFromSet(gr[i].rhs_FIRST));
            populateParseTableWithRhs(gr[i].head,gr[i].non_terminal_num,ff_union,i+1);
        }
        else
            populateParseTableWithRhs(gr[i].head,gr[i].non_terminal_num,gr[i].rhs_FIRST,i+1);
    }
}

void printRule(token_node* head){ //USED IN printParseTable()
    token_node* temp = head;
    while(temp != NULL){
        printf("%s ",temp->token);
        temp = temp->next;
    }
}

void printParseTable(){
    int i=0,j=0;
    for(i=0;i<NO_OF_NON_TERMINALS;++i){
        for(j=0;j<NO_OF_TERMINALS;++j){
            if(ParseTable[i][j].tn != NULL){
                printf("Rule Number =%d, ",ParseTable[i][j].rule_num);
                printf("T=%s ",terminal[j]);
                printf("%s ===> ",non_terminal[i]);
                printRule(ParseTable[i][j].tn);
                printf("\n");
            }
        }
    }
}

void pushRuleOnStack(stack* s,token_node* rule){
    token_node* temp = rule;
    stack* sub_stack = createStack();

    while(temp != NULL){
        push(sub_stack,temp->token);
        temp = temp->next;
    }

    while(!isEmpty(sub_stack)){
        push(s,pop(sub_stack)->tok);
    }
}

void insertRuleInTree(tree_node* parent,token_node* head){
    if(parent == NULL || head == NULL)
        return;
    token_node* temp = head;
    tree_node* t_temp;
    int isT = (getIndexTerminal(temp->token) != -1)? 1:0;
    parent->firstChild = newTreeNode(temp->token,isT);
    t_temp = parent->firstChild;
    t_temp->parent = parent;
    temp = temp->next;
    while(temp != NULL){
        isT = (getIndexTerminal(temp->token) != -1)? 1:0;
        t_temp->nextSibling = newTreeNode(temp->token,isT);
        temp = temp->next;
        t_temp = t_temp->nextSibling;
        t_temp->parent = parent;
    }
}

void initializeLexer(char* fname){
    state = 0,start = 0,lex_beg = 0,forward = 0,ln = 1,str_length = 0,mem_ptr = 0;
	populateKeywordHashTable();
    buf = (char*)malloc(sizeof(char)*buf_size);
    buf[0] = '0'; buf[buf_size/2] = '0';
    //char* fname = "test.txt";
    fp = fopen(fname,"r");
    if(fp == NULL)
        printf("ERROR OPENING FILE!\n");
}

tree_node* findNextCurr(tree_node* curr){
    if(curr == NULL)
        return NULL;

    if(curr->nextSibling != NULL)
        return curr->nextSibling;

    if(curr->parent == NULL)
        return NULL;

    return findNextCurr(curr->parent);
}

void parseInputSourceCode(char* fname){
	number_of_parse_tree_nodes = 0;
	size_of_parse_tree = 0;
    initializeLexer(fname);
    if(fp == NULL)
        return;
    stack* s = createStack();
    push(s,"<startProgram>");
    parseTree = createTree();
    parseTree->root = newTreeNode("<startProgram>",0);
    curr = parseTree->root;
    parseTree->root->rule_num = 1;
    int lookAheadInx;
    int inx;
    isCorrect = 1;
    int no_match_on_stack = 0,end_of_input = 0;
    num_of_errors = 0;
    tokenInfo* temp = getNextToken();
    while(temp != NULL){
        lookAheadInx = getIndexTerminal(temp->token);
        if(top(s)->tok == NULL)
            return;
        inx = getIndexNonTerminal(top(s)->tok);
        if(inx != -1){
            no_match_on_stack = 0; //To realize the no_match_from_stack terminal only once
            if(ParseTable[inx][lookAheadInx].tn != NULL){
                token_node* rule = ParseTable[inx][lookAheadInx].tn;
                curr->rule_num = ParseTable[inx][lookAheadInx].rule_num;
                insertRuleInTree(curr,rule);
                pop(s);
                pushRuleOnStack(s,rule);
                curr = curr->firstChild;
            }
            else{
                isCorrect = 0;
                ++num_of_errors;
                printf("Parse Error, Not allowed for such an assignment or missing Semicolon in line %d.\n",temp->line_number);
                set sync_set = addToSet(FollowSet[inx],17);  //sync _set = FOLLOW U SEMICOLON
                while(temp != NULL && (isInSet(sync_set,lookAheadInx+1) == 0)){
                    temp = getNextToken();
                    if(temp != NULL)
                        lookAheadInx = getIndexTerminal(temp->token);
                }
                pop(s);
                curr = findNextCurr(curr);
            }
        }

        else if(getIndexTerminal(top(s)->tok) == 39){
            end_of_input = 1;
            if(end_of_input == 1){
                isCorrect = 0;
                ++num_of_errors;
                printf("Parse Error, Expected end of input but it has more tokens.\n");
            }
            temp = getNextToken();
        }

        else{
            inx = getIndexTerminal(top(s)->tok);
            if(inx == POS_OF_EPS-1){
                pop(s);
                curr = findNextCurr(curr);
            }
            else if(inx == lookAheadInx){
                no_match_on_stack = 0;
                curr->line_num = temp->line_number;
                curr->actual_lex = temp->lexeme;
                if(inx == 3 || inx == 4)
                    curr->is_num = 1;
                pop(s);
                curr = findNextCurr(curr);
                temp = getNextToken();
            }
            else{
                isCorrect = 0;
                if(inx == 16){
                    tokenInfo* t1 = temp;
                    temp = getNextToken();
                    while(temp != NULL && getIndexTerminal(temp->token) != 16){
                        t1 = temp;
                        temp = getNextToken();
                    }
                    ++num_of_errors;
                    if(temp != NULL)
                        printf("Parse Error, Expected Token Not Found Reached end of statement line number %d.\n",temp->line_number);
                    else
                        printf("Parse Error, Expected Token Not Found Reached end of statement line number %d.\n",t1->line_number);
                }
                else if(no_match_on_stack == 0){
                    no_match_on_stack = 1;
                    ++num_of_errors;
                    printf("Parse Error, Expected %s but instead got %s at line number %d.\n",top(s)->tok,temp->token,temp->line_number);
                    pop(s);
                    curr = findNextCurr(curr);
                }
                else{
                    pop(s);
                    curr = findNextCurr(curr);
                }
            }
        }
    }

    if(getIndexTerminal(top(s)->tok) == 39 && isCorrect == 1)
        printf("\nCompilation Successful with 0 Lexical or Parse Errors.\n");

    else if(getIndexTerminal(top(s)->tok) == 39)
        printf("\nCompilation Terminated with %d Errors.\n",num_of_errors);

    else{
        ++num_of_errors;
        printf("Parse Error, Incomplete Input, expecting more Tokens.\n");
        printf("\nCompilation Terminated with %d Errors.\n",num_of_errors++);
    }
    fclose(fp);
}

void populateTerminalHashTable(){
    int i = 0;
    term_hash = createHashTable(NO_OF_TERMINALS);
    for(i = 0; i < NO_OF_TERMINALS; ++i){
        char* source = terminal[i];
        int hash_index = hash(source) % NO_OF_TERMINALS;
        insertInHashTable(term_hash,hash_index,i,source);
    }
}

void populateNonTerminalHashTable(){
    int i = 0;
    non_term_hash = createHashTable(NO_OF_NON_TERMINALS);
    for(i = 0; i < NO_OF_NON_TERMINALS; ++i){
        char* source = non_terminal[i];
        int hash_index = hash(source) % NO_OF_NON_TERMINALS;
        insertInHashTable(non_term_hash,hash_index,i,source);
    }
}

void printFirstSetRHS(GRAMMAR gr){
    int i = 0,j = 0;
    for(i=0;i<NO_OF_RULES;++i){
        //printf("%" PRId64 "\n", FirstSet[i]);
        printf("FIRST_OF(%s) ",non_terminal[gr[i].non_terminal_num]);
        for(j=0;j<NO_OF_TERMINALS;j++)
            if(isInSet(gr[i].rhs_FIRST,j+1))
                printf("%s ",terminal[j]);
        printf("\n");
    }
}

void printFirstSet(GRAMMAR gr){
    int i = 0,j = 0;
    for(i=0;i<NO_OF_NON_TERMINALS;++i){
        //printf("%" PRId64 "\n", FirstSet[i]);
        printf("FIRST_OF(%s) ",non_terminal[i]);
        for(j=0;j<NO_OF_TERMINALS;j++)
            if(isInSet(FirstSet[i],j+1))
                printf("%s ",terminal[j]);
        printf("\n");
    }
}

void printFollowSet(GRAMMAR gr){
    int i = 0,j = 0;
    for(i=0;i<NO_OF_NON_TERMINALS;++i){
        //printf("%" PRId64 "\n", FirstSet[i]);
        printf("FOLLOW_OF(%s) ",non_terminal[i]);
        for(j=0;j<NO_OF_TERMINALS;j++)
            if(isInSet(FollowSet[i],j+1))
                printf("%s ",terminal[j]);
        printf("\n");
    }
}

void printCommentFreeCode(char* fname){
    initializeLexer(fname);
    if(fp == NULL)
        return;
    removeComments();
}

void printTokenList(char* fname){
    initializeLexer(fname);
    if(fp == NULL)
        return;
    tokenInfo* temp = getNextLexedToken();
    while(temp != NULL){
        printf("line-number : %d , lexeme : %s , Token_name : %s\n",temp->line_number,temp->lexeme,temp->token);
        temp = getNextLexedToken();
    }
}



