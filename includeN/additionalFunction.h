#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include "json.hpp"
#include <codecvt>
#include <sstream>

enum class State {
    S,    // Начальный нетерминал
    C,    // Целое число
    D,    // Символ точки
    I,    // Идентификатор
    E,    // Вещественное число
    U,    // Составная операция сравнения 
    O,    // Логическое НЕ
    K,    // Знак деления
    Q,    // Однострочный комментарий
    M,    // Конец многострочного комментария
    W,    // Однострочный комментарий
    A,    // Логическое ИЛИ
    B,    // Логическое И
    Z,    // Z
    Zs,   // Zs => Z*
};

struct Lexeme {
    int num;             // номер лексемы
    std::string value;   // значение лексемы
    int row;             // строка
    int pos;             // столбец
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

const std::unordered_map<std::string, int> KeywordMap = {
    {"==",     4},
    {"!=",     5},
    {">=",     6},
    {"<=",     7},
    {"||",     8},
    {"&&",     9},
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

void sem1(Context& ctx);
void sem2(Context& ctx);
void sem3(Context& ctx);
void sem4(Context& ctx);
void sem5(Context& ctx);
void sem6(Context& ctx);
void sem7(Context& ctx);
void sem8(Context& ctx);
void sem9(Context& ctx);
void sem10(Context& ctx);
void sem11(Context& ctx);
void sem12(Context& ctx);
void sem13(Context& ctx);
void sem14(Context& ctx);
void sem15(Context& ctx);

const std::unordered_map<int, void(*)(Context&)> SemanticHandlers = {
    {1, sem1}, 
    {2, sem2}, 
    {3, sem3}, 
    {4, sem4}, 
    {5, sem5}, 
    {6, sem6},
    {7, sem7}, 
    {8, sem8}, 
    {9, sem9}, 
    {10, sem10}, 
    {11, sem11}, 
    {12, sem12},
    {13, sem13}, 
    {14, sem14}, 
    {15, sem15}
};

// Удобная штука для объявления такого перехода: State → (CharClass → Transition)
using InnerMap = std::unordered_map<std::string, Transition>;
using Table = std::unordered_map<State, InnerMap>;

static const Table kTransitionTable = {
    { State::S, {
        { "D",      { State::C, 1 } },
        { "L",      { State::I, 2 } },
        { "(",      { State::Z, 3 } },
        { ")",      { State::Z, 3 } },
        { "[",      { State::Z, 3 } },
        { "]",      { State::Z, 3 } },
        { "{",      { State::Z, 3 } },
        { "}",      { State::Z, 3 } },
        { ",",      { State::Z, 3 } },
        { "+",      { State::Z, 3 } },
        { "-",      { State::Z, 3 } },
        { "*",      { State::Z, 3 } },
        { "/",      { State::K, 2 } },
        { "=",      { State::U, 2 } },
        { ">",      { State::U, 2 } },
        { "<",      { State::U, 2 } },
        { ";",      { State::Z, 3 } },
        { "|",      { State::A, 2 } },
        { "!",      { State::O, 2 } },
        { "&",      { State::B, 2 } },
        { " ",      { State::S, 15 } },
        { "\n",     { State::S, 4 } },
        { "Ʇ",      { State::Z, 5 } },
    }},

    { State::C, {
        { "D",      { State::C, 6 } },
        { ")",      { State::Zs, 7 } },
        { "]",      { State::Zs, 7 } },
        { "{",      { State::Zs, 7 } },
        { "}",      { State::Zs, 7 } },
        { ".",      { State::D, 8 } },
        { ",",      { State::Zs, 7 } },
        { "+",      { State::Zs, 7 } },
        { "-",      { State::Zs, 7 } },
        { "*",      { State::Zs, 7 } },
        { "/",      { State::Zs, 7 } },
        { "=",      { State::Zs, 7 } },
        { ">",      { State::Zs, 7 } },
        { "<",      { State::Zs, 7 } },
        { ";",      { State::Zs, 7 } },
        { "!",      { State::Zs, 7 } },
        { " ",      { State::Z, 7 } },
    }},

    { State::D, {
        { "D",      { State::E, 9 } },
    }},

    { State::I, {
        { "D",      { State::I, 10 } },
        { "L",      { State::I, 10 } },
        { "(",      { State::Zs,11 } },
        { ")",      { State::Zs,11 } },
        { "]",      { State::Zs,11 } },
        { "}",      { State::Zs,11 } },
        { ",",      { State::Zs,11 } },
        { "+",      { State::Zs,11 } },
        { "-",      { State::Zs,11 } },
        { "*",      { State::Zs,11 } },
        { "/",      { State::Zs,11 } },
        { "=",      { State::Zs,11 } },
        { ">",      { State::Zs,11 } },
        { "<",      { State::Zs,11 } },
        { ";",      { State::Zs,11 } },
        { "!",      { State::Zs,11 } },
        { " ",      { State::Z, 11 } },
    }},

    { State::U, {
        { "D",      { State::Zs, 13 } },
        { "L",      { State::Zs, 13 } },
        { "(",      { State::Zs, 13 } },
        { "-",      { State::Zs, 13 } },
        { "=",      { State::Z, 14 } },
        { " ",      { State::Z, 13 } }
    }},

    { State::O, {
        { "(",      { State::Zs, 13 } },
        { "=",      { State::Z, 14 } },
        { " ",      { State::Z, 13 } }
    }},

    { State::K, {
        { "D",      { State::Zs, 13 } },
        { "L",      { State::Zs, 13 } },
        { "(",      { State::Zs, 13 } },
        { "-",      { State::Zs, 13 } },
        { "*",      { State::Q, 15 } },
        { "/",      { State::W, 15 } },
        { " ",      { State::Z, 13 } }
    }},

    { State::Q, {
        { "D",      { State::Q, 15 } },
        { "L",      { State::Q, 15 } },
        { "(",      { State::Q, 15 } },
        { ")",      { State::Q, 15 } },
        { "[",      { State::Q, 15 } },
        { "]",      { State::Q, 15 } },
        { "{",      { State::Q, 15 } },
        { "}",      { State::Q, 15 } },
        { ".",      { State::Q, 15 } },
        { ",",      { State::Q, 15 } },
        { "+",      { State::Q, 15 } },
        { "-",      { State::Q, 15 } },
        { "*",      { State::M, 15 } },
        { "/",      { State::Q, 15 } },
        { "=",      { State::Q, 15 } },
        { ">",      { State::Q, 15 } },
        { "<",      { State::Q, 15 } },
        { ";",      { State::Q, 15 } },
        { "|",      { State::Q, 15 } },
        { "!",      { State::Q, 15 } },
        { "&",      { State::Q, 15 } },
        { " ",      { State::Q, 15 } },
        { "\n",     { State::Q, 4  } },
        { "U",      { State::Q, 15 } },
    }},

    { State::M, {
        { "D",      { State::Q, 15 } },
        { "L",      { State::Q, 15 } },
        { "(",      { State::Q, 15 } },
        { ")",      { State::Q, 15 } },
        { "[",      { State::Q, 15 } },
        { "]",      { State::Q, 15 } },
        { "{",      { State::Q, 15 } },
        { "}",      { State::Q, 15 } },
        { ".",      { State::Q, 15 } },
        { ",",      { State::Q, 15 } },
        { "+",      { State::Q, 15 } },
        { "-",      { State::Q, 15 } },
        { "*",      { State::M, 15 } },
        { "/",      { State::S, 15 } },
        { "=",      { State::Q, 15 } },
        { ">",      { State::Q, 15 } },
        { "<",      { State::Q, 15 } },
        { ";",      { State::Q, 15 } },
        { "|",      { State::Q, 15 } },
        { "!",      { State::Q, 15 } },
        { "&",      { State::Q, 15 } },
        { " ",      { State::Q, 15 } },
        { "\n",     { State::Q, 4  } },
        { "U",      { State::Q, 15 } },
    }},

    { State::W, {
        { "D",      { State::Q, 15 } },
        { "L",      { State::Q, 15 } },
        { "(",      { State::Q, 15 } },
        { ")",      { State::Q, 15 } },
        { "[",      { State::Q, 15 } },
        { "]",      { State::Q, 15 } },
        { "{",      { State::Q, 15 } },
        { "}",      { State::Q, 15 } },
        { ".",      { State::Q, 15 } },
        { ",",      { State::Q, 15 } },
        { "+",      { State::Q, 15 } },
        { "-",      { State::Q, 15 } },
        { "*",      { State::M, 15 } },
        { "/",      { State::S, 15 } },
        { "=",      { State::Q, 15 } },
        { ">",      { State::Q, 15 } },
        { "<",      { State::Q, 15 } },
        { ";",      { State::Q, 15 } },
        { "|",      { State::Q, 15 } },
        { "!",      { State::Q, 15 } },
        { "&",      { State::Q, 15 } },
        { " ",      { State::Q, 15 } },
        { "\n",     { State::S, 4  } },
        { "Ʇ",      { State::Zs, 15 } },
        { "U",      { State::Q, 15 } },
    }},

    { State::A, {
        { "|",      { State::Z, 14 } },
    }},

    { State::B, {
        { "&",      { State::Z, 14 } },
    }},
};

//std::unordered_map<int, std::string> g_lexemeDescriptions;
std::string loadTextFile(const std::string& filePath);
void outputResult(const std::vector<Lexeme>& output, const std::unordered_map<int, std::string>& g_lexemeDescriptions);
bool loadLexemeDescriptions(std::unordered_map<int, std::string>& g_lexemeDescriptions, const std::string &filePath);
void dumpLexemeDescriptions(const std::unordered_map<int, std::string>& g_lexemeDescriptions);
