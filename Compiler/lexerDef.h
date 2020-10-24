/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hashTable.h"

#define buf_size 100  //Size of buffer can be changed from here.
#define NO_OF_KEYWORDS 11

typedef union{
    int num_val;
    double rnum_val;
}value;

typedef struct tokenInfo{
    char lexeme[50], token[20];
    int line_number;
    value val;
}tokenInfo;

char* buf;  //This char array is divided into half and then selectively filling up each part as and when the other part is used up.
FILE* fp;
int num_of_errors;
int state,start,lex_beg,forward,ln,str_length,mem_ptr;

