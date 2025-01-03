#include <string.h>
#include <stdio.h>


enum Tokens{
  tok_eof = -1,
  tok_identifier = 1,
  tok_literal = 2,
  tok_semicolon = 3,
  tok_parenthesis = 4,
};

static char identifierstr[64];
static double literalval;


int lexer(char* file){
  return 20;
}


int main(int argc, char *argv[]){
  FILE *fptr;
  fptr = fopen(argv[1], "r");
  char filecontents[1024];
  fgets(filecontents, 1024, fptr);
  printf("%s", filecontents);
  
  return 0;
}


