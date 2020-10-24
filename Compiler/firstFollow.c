/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/

#include "firstFollow.h"

int getIndexNonTerminal(char* token){
    int inx = hash(token) % NO_OF_NON_TERMINALS;
    hashTableNode* temp = non_term_hash[inx].head;
    while(temp != NULL){
        if(strcmp(temp->str,token) == 0)
            return temp->index;
        temp = temp->next;
    }
    return -1;
}

int getIndexTerminal(char* token){
    int inx = hash(token) % NO_OF_TERMINALS;
    hashTableNode* temp = term_hash[inx].head;
    while(temp != NULL){
        if(strcmp(temp->str,token) == 0)
            return temp->index;
        temp = temp->next;
    }
    return -1;
}

void getFirstSet(GRAMMAR gr){
    int i=0,ChangeinFirst=0;
    while(i<NO_OF_RULES || ChangeinFirst == 1){
        if(i == NO_OF_RULES){
            i = 0;
            ChangeinFirst = 0;
        }
        int inx_nt = gr[i].non_terminal_num;
        gr[i].rhs_FIRST = 0;
        token_node *temp = gr[i].head;
        set nt_temp = FirstSet[inx_nt];
        while(temp != NULL){
            int rhsInx = getIndexTerminal(temp->token);
            if(rhsInx != -1){
                FirstSet[inx_nt] = addToSet(FirstSet[inx_nt],rhsInx+1);
                deleteEPSFromSet(FirstSet[inx_nt]);
                if(!isSame(nt_temp,FirstSet[inx_nt]))
                    ChangeinFirst = 1;
                gr[i].rhs_FIRST = addToSet(gr[i].rhs_FIRST,rhsInx+1);
                break;
            }
            else{
                rhsInx = getIndexNonTerminal(temp->token);
                if(isInSet(FirstSet[rhsInx],POS_OF_EPS)){
                    FirstSet[inx_nt] = unionSet(FirstSet[inx_nt],FirstSet[rhsInx]);
                    if(!isSame(nt_temp,FirstSet[inx_nt]))
                        ChangeinFirst = 1;
                    gr[i].rhs_FIRST = unionSet(gr[i].rhs_FIRST,FirstSet[rhsInx]);
                }
                else{
                    FirstSet[inx_nt] = unionSet(FirstSet[inx_nt],FirstSet[rhsInx]);
                    deleteEPSFromSet(FirstSet[inx_nt]);
                    if(!isSame(nt_temp,FirstSet[inx_nt]))
                        ChangeinFirst = 1;
                    gr[i].rhs_FIRST = unionSet(gr[i].rhs_FIRST,FirstSet[rhsInx]);
                    break;
                }
            }
            temp = temp->next;
        }
        ++i;
    }
}


set getFollowForRule(token_node* head,token_node* last){
    if(head == last){
        int inx = getIndexNonTerminal(head->token);

        if(inx == -1){
            if(inx != POS_OF_EPS-1)
                return pow(2,getIndexTerminal(head->token));
            else
                return 0;
        }

        return deleteEPSFromSet(FirstSet[inx]);
    }

    set temp = getFollowForRule(head->next,last);
    int inx = getIndexNonTerminal(head->token);

    if(inx == -1)
        return pow(2,getIndexTerminal(head->token));

    FollowSet[inx] = unionSet(FollowSet[inx],temp);

    if(isInSet(FirstSet[inx],POS_OF_EPS))
        return deleteEPSFromSet(unionSet(FirstSet[inx],temp));

    return deleteEPSFromSet(FirstSet[inx]);
}


set getFollowForLast(token_node* head,token_node* last,int nt_num){
    if(head == last){
        int inx = getIndexNonTerminal(head->token);

        FollowSet[inx] = unionSet(FollowSet[inx],FollowSet[nt_num]);

        if(isInSet(FirstSet[inx],POS_OF_EPS))
            return FollowSet[nt_num];

        return 0;
    }

    set temp = getFollowForLast(head->next,last,nt_num);
    int inx = getIndexNonTerminal(head->token);

    if(inx == -1 || temp == 0)
        return 0;

    FollowSet[inx] = unionSet(FollowSet[inx],temp);
    if(isInSet(FirstSet[inx],POS_OF_EPS))
            return FollowSet[nt_num];

    return 0;
}

void getFollowSet(GRAMMAR gr){
    int i=0;
    for(i=0;i<NO_OF_RULES;++i){
        getFollowForRule(gr[i].head,gr[i].last);
    }
    for(i=0;i<NO_OF_RULES;++i){
        if(getIndexNonTerminal(gr[i].last->token) != -1)
            getFollowForLast(gr[i].head,gr[i].last,gr[i].non_terminal_num);
    }
}


