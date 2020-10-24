/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/

#include "AST.h"

void createAST(tree_node* root){
    if(root == NULL)
        return;

    createAST(root->firstChild);
    tree_node* temp_node;
    if(root->is_terminal == 0){
        switch(root->rule_num){
            case 5:
            case 21:
            case 25:
            case 40:
            case 45:
            case 47:
            case 50:
            case 53:
            case 72:
            case 76:
            case 79:
                root->addr = NULL;
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 4:
            case 6:
            case 16:
            case 17:
            case 18:
            case 19:
            case 28:
            case 55:
            case 56:
            case 57:
            case 58:
            case 59:
            case 63:
            case 64:
            case 65:
            case 66:
            case 67:
            case 68:
            case 69:
            case 71:
            case 81:
            case 82:
            case 83:
            case 84:
            case 85:
            case 86:
            case 87:
            case 88:
                if(root->firstChild->is_terminal == 0){
                    root->addr = root->firstChild->addr;
                    if(root->addr != NULL){
                        free(root->firstChild);
                        root->firstChild = NULL;
                    }
                }
                else{
                    root->addr = root->firstChild;
                }
                break;
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
			case 30:
			case 31:
			case 32:
			case 33:
			case 34:
            case 70:
                root->addr = root->firstChild;
                break;
            case 13:
                root->addr = root->firstChild;
                free(root->firstChild->nextSibling);
                root->firstChild->nextSibling = NULL;
                break;
            case 22:
                root->addr = root->firstChild->addr;
                root->addr->nextSibling = root->firstChild->nextSibling->addr;
                free(root->firstChild->nextSibling->nextSibling);
                root->firstChild->nextSibling->nextSibling = NULL;
                free(root->firstChild->nextSibling);
                root->firstChild->nextSibling = NULL;
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 23:
                temp_node = root->firstChild->nextSibling;
                root->addr = root->firstChild;
                root->addr->nextSibling = root->firstChild->nextSibling->addr;
                free(temp_node);
                break;
            case 24:
                root->addr = root->firstChild->nextSibling->addr;
                free(root->firstChild->nextSibling);
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 1:
                root->addr = root->firstChild->addr;
                free(root->firstChild->nextSibling);
                root->firstChild->nextSibling = NULL;
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 2:
                root->addr = root->firstChild->nextSibling->nextSibling->nextSibling->addr;
                free(root->firstChild->nextSibling->nextSibling->nextSibling->nextSibling);
                root->firstChild->nextSibling->nextSibling->nextSibling->nextSibling = NULL;
                free(root->firstChild->nextSibling->nextSibling->nextSibling);
                root->firstChild->nextSibling->nextSibling->nextSibling = NULL;
                free(root->firstChild->nextSibling->nextSibling);
                root->firstChild->nextSibling->nextSibling = NULL;
                free(root->firstChild->nextSibling);
                root->firstChild->nextSibling = NULL;
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 3:
                root->addr = root->firstChild->addr;
                root->addr->nextSibling = root->firstChild->nextSibling->addr;
                free(root->firstChild->nextSibling);
                root->firstChild->nextSibling = NULL;
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 51:
                if(root->firstChild->nextSibling->addr != NULL){
                    root->addr = root->firstChild->nextSibling->firstChild->addr;
                    root->addr->firstChild = root->firstChild->addr;
                    if(root->addr->firstChild->nextSibling != NULL)
                        root->addr->firstChild->nextSibling->nextSibling = root->firstChild->nextSibling->firstChild->nextSibling->addr;
                    else
                        root->addr->firstChild->nextSibling = root->firstChild->nextSibling->firstChild->nextSibling->addr;
                    free(root->firstChild->nextSibling->firstChild->nextSibling);
                    root->firstChild->nextSibling->firstChild->nextSibling = NULL;
                    free(root->firstChild->nextSibling->firstChild);
                    root->firstChild->nextSibling->firstChild = NULL;
                    free(root->firstChild->nextSibling);
                    root->firstChild->nextSibling = NULL;
                    free(root->firstChild);
                    root->firstChild = NULL;
                }
                else{
                    root->addr = root->firstChild->addr;
                    free(root->firstChild->nextSibling);
                    free(root->firstChild);
                    root->firstChild = NULL;
                }
                break;
            case 52:
                root->addr = root->firstChild->addr;
                break;
            case 49:
                root->addr = root->firstChild->addr;
                break;
            case 48:
                if(root->firstChild->nextSibling->addr != NULL){
                    root->addr = root->firstChild->nextSibling->firstChild->addr;
                    root->addr->firstChild = root->firstChild->addr;
                    if(root->addr->firstChild->nextSibling != NULL)
                        root->addr->firstChild->nextSibling->nextSibling = root->firstChild->nextSibling->firstChild->nextSibling->addr;
                    else
                        root->addr->firstChild->nextSibling = root->firstChild->nextSibling->firstChild->nextSibling->addr;
                    free(root->firstChild->nextSibling->firstChild->nextSibling);
                    root->firstChild->nextSibling->firstChild->nextSibling = NULL;
                    free(root->firstChild->nextSibling->firstChild);
                    root->firstChild->nextSibling->firstChild = NULL;
                    free(root->firstChild->nextSibling);
                    root->firstChild->nextSibling = NULL;
                    free(root->firstChild);
                    root->firstChild = NULL;
                }
                else{
                    root->addr = root->firstChild->addr;
                    free(root->firstChild->nextSibling);
                    free(root->firstChild);
                    root->firstChild = NULL;
                }
                break;
            case 26:
                root->addr = root->firstChild->nextSibling;
                root->addr->firstChild = root->firstChild->addr;
                root->addr->firstChild->nextSibling = root->addr->nextSibling->addr;
                free(root->firstChild->nextSibling->nextSibling->nextSibling);
                root->firstChild->nextSibling->nextSibling->nextSibling = NULL;
                free(root->firstChild->nextSibling->nextSibling);
                root->firstChild->nextSibling->nextSibling = NULL;
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 80:
                temp_node = root->firstChild->nextSibling->nextSibling;
                root->addr = root->firstChild->nextSibling;
                root->addr->nextSibling = root->addr->nextSibling->nextSibling;
                free(temp_node->nextSibling->nextSibling);
                free(temp_node);
                free(root->firstChild);
                root->addr->nextSibling->nextSibling = NULL;
                break;
            case 37:
                root->addr = root->firstChild->nextSibling->addr;
                root->addr->nextSibling = root->firstChild->nextSibling->nextSibling->addr;
                free(root->firstChild->nextSibling->nextSibling->nextSibling->nextSibling);
                free(root->firstChild->nextSibling->nextSibling->nextSibling);
                root->firstChild->nextSibling->nextSibling->nextSibling = NULL;
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 62:
                root->addr = root->firstChild->nextSibling->addr;
                root->addr->firstChild = root->firstChild->addr;
                root->addr->firstChild->nextSibling = root->firstChild->nextSibling->nextSibling->addr;
                free(root->firstChild->nextSibling->nextSibling);
                free(root->firstChild->nextSibling);
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 60:
                root->addr = root->firstChild->nextSibling->nextSibling->nextSibling->addr;
                root->addr->firstChild = root->firstChild->nextSibling->addr;
                root->addr->firstChild->nextSibling = root->firstChild->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling->addr;
                free(root->firstChild->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling);
                free(root->firstChild->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling);
                free(root->firstChild->nextSibling->nextSibling->nextSibling->nextSibling);
                free(root->firstChild->nextSibling->nextSibling->nextSibling);
                free(root->firstChild->nextSibling->nextSibling);
                free(root->firstChild->nextSibling);
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 61:
                root->addr = root->firstChild;
                root->addr->firstChild = root->firstChild->nextSibling->nextSibling->addr;
                free(root->firstChild->nextSibling->nextSibling->nextSibling);
                free(root->firstChild->nextSibling->nextSibling);
                free(root->firstChild->nextSibling);
                root->firstChild->nextSibling = NULL;
                break;
            case 36:
                root->addr = root->firstChild->nextSibling->nextSibling->addr;
                root->addr->nextSibling = root->firstChild->nextSibling->nextSibling->nextSibling->nextSibling->addr;
                root->addr->nextSibling->nextSibling = root->firstChild->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling->addr;
                temp_node = root->addr->nextSibling;
                while(temp_node->nextSibling != NULL)
                    temp_node = temp_node->nextSibling;
                temp_node->nextSibling = root->firstChild->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling;
                free(root->firstChild->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling);
                free(root->firstChild->nextSibling->nextSibling->nextSibling->nextSibling);
                free(root->firstChild->nextSibling->nextSibling->nextSibling);
                free(root->firstChild->nextSibling->nextSibling);
                free(root->firstChild->nextSibling);
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 39:
                root->addr = root->firstChild->addr;
                root->addr->nextSibling = root->firstChild->nextSibling->addr;
                free(root->firstChild->nextSibling);
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 38:
                root->addr = NULL;
                free(root->firstChild->nextSibling);
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 41:
            case 42:
                free(root->firstChild->nextSibling->nextSibling->nextSibling->nextSibling);
                free(root->firstChild->nextSibling->nextSibling->nextSibling);
                root->addr = root->firstChild;
                temp_node = root->firstChild->nextSibling;
                root->addr->nextSibling = root->firstChild->nextSibling->nextSibling;
                root->addr->nextSibling->nextSibling = NULL;
                free(temp_node);
                break;
            case 54:
                root->addr = root->firstChild->nextSibling->addr;
                free(root->firstChild->nextSibling->nextSibling);
                free(root->firstChild);
                root->addr->nextSibling = NULL;
                break;
            case 73:
                root->addr = root->firstChild->nextSibling->addr;
                free(root->firstChild->nextSibling->nextSibling);
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 74:
                root->addr = root->firstChild->addr;
                root->addr->nextSibling = root->firstChild->nextSibling->addr;
                free(root->firstChild->nextSibling);
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 75:
                root->addr = root->firstChild->nextSibling->addr;
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 77:
                root->addr = root->firstChild;
                root->addr->firstChild = root->firstChild->nextSibling->addr;
                free(root->addr->nextSibling);
                root->addr->nextSibling = NULL;
                break;
            case 78:
                temp_node = root->firstChild->nextSibling->nextSibling;
                root->addr = root->firstChild->nextSibling;
                root->addr->firstChild = root->firstChild->nextSibling->nextSibling->addr;
                free(temp_node);
                free(root->firstChild);
                root->firstChild = NULL;
                root->addr->nextSibling = NULL;
                break;
            case 35:
                root->addr = root->firstChild;
                root->addr->nextSibling = root->firstChild->nextSibling;
                break;
            case 43:
                root->addr = root->firstChild;
                root->addr->firstChild = root->firstChild->nextSibling->nextSibling->addr;
                free(root->firstChild->nextSibling->nextSibling->nextSibling);
                free(root->firstChild->nextSibling->nextSibling);
                free(root->firstChild->nextSibling);
                root->firstChild->nextSibling = NULL;
                break;
            case 44:
                root->addr = root->firstChild->addr;
                root->addr->nextSibling = root->firstChild->nextSibling->addr;
                free(root->firstChild->nextSibling);
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 46:
                root->addr = root->firstChild->nextSibling->addr;
                free(root->firstChild->nextSibling);
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 27:
                root->addr = root->firstChild->nextSibling;
                root->addr->firstChild = root->firstChild->addr;
                temp_node = root->addr->firstChild;
                while(temp_node->nextSibling != NULL)
                    temp_node = temp_node->nextSibling;
                temp_node->nextSibling = root->addr->nextSibling->addr;
                free(root->firstChild->nextSibling->nextSibling->nextSibling);
                free(root->firstChild->nextSibling->nextSibling);
                root->firstChild->nextSibling->nextSibling = NULL;
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 29:
                root->addr = root->firstChild->nextSibling->addr;
                free(root->firstChild->nextSibling->nextSibling);
                free(root->firstChild->nextSibling);
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 20:
                root->addr = root->firstChild->nextSibling->addr;
                free(root->firstChild->nextSibling);
                free(root->firstChild);
                root->firstChild = NULL;
                break;
            case 15:
                root->addr = root->firstChild->addr;
                root->addr->nextSibling = root->firstChild->nextSibling;
                root->addr->nextSibling->nextSibling = root->firstChild->nextSibling->nextSibling->addr;
                break;
            case 14:
                root->addr = root->firstChild->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling;
                root->addr->firstChild = root->firstChild->nextSibling->nextSibling->addr;
                root->addr->firstChild->firstChild = root->addr->nextSibling->nextSibling->addr;
                root->addr->firstChild->firstChild->firstChild = root->addr->nextSibling->nextSibling->nextSibling->nextSibling->addr;
                free(root->firstChild->nextSibling->nextSibling->nextSibling->nextSibling);
                free(root->firstChild->nextSibling->nextSibling->nextSibling);
                free(root->firstChild->nextSibling->nextSibling);
                free(root->firstChild->nextSibling);
                free(root->firstChild);
                root->firstChild = NULL;
                free(root->addr->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling);
                free(root->addr->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling);
                free(root->addr->nextSibling->nextSibling->nextSibling->nextSibling);
                free(root->addr->nextSibling->nextSibling->nextSibling);
                free(root->addr->nextSibling->nextSibling);
                free(root->addr->nextSibling);
                root->addr->nextSibling = NULL;
                break;
            default:
                break;
        }
    }

    createAST(root->nextSibling);
}









