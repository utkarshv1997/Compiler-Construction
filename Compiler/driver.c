/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/

//#include "parser.h"
//#include "AST.h"
#include "codegen.h"

void checkForSyntax(char* fname){
    populateTerminalHashTable();
    populateNonTerminalHashTable();

    GRAMMAR g = (node*)malloc(sizeof(node)*NO_OF_RULES);
    getGrammar(g);
    getFirstSet(g);
    getFollowSet(g);
    populateParseTable(g);
    parseInputSourceCode(fname);
    //printGrammar(g);
}

int main(int argc, char* argv[])
{
    printf("LEVEL 4 : Symbol Table/AST/Semantic Rules Modules work. Most Errors whether Semantic or Syntactic were handled and reported properly with appropriate error messages in particular line numbers.\n");
    printf("Error lines are modeled in such a way that they handle errors particularly, nothing is general.\n");
    printf("\nPlease Note:- The Code Generation module generates an intermediate code(printed on the console) for the complete input code. It does NOT however generate the final code to be run on NASM.\n");
    printf("The Symbol Table module is built inside the Semantic Checker module itself.\n");

	printf("\nPress option for the defined task(as asked on NALANDA)\n");
    
    int option;
    char* fname = argv[1];
    FILE* ofile;
    char* out_file = argv[2];
	float ans = 0;
	int syntax_flag=0,semantics_flag=0,ast_flag=0;
	while(1){
		printf("\n\nEnter your Option: ");
		scanf(" %d", &option);
		switch(option)
		{
			case 0:
				return 0;
				break;
				
		    case 1:
		    	printf("Following is the Token List of lexemes.\n");
		        printTokenList(fname);
		        printf("\n--------------------------------------------------------------------------\n");
		        break;
		    
		    case 2:
		    	if(ast_flag == 1){
		        	printf("Syntax Analysis has been done already, the parseTree has been converted to the AST.\n");
		        	break;
		        }
		    	if(syntax_flag == 0){
		        	checkForSyntax(fname);
		        	syntax_flag = 1;
		        }
		        printf("\nPARSE TREE STARTS HERE(-- == Not Applicable)");
		        printf("----------------------------------------------------------------------------\n");
		        printParseTreeInOrder(parseTree->root);
		        printf("\n----------------------------------------------------------------------------\n");
		        break;
		    
		    case 3:
		    	if(syntax_flag == 0){
		        	checkForSyntax(fname);
		        	syntax_flag = 1;
		        }
		    	if(isCorrect == 0){
		    		printf("The given program is not syntactically correct, phase 2 can't be implemented.\n");
		    		break;
		    	}
		    	
		    	printf("FOLLOWING IS THE PRE-ORDER TRAVERSAL OF THE ABSTRACT SYNTAX TREE(Root is printed first, then all the children in left to right order).\n");
		    	printf("------------------------------------------------------------------------------\n");
		        number_of_AST_nodes = 0;
		        size_of_AST = 0;
		        if(ast_flag == 0){
		        	createAST(parseTree->root);
		        	ast_flag = 1;
		        }
		        printAST(parseTree->root);
		        printf("\n------------------------------------------------------------------------------\n");
		        break;
		        
		   	case 4:
		   		if(syntax_flag == 0){
		        	checkForSyntax(fname);
		        	syntax_flag = 1;
		        }
		   		if(ast_flag == 0){
		   			createAST(parseTree->root);	
		   			ast_flag = 1;
		   		}	
		   		printf("\n");
		   		printf("Memory Calculations are as follows:-\n");
		   		printf("----------------------------------------------------------------------------\n");
		   		calAST(parseTree->root);
		   		ans = ((size_of_parse_tree - size_of_AST)/size_of_parse_tree)*100;
		   		printf("Parse tree Number of nodes = %f 	Allocated Memory = %f\n",number_of_parse_tree_nodes,size_of_parse_tree);
		   		printf("AST Number of nodes = %f 			Allocated Memory = %f\n",number_of_AST_nodes,size_of_AST);
		   		printf("Compression percentage = %f %% \n",ans); 	
		   		printf("\n--------------------------------------------------------------------------\n");	
		   		break;
		    
		    case 5:
		    	if(syntax_flag == 0){
		        	checkForSyntax(fname);
		        	syntax_flag = 1;
		        }
		    	if(ast_flag == 0){
		   			createAST(parseTree->root);	
		   			ast_flag = 1;
		   		}
		   		if(semantics_flag == 0){
		    		semanticCheck(parseTree->root);
		    		printf("\n");
		    		semantics_flag = 1;
		    	}
		    	printf("Following is the traversal of tree of Symbol Tables.\n");
				printf("--------------------------------------------------------------------------\n");
		    	printSymbolTable(symRoot);
		    	printf("\n--------------------------------------------------------------------------\n");
		    	break;
		       
		    case 6:
		    	if(syntax_flag == 0){
		        	checkForSyntax(fname);
		        	syntax_flag = 1;
		        }
		    	if(ast_flag == 0){
		   			createAST(parseTree->root);	
		   			ast_flag = 1;
		   		}
		    	printf("\n");
		    	if(isCorrect == 0){
		    		printf("The given program is not syntactically correct, phase 2 can't be implemented.\n");
		    		break;
		    	}
		    	semanticCheck(parseTree->root);
		    	semantics_flag = 1;
		    	break;   
		   
		   case 7:
		   		if(syntax_flag == 0){
		        	checkForSyntax(fname);
		        	syntax_flag = 1;
		        }	
		   		if(isCorrect == 1)
		   			if(ast_flag == 0)
		   				createAST(parseTree->root);
		   			if(semantics_flag == 0){
		   				printf("\n");
		       			semanticCheck(parseTree->root);
		        	}
		        	printf("\n");
		        	printf("Folowing is the intermediate Code for the Program.\n");
		        	if(semantic_flag == 1){
		        		generateIntCode(parseTree->root);
		        		printf("\n--------------------------------------------------------------------------\n");
		        	}
		        	else
		        		printf("Error in sematics, Cannot generate Intermediate Code.\n");
		   		break;
		   
			default:
				printf("Invalid choice.\n");
		}
	}
	
    return 0;
}
