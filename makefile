build:
	g++ -Iinclude     include/lexer/lexer.cpp     include/genOPS/genOPS.cpp  include/interp/iterp.cpp  -o program -std=c++17

run: build
	./program erunda.trash

start: 
	./program erunda.trash

clear:
	rm ./program erunda.trash