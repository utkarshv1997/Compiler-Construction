#Name- UTKARSH VASHISHTHA
#ID- 2015A7PS0130P

all: driver.o set.o hashTable.o tree.o lexer.o firstFollow.o stack.o parser.o AST.o semanticsChecker.o symHash.o codegen.o
	gcc driver.o set.o hashTable.o tree.o lexer.o firstFollow.o stack.o parser.o AST.o semanticsChecker.o symHash.o codegen.o -o compiler -lm

driver.o: driver.c
	gcc -c driver.c
	
set.o: set.c
	gcc -c set.c
	
hashTable.o: hashTable.c
	gcc -c hashTable.c

tree.o: tree.c
	gcc -c tree.c

lexer.o: lexer.c
	gcc -c lexer.c
	
firstFollow.o: firstFollow.c
	gcc -c firstFollow.c

stack.o: stack.c
	gcc -c stack.c
	
parser.o: parser.c
	gcc -c parser.c

AST.o: AST.c
	gcc -c AST.c

symHash.o: symHash.c
	gcc -c symHash.c

semanticsChecker.o: semanticsChecker.c
	gcc -c semanticsChecker.c

codegen.o: codegen.c
	gcc -c codegen.c

clean:
	rm -rf *o compiler
