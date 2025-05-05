build:
	g++ -Iinclude     include/lexer/lexer.cpp     include/lexer/semantics/semantics.cpp     include/lexer/LexemeLoader.cpp     -o program -std=c++17

run: build
	./program

clear:
	rm ./program