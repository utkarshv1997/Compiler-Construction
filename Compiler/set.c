/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/

#include "set.h"

set unionSet(set a,set b){
    return a | b;
}

set addToSet(set a,int pos){
    set temp = pow(2,pos-1);
    return a | temp;
}

int isInSet(set a,int pos){
    set temp = a >> (pos-1);
    return ((temp % 2) == 1)? 1:0;
}

set deleteEPSFromSet(set a){
    if(!isInSet(a,POS_OF_EPS))
        return a;

    set temp = pow(2,POS_OF_EPS-1) - 1;
    return a & temp;
}

int isSame(set a,set b){
    if((a ^ b) == 0)
        return 1;

    return 0;
}


