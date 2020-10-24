/*Name- UTKARSH VASHISHTHA
  ID- 2015A7PS0130P*/

#include "lexer.h"

char* keywords[] = {"int","end","real","string","matrix","if","else","endif","read","print","function"};
char* keywords_tokens[] = {"INT","END","REAL","STRING","MATRIX","IF","ELSE","ENDIF","READ","PRINT","FUNCTION"};

HashTable* keyword_hash;

enum token{ASSIGNOP=1,FUNID,ID,NUM,RNUM,STR,END,INT,REAL,STRING,MATRIX,MAIN,SQO,SQC,OP,CL,SEMICOLON,COMMA,IF,ELSE,ENDIF,READ,PRINT,FUNCTION,PLUS,MINUS,MUL,DIV,SIZE,AND,OR,NOT,LT,LE,EQ,GT,GE,NE,COMMENT};

int fail(){
    forward = lex_beg;
    switch(start){
        case 0 : start = 12; break;
        case 12 : start = 15; break;
        case 15 : start = 18; break;
        case 18 : start = 20; break;
        case 20 : start = 22; break;
        case 22 : start = 25; break;
        case 25 : start = 29; break;
        case 29 : start = 39; break;
        case 39 : start = 46; break;
        default : printf("There is a serious problem with the lexer, please report at *REDACTED*!\n");
    }
    return start;
}

char nextChar(){  //Implemets a Twin Buffer by considering a single array as of size half of the total buffer size.
    if(forward == mem_ptr){
        int c = fread(buf+mem_ptr,1,buf_size/2,fp);
        if(c < buf_size/2)
            buf[c+mem_ptr] = '\0';
        if(mem_ptr == 0)
            mem_ptr = buf_size/2;
        else
            mem_ptr = 0;
    }
    char temp = buf[forward];

    if(forward == buf_size-1)
        forward = 0;
    else
        ++forward;

    return temp;
}

tokenInfo* newToken(char str[],char lex[]){
    tokenInfo* temp = (tokenInfo*)malloc(sizeof(tokenInfo));
    temp->line_number = ln;
    strcpy(temp->lexeme,lex);
    strcpy(temp->token,str);
    return temp;
}

void retract(int times){
    if(times == 1){
        if(forward == 0)
            forward = buf_size-1;
        else
            --forward;
    }
    else{
        if(forward == 0)
            forward = buf_size - 2;
        else if(forward == 1)
            forward = buf_size - 1;
        else
            forward = forward - 2;
    }
}

tokenInfo* getNumToken(int type){
    char *s = (char*)malloc(sizeof(char)*50);
    int i = 0;

    while(lex_beg != forward){
        s[i] = buf[lex_beg];

        if(lex_beg == buf_size-1)
            lex_beg = 0;
        else
            ++lex_beg;

        ++i;
    }
    s[i] = '\0';
    tokenInfo* temp;

    if(type == 0){
        int temp_val = atoi(s);
        temp = newToken("NUM",s);
        temp->val.num_val = temp_val;
    }

    else{
        char *ptr;
        temp = newToken("RNUM",s);
        temp->val.rnum_val = strtod(s, &ptr);
    }

    return temp;
}

int getIndexKeyword(char* token){
    int inx = hash(token) % NO_OF_KEYWORDS;
    hashTableNode* temp = keyword_hash[inx].head;
    while(temp != NULL){
        if(strcmp(temp->str,token) == 0)
            return temp->index;
        temp = temp->next;
    }
    return -1;
}

tokenInfo* getLookup(){
    char *s = (char*)malloc(sizeof(char)*50);
    int i = 0;

    while(lex_beg != forward){
        s[i] = buf[lex_beg];

        if(lex_beg == buf_size-1)
            lex_beg = 0;
        else
            ++lex_beg;

        ++i;
    }
    s[i] = '\0';

    if(i >= 20){
    	printf("Lexical Error, Invalid Identifier, length > 20 in line number %d.\n",ln);
    	return newToken("ERROR","e");
    }

    int kinx = getIndexKeyword(s);

    if(kinx != -1)
    	return newToken(keywords_tokens[kinx],s);

    tokenInfo* temp = newToken("ID",s);

    return temp;
}

