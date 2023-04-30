sudo npm install -g syntax-cli

syntax-cli -g EvaGrammar.bnf -m LALR1 -o src/parser/EvaParser.h

clang++ -std=c++17 -Wall -ggdb3 eva-vm.cpp -o eval

pause in 7:16