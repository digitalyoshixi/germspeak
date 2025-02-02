import sys
from enum import Enum, auto
import re 
from typing import Callable

keywords = ["int", "printf", "return", "germ"]

# ------------------ Tokenizer ------------------------
# enums
class Token(Enum):
  NumberLiteral = 0
  Keyword = 1
  Identifier = 2
  Equal = 3
  Plus = 4
  Minus = 5
  Star = 6
  Slash = 7
  LeftParen = 8
  RightParen = 9
  Comma = 10
  SemiColon = 11

def isgoodascii(targetchar):
    return re.match("[aA-zZ_123457890]",targetchar)

def tokenizer(filecontents : str) -> list[(Token,str)]:
    retlist = []
    i = 0
    while i < len(filecontents):
        #print(f"< {filecontents[i]} >")
        if filecontents[i] == "(":
            retlist.append((Token.LeftParen,filecontents[i]))
        elif filecontents[i] == ")":
            retlist.append((Token.RightParen,filecontents[i]))
        elif filecontents[i] == "=":
            retlist.append((Token.Equal,filecontents[i]))
        elif filecontents[i] == "+":
            retlist.append((Token.Plus,filecontents[i]))
        elif filecontents[i] == "-":
            retlist.append((Token.Minus,filecontents[i]))
        elif filecontents[i] == "*":
            retlist.append((Token.Star,filecontents[i]))
        elif filecontents[i] == "/":
            retlist.append((Token.Slash,filecontents[i]))
        elif filecontents[i] == ",":
            retlist.append((Token.Comma,filecontents[i]))
        elif filecontents[i] == ";":
            retlist.append((Token.SemiColon,filecontents[i]))
        else:
            if isgoodascii(filecontents[i]):
                strcapture = ""
                strcapture += filecontents[i]
                while isgoodascii(filecontents[i+1]):
                    if i < len(filecontents)-1:
                        i = i + 1
                        strcapture += filecontents[i]
                # cases to check if identifier, num literal or string literal
                if strcapture in keywords:
                    retlist.append((Token.Keyword, strcapture))
                elif strcapture.isdigit():
                    retlist.append((Token.NumberLiteral, strcapture))
                else:
                    retlist.append((Token.Identifier, strcapture))
        i = i + 1 

    return retlist    
       
# ------------------ Parser and AST -----------------------
# creating the AST Tree helper functions

class NodeType(Enum):
    # Tree Types
    Program = "Program"
    # Statements
    ExpressionStatement = "ExpressionStatement"
    # Expressions
    InfixExpression = "InfixExpression"
    # Literals
    NumberLiteral = "NumberLiteral"

class Node():
    def type(self) -> NodeType:
        pass 
    # pure debugging to be able to see AST in json format
    def json(self) -> dict: 
        pass 

class Statement(Node):
    pass

class Expression(Node):
    pass

class Program(Node):
    def __init__(self) -> None:
        self.statements : list[Statement] = []

    def type(self) -> NodeType:
        return NodeType.Program
    
    def json(self) -> dict: 
        return {
                "type" : self.type().value,
                # list comprehension
                "statements" : [{stmt.type().value : stmt.json()} for stmt in self.statements]
            }
# Statements

class ExpressionStatement(Statement):
    def __init__(self, expr : Expression = None) -> None:
        self.expr : Expression = expr 

    def type(self) -> NodeType:
        return NodeType.ExpressionStatement

    def json(self) -> dict:
        return {
                "type" : self.type().value,
                "expr" : self.expr.json()
                }

class InfixExpression(Expression):
    def __init__(self, left_node : Expression, operator : str, right_node : Expression = None):
        self.left_node : Expression = left_node
        self.operator : str = operator
        self.right_node : Expression = right_node

    def type(self):
        return NodeType.InfixExpression

    def json(self) -> dict:
        return {
                "type" : self.type().value,
                "left_node" : self.left_node.json(),
                "operator" : self.operator,
                "right_node" : self.right_node.json()
                }

class NumberLiteral(Expression):
    def __init__(self, value : int = None) -> None:
        self.value : int = value

    def type(self) -> NodeType:
        return NodeType.NumberLiteral

    def json(self) -> dict:
        return {
                "type" : self.type().value,
                "value" : self.value
            }



# creating the Parser Object
class PrecedenceType(Enum):
    P_LOWEST = 0
    P_EQUALS = 1
    P_LESSGREATER = 2
    P_SUM = 3
    P_PRODUCT = 4
    P_PREFIX = 5
    P_CALL = 6
    P_INDEX = 7