tokenInfo* checkForString(){
    char *s = (char*)malloc(sizeof(char)*50);
    int i = 0;

    if(lex_beg == buf_size-1)
        lex_beg = 0;
    else
        ++lex_beg;

    while(lex_beg != forward-1){
        s[i] = buf[lex_beg];

        if(lex_beg == buf_size-1)
            lex_beg = 0;
        else
            ++lex_beg;

        ++i;
    }

    if(lex_beg == buf_size-1)
        lex_beg = 0;
    else
        ++lex_beg;

    s[i] = '\0';

    if(i > 18){ 							//Not i >= 20 because pointer points to " finally.
        printf("Lexical Error, Invalid String, length > 20 in line number %d.\n",ln);
        return newToken("ERROR","e");
    }

    tokenInfo* temp = newToken("STR",s);

    return temp;
}

tokenInfo* checkFunId(){
    int len = 0;
    if(forward > lex_beg)
        len = forward - lex_beg;
    else
        len = buf_size - lex_beg + forward + 2;

    char *s = (char*)malloc(sizeof(char)*len);
    int i = 0;

    while(lex_beg != forward){
        s[i] = buf[lex_beg];

        if(lex_beg == buf_size-1)
            lex_beg = 0;
        else
            ++lex_beg;

        ++i;
    }

    s[i] = '\0';

    tokenInfo* temp = newToken("FUNID",s);

    return temp;
}

void handleLexErrorString(){
    char c = nextChar();
    while(c != '"' && c != '\n'){
        c = nextChar();
    }
    if(c == '\n'){
        retract(1);
        printf("Lexical error, expected closing \" for a string in line number %d\n",ln);
    }
    else if(c == '"'){
        printf("Lexical error, invalid character found in string in line number %d\n",ln);
    }
    lex_beg = forward;
}

void removeComments(){
    char c;
    while(1){
        if(buf[forward] == '\0')
            return;

        c = nextChar();

        if(c == '#'){
            if(buf[forward] == '\0')
                return;

            c = nextChar();

            while(c != '\n'){
                if(buf[forward] == '\0')
                    return;
                c = nextChar();
            }
        }
        printf("%c",c);
    }
}

