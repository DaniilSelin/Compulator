build:
	g++ -Iinclude     include/lexer/lexer.cpp     include/genOPS/genOPS.cpp   -o program -std=c++17

run: build
	./program erunda.trash

clear:
	rm ./program erunda.trash