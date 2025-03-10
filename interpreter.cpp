#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
using namespace std;


enum TokenType{
  NumberLiteral,
  Keyword,
  Identifier,
  Equal,
  Plus,
  Minus,
  Star,
  Slash,
  LeftParen,
  RightParen,
  Comma,
  SemiColon,
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
    //cout << "POS: " << pos << " char : " << currentchar << endl;
    // checking if it is a one-word char
    if (currentchar == '='){
      //cout << currentchar << endl;
      TokenType currenttokentype = Equal;
      struct Token currenttoken = {currenttokentype,"="};
      returnvec.push_back(currenttoken);
    }
    else if (currentchar == '('){
      //cout << currentchar << endl;
      TokenType currenttokentype = LeftParen;
      struct Token currenttoken = {currenttokentype,"("};
      returnvec.push_back(currenttoken);
    }
    else if (currentchar == ')'){
      //cout << currentchar << endl;
      TokenType currenttokentype = RightParen;
      struct Token currenttoken = {currenttokentype,")"};
      returnvec.push_back(currenttoken);
    }
    else if (currentchar == '+'){
      //cout << currentchar << endl;
      TokenType currenttokentype = Plus;
      struct Token currenttoken = {currenttokentype,"+"};
      returnvec.push_back(currenttoken);
    }
    else if (currentchar == '-'){
      //cout << currentchar << endl;
      TokenType currenttokentype = Minus;
      struct Token currenttoken = {currenttokentype,"-"};
      returnvec.push_back(currenttoken);
    }
    else if (currentchar == '*'){
      //cout << currentchar << endl;
      TokenType currenttokentype = Minus;
      struct Token currenttoken = {currenttokentype,"*"};
      returnvec.push_back(currenttoken);
    }
    else if (currentchar == '/'){
      //cout << currentchar << endl;
      TokenType currenttokentype = Minus;
      struct Token currenttoken = {currenttokentype,"/"};
      returnvec.push_back(currenttoken);
    }
    else if (currentchar == ','){
      //cout << currentchar << endl;
      TokenType currenttokentype = Comma;
      struct Token currenttoken = {currenttokentype,","};
      returnvec.push_back(currenttoken);
    }
    else if (currentchar == ';'){
      //cout << currentchar << endl;
      TokenType currenttokentype = SemiColon;
      struct Token currenttoken = {currenttokentype,";"};
      returnvec.push_back(currenttoken);
    }
    // checking numeric literals
    else if (48 <= (int)currentchar && (int)currentchar <= 57){
      // check next char
      //cout << currentchar << endl;
      TokenType currenttokentype = NumberLiteral;
      string numliteral = "";
      numliteral += currentchar;
      
      currentchar = filecontent[++pos]; // get the next character
      while (48 <= (int)currentchar && (int)currentchar <= 57){
        numliteral += currentchar;
        currentchar = filecontent[++pos]; // get the next character
      }
      //cout << "NUMBER IS : " << numliteral << endl;
      struct Token currenttoken = {currenttokentype,numliteral};
      returnvec.push_back(currenttoken);
    }
    // checking words. see if its in a-zA-Z
    else if ((97 <= (int)currentchar && (int)currentchar <= 122) || (65 <= (int)currentchar && (int)currentchar <= 90)){
      string strliteral = "";
      strliteral += filecontent[pos];
      while (filecontent[pos+1] != ' ' && filecontent[pos+1] != '\t' && filecontent[pos+1] != '\n' && filecontent[pos+1] != ';' && filecontent[pos+1] != ')' && filecontent[pos+1] != '(' && filecontent[pos+1] != ','){
        strliteral += filecontent[pos+1]; // add the next character
        pos++; // increase the iterator
      }
      // check if reserved keyword
      if (strliteral.compare("int") == 0){
        TokenType currenttokentype = Keyword;
        struct Token currenttoken = {currenttokentype,strliteral};
        returnvec.push_back(currenttoken);
      }
      else if(strliteral.compare("germ") == 0){
        TokenType currenttokentype = Keyword;
        struct Token currenttoken = {currenttokentype,strliteral};
        returnvec.push_back(currenttoken);

      }
      else if(strliteral.compare("printf") == 0){
        TokenType currenttokentype = Keyword;
        struct Token currenttoken = {currenttokentype,strliteral};
        returnvec.push_back(currenttoken);

      }
      else if(strliteral.compare("return")== 0){
        TokenType currenttokentype = Keyword;
        struct Token currenttoken = {currenttokentype,strliteral};
        returnvec.push_back(currenttoken);

      }
      else if(strliteral.compare("char")== 0){
        TokenType currenttokentype = Keyword;
        struct Token currenttoken = {currenttokentype,strliteral};
        returnvec.push_back(currenttoken);

      }
      else{
        TokenType currenttokentype = Identifier;
        struct Token currenttoken = {currenttokentype,strliteral};
        returnvec.push_back(currenttoken);
      }
      //cout << "String Literal IS : " << strliteral << endl;
      // construct the word from before whitespace
    }
    // increment the file pointer position
    pos++;
  }
  return returnvec;

}

// Creating the parser objects

class ExprAST { // Base class for all expressions
  // constructor 
  public:
    virtual ~ExprAST() = default;
};

class NumberExprAST {
  // stores the current value for the number
  double Val;
  // constructor
  public:
    NumberExprAST(double Val) : Val(Val) {}
};

// AST class to capture the variable name
class VariableExprAST {
  std::string Name;
  // constructor
  public:
    VariableExprAST(const std::string &Name) : Name(Name) {}
};

/// AST class to capture binary operation
class BinaryExprAST : public ExprAST {
  char Op;
  ExprAST* LHS;
  ExprAST* RHS;
  // constructor
  public:
    BinaryExprAST(char Op, ExprAST* LHS, ExprAST* RHS) : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
  std::string Callee;
  std::vector<ExprAST*> Args;
  // constructor
  public:
    CallExprAST(const std::string &Callee, std::vector<ExprAST*> Args) : Callee(Callee), Args(std::move(Args)) {}
};
// PrototypeSAT - Basic AST tree object that is used for function AST
class PrototypeAST {
  std::string Name;
  std::vector<std::string> Args;

  public:
    PrototypeAST(const std::string &Name, std::vector<std::string> Args) : Name(Name), Args(std::move(Args)) {}
    const std::string &getName() const { return Name; }
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
  PrototypeAST* Proto;
  ExprAST* Body;

public:
  FunctionAST(PrototypeAST* Proto, ExprAST* Body) : Proto(std::move(Proto)), Body(std::move(Body)) {}
};

// Grabs each next token and then perpetually push to 
ExprAST* parser(){
  // make the root node
  ExprAST* root_node = new ExprAST();
  return root_node;
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

    for (size_t i = 0; i < filetokens.size(); ++i) {
        const auto& token = filetokens[i];
        cout << "Token\t" << i << "\tToken Type : " << token.token_type << "\tToken Lexem : " << token.lexeme << endl;
    }

  }
  else {
    cout << "give filename";
  }

  return 0;
}
