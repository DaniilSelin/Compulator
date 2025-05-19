build:
	g++ -Iinclude     include/lexer/lexer.cpp     include/genOPS/genOPS.cpp  include/interp/iterp.cpp  -o gс+- -std=c++17

run: build
	./gс+- erunda.trash

start: 
	./gс+- erunda.trash

clear:
	rm ./gс+-
