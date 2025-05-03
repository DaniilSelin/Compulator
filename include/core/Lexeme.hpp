#ifndef LEXEME_HPP
#define LEXEME_HPP

#include <string>

struct Lexeme {
    int num;             // номер лексемы
    std::string value;   // значение лексемы
    int row;
    int pos;
};

#endif