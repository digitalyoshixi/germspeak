# germspeak
A simple imperative programming lanuage created that DOES NOT collect garbage.

# TODO:
- [ ] codegen
Now, I want to make clear the only shared values between the node is the codegen() function, and a few more debug things that each node will have. All other values are private
# Documentation
### Keywords
- `germ` : the keyword for definind a fn
### Operations
- `+` : add two integers
- `-` : subtract integers
- `*` : multiply integers
- `/` : integer divide integers

g++ -o ./interpreter -g -lLLVM interpreter.cpp && gdb ./interpreter
