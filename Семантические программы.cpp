// C[i] - текущий входной символ, name - символьная строка, n - целочисленная переменная,
// x, d - вещественные переменные, row - номер текущей строки, pos - позиция текущего символа в строке,
// prog - список распознанных лексем программы (vector<Lexeme>).
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
using namespace std;

struct Lexeme{
	int num;
	string value;
	int row;
	int pos;
};

// 1
n = C[i] - '0';

// 2
name = C[i];

// 3
prog.push_back(Lexeme{int(C[i]), string(1, C[i]), row, pos});

// 4
row++;
pos = 0;

// 5
prog.push_back(Lexeme{20, string(1, C[i]), row, pos});

// 6
n = 10 * n + C[i] - '0';

// 7
prog.push_back(Lexeme{2, to_string(n), row, pos});

// 8
d = 1; x = n;

// 9
d *= 0.1;
x += (C[i] - '0') * d;

// 10
name += C[i];

// 11 Поменять на map
if (name == "if")
	prog.push_back(Lexeme{10, name, row, pos});
else if (name == "else")
	prog.push_back(Lexeme{11, name, row, pos});
else if (name == "while")
	prog.push_back(Lexeme{12, name, row, pos});
else if (name == "scan")
	prog.push_back(Lexeme{13, name, row, pos});
else if (name == "print")
	prog.push_back(Lexeme{14, name, row, pos});
else if (name == "abs")
	prog.push_back(Lexeme{15, name, row, pos});
else if (name == "sqrt")
	prog.push_back(Lexeme{16, name, row, pos});
else if (name == "ln")
	prog.push_back(Lexeme{17, name, row, pos});
else if (name == "lg")
	prog.push_back(Lexeme{18, name, row, pos});
else if (name == "exp")
	prog.push_back(Lexeme{19, name, row, pos});
else if (name == "int")
	prog.push_back(Lexeme{20, name, row, pos});
else if (name == "real")
	prog.push_back(Lexeme{21, name, row, pos});
else
	prog.push_back(Lexeme{1, name, row, pos});

// 12
prog.push_back(Lexeme{3, to_string(x), row, pos});

// 13
prog.push_back(Lexeme{int(name[0]), name, row, pos});

// 14 Поменять на map
name += C[i];
if (name == "==")
	prog.push_back(Lexeme{4, name, row, pos});
else if (name == "!=")
	prog.push_back(Lexeme{5, name, row, pos});
else if (name == ">=")
	prog.push_back(Lexeme{6, name, row, pos});
else if (name == "<=")
	prog.push_back(Lexeme{7, name, row, pos});
else if (name == "||")
	prog.push_back(Lexeme{8, name, row, pos});
else if (name == "&&")
	prog.push_back(Lexeme{9, name, row, pos});