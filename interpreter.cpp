#include <iostream>
#include <fstream>

char **tokenizer(){
  // returns a json of the tokens
}

char **parser(){
  // returns the AST tree
}

int main(int argc, char *argv[]){
	std::cout << argv[1]; // this should be the filename like 'germspeakexample.gm'
	// read the file contents
  std::ifstream file(argv[1]);
  std::string line;

  while (std::getline(file, line)) {
    std::cout << line << std::endl;
  }
  return 0;
}
