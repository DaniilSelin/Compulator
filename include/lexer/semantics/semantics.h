#pragma once

#include "core/Lexeme.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

// Контекст для семантических функций
// Нагло укал из Go, просто чтобы давать текущее состояние в одной структуре, а не куча аргументов
struct Context {
    std::vector<Lexeme> prog;
    std::string InputSTR;
    int i = 0;
    // без встраивания структур, хз как это делать вне Го красиво
    int n = 0;
    std::string name;
    int row = 1;
    int pos = 0;
    double x = 0; // тута дробное храним
    double d = 0; // некст десятичный разряд
};

const std::unordered_map<std::string,int> KeywordMap = {
	{"==", 4},
    {"!=", 5},
    {">=", 6},
    {"<=", 7},
    {"||", 8},
    {"&&", 9},
    {"if",    10},
    {"else",  11},
    {"while", 12},
    {"scan",  13},
    {"print", 14},
    {"abs",   15},
    {"sqrt",  16},
    {"ln",    17},
    {"lg",    18},
    {"exp",   19},
    {"int",   20},
    {"real",  21}
};

extern const std::unordered_map<int, void(*)(Context&)> SemanticHandlers;