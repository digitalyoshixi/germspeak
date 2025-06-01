#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <fstream>
#include <string>
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
  CurlyLeftParen,
  CurlyRightParen,
  Comma,
  SemiColon,
  End,
};

struct Token{
  TokenType token_type;  // The token's Type Enum
  string lexeme;// the actual string value of the token
};

bool isstringdigit(string mystring){
  for (int i = 0; i < mystring.size();i++) {
    if (!isdigit(mystring[i])) return 0;
  }
  return 1;
}


class Tokenizer{
  public:
    Tokenizer(string filecontent){
      this->filecontent = filecontent;
      filelength = filecontent.size();
    };

    void printfilecontent(){
      cout << filecontent;
    }

    void resetcontent(string content){
      this->filecontent = content;
      this->index=0;
    }

    Token nextTok(){
      if (index < filelength){
        while (isspace(filecontent[index]) && index < filelength){
          index++;
        }
        if (filecontent[index] == '=') { 
          index++; 
          return {TokenType::Equal, "="};
        }
        else if (filecontent[index] == '+') {
          index++;
          return {TokenType::Plus, "+"};
        }
        else if (filecontent[index] == '-'){
          index++;
          return {TokenType::Minus, "-"};
        } 
        else if (filecontent[index] == '*'){
          index++;
          return {TokenType::Star, "*"};
        } 
        else if (filecontent[index] == '/'){
          index++;
          return {TokenType::Slash, "/"};
        }
        else if (filecontent[index] == '('){
          index++;
          return {TokenType::LeftParen, "("};
        } 
        else if (filecontent[index] == ')'){
          index++;
          return {TokenType::RightParen, ")"};
        }
        else if (filecontent[index] == '{'){
          index++;
          return {TokenType::CurlyLeftParen, "{"};
        }
        else if (filecontent[index] == '}'){
          index++;
          return {TokenType::CurlyRightParen, "}"};
        }
        else if (filecontent[index] == ','){
          index++;
          return {TokenType::Comma, ","};
        }
        else if (filecontent[index] == ';'){
          index++;
          return {TokenType::SemiColon, ";"};
        } 
        else if (index < filelength){
          // create a string to capture with
          string currentstring;
          while (index < filelength && isalnum(filecontent[index])){
            currentstring += filecontent[index++];
          }
          // check capture
          if (isstringdigit(currentstring)) return {TokenType::NumberLiteral, currentstring};
          // check keywords
          else if (currentstring.compare("germ") == 0) return {TokenType::Keyword, currentstring};
          // fallback
          else return {TokenType::Identifier, currentstring};
        }
        index++;
      }
      return {TokenType::End, ""};
    }
  private:
    string filecontent;
    int index = 0;
    int filelength; 
};

// -------------------- AST NODES ----------------
class Node{};
class ExprNode : public Node{
  public:
    ~ExprNode() = default;
};
class NumberNode : public ExprNode{
  double val; 
  public:
    NumberNode(double val){
      this->val = val;
    }
};
class VariableNode : public ExprNode{
  string identifier; 
  public:
    VariableNode(string identifier){
      this->identifier = identifier;
    }
};
class BinaryExprNode : public ExprNode{
  ExprNode *left;
  ExprNode *right;
  int op;
  public:
    BinaryExprNode(ExprNode *left, ExprNode *right, int op){
      this->left = left;
      this->right = right;
      this->op = op;
    }
};
class CallNode: public ExprNode{
  string functionname;
  vector<string> args;
  public:
    CallNode(string functionname, vector<string> args){
      this->functionname = functionname;
      this->args = args;
    }
};
class PrototypeNode{
  string functionname;
  vector<string> args;
  public:
    PrototypeNode(string functionname, vector<string> args){
      this->functionname = functionname;
      this->args = args;
    }
};
class FunctionNode : public Node {
  PrototypeNode *prototype;
  ExprNode *body;
  public:
    FunctionNode(PrototypeNode* prototype, ExprNode* body){
      this->prototype = prototype;
      this->body = body;
    } 
};

// ----------------- PARSER -------------------
class Parser{
  public:
    Parser(){}
    Node *parse(string content){
      this->content = content;
      this->tokenizer = new Tokenizer(content);
      currtok = tokenizer->nextTok();
      return parse();
    }
  private:
    string content;
    Token currtok;
    Tokenizer *tokenizer;
    map<char, int> BinOpPrecedence{
      {'<', 0},
      {'+', 10},
      {'-', 20},
      {'*', 30},
    };

