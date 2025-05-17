#ifndef STRUCTS_H
#define STRUCTS_H

#include "core/Lexeme.hpp"
#include "core/Literal.hpp"
#include <unordered_map>
#include <deque>
#include <any>

enum class GenState {
    A,
    P,
    Q,
    B,
    C,
    D,
    S,
    U,
    T,
    V,
    F,
    G,
    H,
    L,
    W,
    M,
    X,
    N,
    O,
    E,
    Z
};

struct GenContext {
    std::deque<int> MarkVector;
    std::vector<std::unique_ptr<Literal>> OPS;
    int curLex = 0;
    bool InitReal = false;
    std::vector<Lexeme> prog;

    // Конструктор
    GenContext(const std::vector<Lexeme>& program): prog(program) {
        MarkVector.clear();
        OPS.clear();
    }
};

struct GenRules {
    std::vector<std::string> pattern;   // Правило перехода
    std::vector<int> semGen;            // Семантические программы для генератора ОПС
};

#endif // STRUCTS_H