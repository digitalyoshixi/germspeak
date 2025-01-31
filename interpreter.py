import sys
from enum import Enum, auto
import re 

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

class PrecedenceType(Enum):
    P_LOWEST = 0
    P_EQUALS = 1
    P_LESSGREATER = 2
    P_SUM = 3
    P_PRODUCT = 4
    P_PREFIX = 5
    P_CALL = 6
    P_INDEX = 7

PRECEDENCES : dict[Token, Precedences] = {
        Token.Plus : PrecedenceType.P_SUM,
        Token.Minus : PrecedenceType.P_SUM,
        Token.Slash : PrecedenceType.P_PRODUCT,
        Token.Star : PrecedenceType.P_PRODUCT,
}

class AST(object):
    def __init__(self, tokens):
        self.tokens = tokens 
        self.token = None
        self.symbol_table = {}
        self.define("<end>")




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
    else:
        print("give a filename")

if __name__ == "__main__":
    main()