    int GetPrecedence(char lex){
      int precedence = BinOpPrecedence[lex];
      if (precedence <= 0){
        return -1; // return the highest!
      }
      return precedence;
    }

    Node *parse(){
      // check for functions 
      if (currtok.token_type == TokenType::Keyword){
        if (currtok.lexeme.compare("germ") == 0) return parseGerm();
      } 
      return parseExpr();
    }
    ExprNode *parseExpr(){
      // parse for expressions
      ExprNode *LHS = parsePrimary();
      if (!LHS){
        cout << "invalid expression\n";
        return nullptr;
      }
      return parseBinOp(0, LHS);

    }
    // primary expressions
    ExprNode *parsePrimary(){
      if (currtok.token_type == TokenType::NumberLiteral) return parseNumber();
      else if (currtok.token_type == TokenType::Identifier) return parseIdentifier();
      else if (currtok.token_type == TokenType::LeftParen) return parseParen();
      return nullptr;
    }
    ExprNode *parseNumber(){
      double currval = stod(currtok.lexeme);
      currtok = tokenizer->nextTok();
      return new NumberNode(currval);
    }
    ExprNode *parseIdentifier(){
      string identifier = currtok.lexeme;
      currtok = tokenizer->nextTok();
      // we check if this is a simple variable, or a callee
      if (currtok.token_type == TokenType::LeftParen){
        // parse callee 
        vector<string> args;
        tokenizer->nextTok();
        while(currtok.token_type != TokenType::RightParen){
          if (currtok.token_type != TokenType::Comma ){
            args.push_back(currtok.lexeme);
          }
        }
        return new CallNode(identifier, args);
      }
      return new VariableNode(identifier);
    }
    ExprNode *parseParen(){
      currtok = tokenizer->nextTok();
      ExprNode *V = parseExpr();
      if (V){
        // eat ')'
        tokenizer->nextTok();
        return V;
      }
      return nullptr;
    }
    // binary expression
    ExprNode *parseBinOp(int opcode, ExprNode *LHS){
      // get the current token's binopcode 
      while (true){
        int binopcode = GetPrecedence(currtok.lexeme[0]);
        if (binopcode < opcode){
          return LHS;
        }
        currtok = tokenizer->nextTok(); // grab the next token after the operator
        ExprNode *RHS = parsePrimary();
        if (!RHS){
          return nullptr;
        }
        int nextopcode = GetPrecedence(currtok.lexeme[0]);
        if (binopcode < nextopcode){
          // add current RHS to the next parse binop 
          RHS = parseBinOp(binopcode+1, RHS);
          if (!RHS){
            return nullptr;
          }
        }
        LHS = new BinaryExprNode(LHS, RHS, binopcode);
      }
    }
    // functions
    PrototypeNode *parseProto(){
      // capture the first identifier
      string identifier = tokenizer->nextTok().lexeme;
      // capture '('
      if (currtok.token_type == TokenType::LeftParen){
        // parse callee 
        vector<string> args;
        tokenizer->nextTok();
        while(currtok.token_type != TokenType::RightParen){
          if (currtok.token_type != TokenType::Comma ){
            args.push_back(currtok.lexeme);
          }
        }
        return new PrototypeNode(identifier, args);
      }
      return nullptr;
    }
    FunctionNode *parseGerm(){
      PrototypeNode *proto = parseProto();
      if (!proto){
        return nullptr;
      }
      tokenizer->nextTok(); // capture '{'
      ExprNode *body = parseExpr();
      if (!body){
        return nullptr;
      }
      tokenizer->nextTok(); // capture '}'
      return new FunctionNode(proto, body);
    }


};

int main(int argc, char *argv[]){
  // Check if a filename argument is given. 
  if (argc >= 2){
    // Read the file contents
    ifstream ifs(argv[1]);
    string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );

    Tokenizer* tokenizer = new Tokenizer(content);
    tokenizer->printfilecontent();
    Token currtok;
    while (currtok.token_type != TokenType::End){
      currtok = tokenizer->nextTok();
      cout << currtok.token_type << " | " << currtok.lexeme << "\n";
    }
  }
  else {
    cout << "Acting as a REPL\n";
    Parser *parser = new Parser();
    while (true){
      cout << "> ";
      string content;
      cin >> content;
      cout << "parsed something! " << parser->parse(content) << "\n";
    }
  }
  return 0;
}
