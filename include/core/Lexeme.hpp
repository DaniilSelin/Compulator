#ifndef LEXEME_HPP
#define LEXEME_HPP

#include <string>
#include <iostream>
#include <vector>

struct Lexeme {
    int num;             // номер лексемы
    std::string value;   // значение лексемы
    int row;             // строка
    int pos;             // столбец
};


// Перегрузка оператора вывода для одной лексемы
inline std::ostream&operator<<(std::ostream& os, const Lexeme& lex) {
    return os << "Lex " << lex.num 
              << ": '" << lex.value << "'"
              << " at (" << lex.row << ":" << lex.pos << ")";
}

// Функция для вывода вектора лексем
inline void printLexemes(const std::vector<Lexeme>& prog) {
    for (const auto& lex : prog) {
        std::cout << lex << std::endl;
    }
}

inline std::ostream& operator<<(std::ostream& os, const std::vector<Lexeme>& lexemes) {
    os << "[" << lexemes.size() << " lexemes]:\n";
    for (const auto& lex : lexemes) {
        os << "  " << lex << "\n";
    }
    return os;
}

#endif