PRECEDENCES : dict[Token, PrecedenceType] = {
        Token.Plus : PrecedenceType.P_SUM,
        Token.Minus : PrecedenceType.P_SUM,
        Token.Slash : PrecedenceType.P_PRODUCT,
        Token.Star : PrecedenceType.P_PRODUCT,
}

class Parser(object):
    def __init__(self, tokens):
        self.tokens = tokens 
        self.token_counter = 0
        self.current_token = tokens[0] # 
        self.peek_token = tokens[1] # next token we peek at
        self.errors= []

        # parsing behaviors depending on tokens 
        self.prefix_parse_fns : dict[Token, Callable]=  {
            Token.NumberLiteral : self.parse_int_literal,
            Token.LeftParen : self.parse_grouped_expression
        }
        self.infix_parse_fns : dict[Token, Callable]=  {
            Token.Plus : self.parse_infix_expression,
            Token.Minus : self.parse_infix_expression,
            Token.Slash : self.parse_infix_expression,
            Token.Star : self.parse_infix_expression,
        }

    # token incrementing
    def next_token(self) -> None:
        print(self.token_counter, self.tokens[self.token_counter])
        self.token_counter = self.token_counter + 1 
        self.current_token = self.tokens[self.token_counter]
        self.peek_token = self.tokens[self.token_counter+1]

    # operator binding power precendence
    def current_precedence(self) -> PrecedenceType:
        # get the precedence enum of the current token
        precedence = PRECEDENCES.get(self.tokens[self.token_counter][0])
        if precedence == None:
            return PrecedenceType.P_LOWEST
        return precedence

    def peek_precedence(self):
        # get the precedence enum of the peek token
        precedence = PRECEDENCES.get(self.tokens[self.token_counter+1][0])
        if precedence == None:
            return PrecedenceType.P_LOWEST
        return precedence


    # errors
    def expect_error(self, tt : Token, got : Token):
        self.errors.append(f"Expected {tt} and got {got} instead")
    def prefix_error(self, tt : Token):
        self.errors.append(f"No prefix function for {tt}, you are probably using the wrong operator")
    
    # main parsing loop 
    def parse_program(self) -> None:
        program = Program()
        while self.token_counter < len(self.tokens) -1:
            stmt : Statement = self.parse_statement()
            if stmt is not None:
                program.statements.append(stmt)
        
            self.next_token()
        return program

    def parse_statement(self) -> Statement:
        # since there is only one type of statement, it is just this for now
        return self.parse_expression_statement()

    def parse_expression_statement(self) -> Statement:
        expr = self.parse_expression(PrecedenceType.P_LOWEST)
        #if self.peek_token[0] == Token.SemiColon:
        #    print("is this troublesome?")
        #    self.next_token() # this might cause some trouble, look over this later
        stmt = ExpressionStatement(expr=expr)
        return stmt
    def parse_expression(self, precedence : PrecedenceType) -> Expression:
        # getting the parsing function based off current token
        prefix_fn = self.prefix_parse_fns.get(self.current_token[0])
        if prefix_fn is None:
            self.prefix_error(self.current_token[0])
            return None
        # run the prefix function if there is one
        left_expr = prefix_fn()
        # continuously check for infix operators 
        while not (self.peek_token == Token.SemiColon) and precedence.value < self.peek_precedence().value:
            infix_fn = self.infix_parse_fns.get(self.peek_token[0])
            if infix_fn is None:
                return left_expr
            left_expr = infix_fn(left_expr)
        return left_expr

    def parse_infix_expression(self, left_node : Expression) -> Expression:
        infix_expr = InfixExpression(left_node = left_node, operator=self.current_token[1])
        precedence = self.current_precedence()
        self.next_token()
        infix_expr.right_node = self.parse_expression(precedence)
        return infix_expr
    def parse_grouped_expression(self) -> Expression:
        self.next_token()
        expr : Expression = self.parse_expression(PrecedenceType.P_LOWEST)
        if not self.peek_token == Token.RightParen:
            return None
        return expr
    
    def parse_int_literal(self) -> Expression:
        int_lit : NumberLiteral = NumberLiteral()

        try:
            int_lit.value = int(self.current_token[1])
        except:
            self.errors.append(f"Could not parse {self.current_token[1]} as {Token.NumberLiteral}")
        return int_lit



# ------------------ MAIN -----------------------

def main():
    # read the file
    if (len(sys.argv) >= 2):
        print(sys.argv[1])
        f = open(sys.argv[1])
        filecontents = f.read()
        print("-----------")
        print(filecontents)
        print("-----------")
        tokens = tokenizer(filecontents)
        for i in tokens:
            print(i)
        f.close() 
        parser = Parser(tokens)
        program = parser.parse_program()
        print(program.json())
    else:
        print("give a filename")

if __name__ == "__main__":
    main()
