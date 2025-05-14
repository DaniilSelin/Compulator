#ifndef STRUCTS_H
#define STRUCTS_H
#include <unordered_map>
#include <deque>
#include <any>
#include <locale.h>
#include <fstream>
#include <codecvt>
#include <sstream>
#include <cstdint>
#include <stdexcept>

#include "EnumClasses.h"
#include "Lexeme.h"

struct VarObject {
    std::uintptr_t addr;
    bool IsReal;
};

struct Literal {
    typeLexeme type;
    std::any value;
};

struct GenContext {
    std::unordered_map<std::string, VarObject> VarMap;
    std::deque<int> MarkVector;
    std::vector<Literal> OPS;
    int curLex = 0;
    bool InitReal = false;
    std::vector<Lexeme> prog;

    // Конструктор
    GenContext(const std::vector<Lexeme>& program): prog(program) {
        VarMap.clear();
        MarkVector.clear();
        OPS.clear();
    }
};

struct GenRules {
    std::vector<std::string> pattern;   // Правило перехода
    std::vector<int> semGen;            // Семантические программы для генератора ОПС
};

struct Transition {
    State nextState;
    int action; // Номер семантической программы
};

struct Context {
    std::vector<Lexeme> prog; // Список распознанных лексем
    std::string InputSTR; // входная строка
    int i = 0;
    // без встраивания структуры Lexeme для упрощения обращения к полям
    int n = 0;
    std::string name;
    int row = 1;
    int pos = 0;
    //
    double x = 0; // дробная часть
    double d = 0; // следующий десятичный разряд
};

#endif // STRUCTS_H