tokenInfo* nextToken(){
    char c;
    while(1){

        if(buf[lex_beg] == '\0')
            return NULL;
        switch(state){
            case 0 : c = nextChar();

                if(c == ' ' || c == '\t'){
                    state = 0;
                    if(lex_beg == buf_size-1)
                        lex_beg = 0;
                    else
                        ++lex_beg;

                    forward = lex_beg;
                }

                else if(c == '\n'){
                    state = 0;
                    if(lex_beg == buf_size-1)
                        lex_beg = 0;
                    else
                        ++lex_beg;

                    forward = lex_beg;
                    ++ln;
                }

                else if(c == ';')
                    state = 1;

                else if(c == ',')
                    state = 2;

                else if(c == '+')
                    state = 3;

                else if(c == '-')
                    state = 4;

                else if(c == '*')
                    state = 5;

                else if(c == '/')
                    state = 6;

                else if(c == '[')
                    state = 7;

                else if(c == ']')
                    state = 8;

                else if(c == '(')
                    state = 9;

                else if(c == ')')
                    state = 10;

                else if(c == '@')
                    state = 11;

                else
                    state = fail();

                break;

            case 1 : lex_beg = forward;
                return newToken("SEMICOLON",";");

            case 2 : lex_beg = forward;
                return newToken("COMMA",",");

            case 3 : lex_beg = forward;
                return newToken("PLUS","+");

            case 4 : lex_beg = forward;
                return newToken("MINUS","-");

            case 5 : lex_beg = forward;
                return newToken("MUL","*");

            case 6 : lex_beg = forward;
                return newToken("DIV","/");

            case 7 : lex_beg = forward;
                return newToken("SQO","[");

            case 8 : lex_beg = forward;
                return newToken("SQC","]");

            case 9 : lex_beg = forward;
                return newToken("OP","(");

            case 10 : lex_beg = forward;
                return newToken("CL",")");

            case 11 : lex_beg = forward;
                return newToken("SIZE","@");

            case 12 : c = nextChar();
                if(isalpha(c))
                    state = 13;

                else
                    state = fail();

                break;

            case 13 : c = nextChar();
                if(isalpha(c))
                    state = 13;

                else if(isdigit(c)){
                    return getLookup();
                }

                else
                    state = 14;

                break;

            case 14 : retract(1);
                return getLookup();

            case 15 : c = nextChar();
                if(c == '=')
                    state = 16;

                else
                    state = fail();

                break;

            case 16 : c = nextChar();
                if(c == '='){
                    lex_beg = forward;
                    return newToken("EQ","==");
                }

                else if(c == '/')
                    state = 17;

                else{
                    retract(1);
                    lex_beg = forward;
                    return newToken("ASSIGNOP","=");
                }

                break;

            case 17 : c = nextChar();
                if(c == '='){
                    lex_beg = forward;
                    return newToken("NE","=/=");
                }

                else{
                    printf("Lexical Error, expected '=' after '/' at line number %d\n",ln);
                    retract(1);
                    lex_beg = forward;
                    return newToken("ERROR","e");
                }
                break;

            case 18 : c = nextChar();
                if(c == '>')
                    state = 19;

                else
                    state = fail();

                break;

            case 19 : c = nextChar();
                if(c == '='){
                    lex_beg = forward;
                    return newToken("GE",">=");
                }

                else{
                    retract(1);
                    lex_beg = forward;
                    return newToken("GT",">");
                }

                break;

            case 20 : c = nextChar();
                if(c == '<')
                    state = 21;

                else
                    state = fail();

                break;

            case 21 : c = nextChar();
                if(c == '='){
                    lex_beg = forward;
                    return newToken("LE","<=");
                }

                else{
                    retract(1);
                    lex_beg = forward;
                    return newToken("LT","<");
                }

                break;

            case 22 : c = nextChar();
                if(c == '"')
                    state = 23;

                else
                    state = fail();

                break;

            case 23 : c = nextChar();
                if(isalpha(c) || c == ' '){
                    state = 24;
                }

                else if(c == '"'){
                    printf("Lexical error, Empty string not allowed at line number %d\n",ln);
                    lex_beg = forward;
                    return newToken("ERROR","e");
                }

                else{
                    handleLexErrorString();
                    return newToken("ERROR","e");
                }

                break;

            case 24 : c = nextChar();
                if(isalpha(c) || c == ' '){
                    state = 24;
                }

                else if(c == '"'){
                    return checkForString();
                }

                else{
                    handleLexErrorString();
                    return newToken("ERROR","e");
                }

                break;

            case 25 : c = nextChar();
                if(isdigit(c))
                    state = 26;

                else
                    state = fail();

                break;

            case 26 : c = nextChar();
                if(isdigit(c))
                    state = 26;

                else if(c == '.')
                    state = 27;

                else{
                    retract(1);
                    return getNumToken(0);
                }

                break;

            case 27 : c = nextChar();
                if(isdigit(c))
                    state = 28;

                else{
                    retract(2);
                    return getNumToken(0);
                }

                break;

            case 28 : c = nextChar();
                if(isdigit(c)){
                    return getNumToken(1);
                }

                else{
                    printf("Lexical error, expected 2 digits after decimal point in line number %d.\n",ln);
                    retract(1);
                    lex_beg = forward;
                    return newToken("ERROR","e");
                }

                break;

            case 29 : c = nextChar();
                if(c == '.')
                    state = 30;

                else
                    state = fail();

                break;

            case 30 : c = nextChar();
                if(c == 'a')
                    state = 31;

                else if(c == 'n')
                    state = 32;

                else if(c == 'o')
                    state = 33;

                else{
                    printf("Lexical error, expected logical operator after '.' in line number %d.\n",ln);
                    retract(1);
                    lex_beg = forward;
                    return newToken("ERROR","e");
                }

                break;

            case 31 : c = nextChar();
                if(c == 'n')
                    state = 34;

                else{
                    printf("Lexical error, expected 'and', found something else in line number %d.\n",ln);
                    retract(1);
                    lex_beg = forward;
                    return newToken("ERROR","e");
                }

                break;

            case 32 : c = nextChar();
                if(c == 'o')
                    state = 35;

                else{
                    printf("Lexical error, expected 'not', found something else in line number %d.\n",ln);
                    retract(1);
                    lex_beg = forward;
                    return newToken("ERROR","e");
                }

                break;

            case 33 : c = nextChar();
                if(c == 'r')
                    state = 36;

                else{
                    printf("Lexical error, expected 'or', found something else in line number %d.\n",ln);
                    retract(1);
                    lex_beg = forward;
                    return newToken("ERROR","e");
                }

                break;

            case 34 : c = nextChar();
                if(c == 'd')
                    state = 37;

                else{
                    printf("Lexical error, expected 'and', found something else in line number %d.\n",ln);
                    retract(1);
                    lex_beg = forward;
                    return newToken("ERROR","e");
                }

                break;

            case 35 : c = nextChar();
                if(c == 't')
                    state = 38;

                else{
                    printf("Lexical error, expected 'not', found something else in line number %d.\n",ln);
                    retract(1);
                    lex_beg = forward;
                    return newToken("ERROR","e");
                }

                break;

            case 36 : c = nextChar();
                if(c == '.'){
                    lex_beg = forward;
                    return newToken("OR","or");
                }

                else{
                    printf("Lexical error, expected '.', found something else in line number %d.\n",ln);
                    retract(1);
                    lex_beg = forward;
                    return newToken("ERROR","e");
                }

                break;

            case 37 : c = nextChar();
                if(c == '.'){
                    lex_beg = forward;
                    return newToken("AND","and");
                }

                else{
                    printf("Lexical error, expected '.', found something else in line number %d.\n",ln);
                    retract(1);
                    lex_beg = forward;
                    return newToken("ERROR","e");
                }

                break;

            case 38 : c = nextChar();
                if(c == '.'){
                    lex_beg = forward;
                    return newToken("NOT","not");
                }

                else{
                    printf("Lexical error, expected '.', found something else in line number %d.\n",ln);
                    retract(1);
                    lex_beg = forward;
                    return newToken("ERROR","e");
                }

                break;

            case 39 : c = nextChar();
                if(c == '_')
                    state = 40;

                else
                    state = fail();

                break;

            case 40 : c = nextChar();
                if(c == 'm')
                    state = 41;

                else if(isalpha(c))
                    state = 42;

                else{
                    printf("Lexical error, expected an alphabet at the start of function name at line number %d.\n",ln);
                    retract(1);
                    lex_beg = forward;
                    return newToken("ERROR","e");
                }

                break;

            case 41 : c = nextChar();
                if(c == 'a')
                    state = 43;

                else if(isalpha(c) || isdigit(c))
                    state = 42;

                else{
                    retract(1);
                    return checkFunId();
                }

                break;

            case 42 : c = nextChar();
                if(isalpha(c) || isdigit(c))
                    state = 42;

                else{
                    retract(1);
                    return checkFunId();
                }

                break;

            case 43 : c = nextChar();
                if(c == 'i')
                    state = 44;

                else if(isalpha(c) || isdigit(c))
                    state = 42;

                else{
                    retract(1);
                    return checkFunId();
                }

                break;

            case 44 : c = nextChar();
                if(c == 'n')
                    state = 45;

                else if(isalpha(c) || isdigit(c))
                    state = 42;

                else{
                    retract(1);
                    return checkFunId();
                }

                break;

            case 45 : c = nextChar();
                if(isalpha(c) || isdigit(c))
                    state = 42;

                else{
                    retract(1);
                    lex_beg = forward;
                    return newToken("MAIN","main");
                }

                break;

            case 46 : c = nextChar();
                if(c == '#')
                    state = 47;

                else{
                	printf("Lexical Error, Unidentified token %c in line number %d.\n",c,ln);
                    lex_beg = forward;
                	return newToken("ERROR","e");
                }

                break;

            case 47 : c = nextChar();
                if(c != '\n')
                    state = 47;

                else{
                    tokenInfo* temp = newToken("COMMENT","#");
                    ++ln;
                    lex_beg = forward;
                    return temp;
                }

                break;

            default : fail();
        }
    }
}

tokenInfo* getNextLexedToken(){
    start = 0;
    state = 0;

    tokenInfo* temp = nextToken();

    if(temp == NULL)
        return temp;

    if(strcmp(temp->token,"ERROR") != 0)
        return temp;

    return getNextToken();
}

tokenInfo* getNextToken(){
    start = 0;
    state = 0;

    tokenInfo* temp = nextToken();

    if(temp == NULL)
        return temp;

    if(strcmp(temp->token,"ERROR") != 0 && strcmp(temp->token,"COMMENT") != 0)
        return temp;

    if(strcmp(temp->token,"ERROR") == 0)
        ++num_of_errors;

    return getNextToken();
}

void populateKeywordHashTable(){
    int i = 0;
    keyword_hash = createHashTable(NO_OF_KEYWORDS);
    for(i = 0; i < NO_OF_KEYWORDS; ++i){
        char* source = keywords[i];
        int hash_index = hash(source) % NO_OF_KEYWORDS;
        insertInHashTable(keyword_hash,hash_index,i,source);
    }
}

