#include <iostream>
#include <fstream>

char **tokenizer(){
  // returns a json of the tokens
}

char **parser(){
  // returns the AST tree
}

int main(int argc, char *argv[]){
  
  if (argc >= 2){
    std::ifstream ifs(argv[1]);
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );
  }
  else {
    std::cout << "give filename";
  }
  return 0;
}
