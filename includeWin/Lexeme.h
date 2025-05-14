#ifndef LEXEME_H
#define LEXEME_H
#include <string>
#include <iostream>

struct Lexeme {
    int num;             // номер лексемы
    std::string value;   // значение лексемы
    int row;             // строка
    int pos;             // столбец
};

// Перегрузка оператора вывода для одной лексемы
inline std::ostream&operator<<(std::ostream& os, const Lexeme& lex) {
    return os << "Лексема " << lex.num
              << ": '" << lex.value << "'"
              << " на позиции [" << lex.row << ";" << lex.pos << "]";
}

inline std::ostream& operator<<(std::ostream& os, const std::vector<Lexeme>& lexemes) {
    os << "Всего [" << lexemes.size() << "] лексем:\n";
    for (const auto& lex : lexemes) {
        os << "  " << lex << "\n";
    }
    return os;
}

#endif // LEXEME_H
