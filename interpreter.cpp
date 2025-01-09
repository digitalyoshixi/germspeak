#include <iostream>
#include <vector>
#include <fstream>
using namespace std;


enum TokenType{
  NumberLiteral,
  Identifier,
  Equal,
  Plus,
  Minus,
  Star,
  Slash,
  LeftParen,
  RightParen,
};

struct Token{
  TokenType token_type;  // The token's Type Enum
  string lexeme;// the actual string value of the token
};


vector<Token> tokenizer(string filecontent){
  // returns a list of tokens
  int pos = 0;
  vector<Token> returnvec;
  while (pos < filecontent.size()){

    char currentchar = filecontent[pos];
    cout << "POS: " << pos << " char : " << currentchar << endl;
    // checking if it is a one-word char
    if (currentchar == '='){
      cout << currentchar << endl;
      TokenType currenttokentype = Equal;
      struct Token currenttoken = {currenttokentype,"="};
      returnvec.push_back(currenttoken);
    }
    else if (currentchar == '('){
      cout << currentchar << endl;
      TokenType currenttokentype = LeftParen;
      struct Token currenttoken = {currenttokentype,"("};
      returnvec.push_back(currenttoken);
    }
    else if (currentchar == ')'){
      cout << currentchar << endl;
      TokenType currenttokentype = RightParen;
      struct Token currenttoken = {currenttokentype,")"};
      returnvec.push_back(currenttoken);
    }
    else if (currentchar == '+'){
      cout << currentchar << endl;
      TokenType currenttokentype = Plus;
      struct Token currenttoken = {currenttokentype,"+"};
      returnvec.push_back(currenttoken);
    }
    else if (currentchar == '-'){
      cout << currentchar << endl;
      TokenType currenttokentype = Minus;
      struct Token currenttoken = {currenttokentype,"-"};
      returnvec.push_back(currenttoken);
    }
    else if (currentchar == '*'){
      cout << currentchar << endl;
      TokenType currenttokentype = Minus;
      struct Token currenttoken = {currenttokentype,"*"};
      returnvec.push_back(currenttoken);
    }
    else if (currentchar == '/'){
      cout << currentchar << endl;
      TokenType currenttokentype = Minus;
      struct Token currenttoken = {currenttokentype,"/"};
      returnvec.push_back(currenttoken);
    }
    // checking numeric literals
    else if (48 <= (int)currentchar && (int)currentchar <= 57){
      // check next char
      cout << currentchar << endl;
      TokenType currenttokentype = NumberLiteral;
      string numliteral = "";
      numliteral += currentchar;
      
      currentchar = filecontent[++pos]; // get the next character
      while (48 <= (int)currentchar && (int)currentchar <= 57){
        numliteral += currentchar;
        currentchar = filecontent[++pos]; // get the next character
      }
      cout << "NUMBER IS : " << numliteral << endl;

      struct Token currenttoken = {currenttokentype,numliteral};
      returnvec.push_back(currenttoken);
    }
    // increment the file pointer position
    pos++;
  }
  return returnvec;

}

char **parser(){
  // returns the AST tree
}

int main(int argc, char *argv[]){
  // Check if a filename argument is given. 
  if (argc >= 2){
    // Read the file contents
    ifstream ifs(argv[1]);
    string content( (istreambuf_iterator<char>(ifs) ),
                         (istreambuf_iterator<char>()    ) );
    
    cout << "File " << argv[1] << " read:\n-------------------------------\n";
    cout << content;
    cout << "-------------------------------\n";
    vector<Token> filetokens = tokenizer(content);

//    for (size_t i = 0; i < filetokens.size(); ++i) {
//        const auto& token = filetokens[i];
//        cout << "Token " << i << ", Member2: " << token.lexeme << endl;
//    }

  }
  else {
    cout << "give filename";
  }

  return 0;
}
