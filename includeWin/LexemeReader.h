#ifndef LEXEMEREADER_H
#define LEXEMEREADER_H
#include "Structs.h"

std::vector<Lexeme> lexemeReader(Context& ctx, const std::string& inputString);

using InnerMap = std::unordered_map<std::string, Transition>;
using Table = std::unordered_map<State, InnerMap>;

void LexSem1(Context& ctx);
void LexSem2(Context& ctx);
void LexSem3(Context& ctx);
void LexSem4(Context& ctx);
void LexSem5(Context& ctx);
void LexSem6(Context& ctx);
void LexSem7(Context& ctx);
void LexSem8(Context& ctx);
void LexSem9(Context& ctx);
void LexSem10(Context& ctx);
void LexSem11(Context& ctx);
void LexSem12(Context& ctx);
void LexSem13(Context& ctx);
void LexSem14(Context& ctx);
void LexSem15(Context& ctx);

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

const std::unordered_map<int, void(*)(Context&)> SemanticHandlers = {
    {1, LexSem1},
    {2, LexSem2},
    {3, LexSem3},
    {4, LexSem4},
    {5, LexSem5},
    {6, LexSem6},
    {7, LexSem7},
    {8, LexSem8},
    {9, LexSem9},
    {10, LexSem10},
    {11, LexSem11},
    {12, LexSem12},
    {13, LexSem13},
    {14, LexSem14},
    {15, LexSem15}
};


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
                { "\t",     { State::S, 15 } },
                { "\n",     { State::S, 4 } },
                { "#",      { State::Z, 5 } },
                }},

    { State::C, {
                { "D",      { State::C, 6 } },
                { ")",      { State::Zs, 7 } },
                { "]",      { State::Zs, 7 } },
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
                { "\t",     { State::Z, 7 } },
                }},

    { State::D, {
                { "D",      { State::E, 9 } },
                }},

    { State::I, {
                { "D",      { State::I, 10 } },
                { "L",      { State::I, 10 } },
                { "(",      { State::Zs,11 } },
                { ")",      { State::Zs,11 } },
                { "[",      { State::Zs,11 } },
                { "]",      { State::Zs,11 } },
                { "{",      { State::Zs,11 } },
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
                { "\t",     { State::Z, 11 } },
                }},

    { State::E, {
                { "D",  { State::E, 9 } },
                { ")",  { State::Zs, 12 } },
                { "]",  { State::Zs, 12 } },
                { "}",  { State::Zs, 12 } },
                { ",",  { State::Zs, 12 } },
                { "+",  { State::Zs, 12 } },
                { "-",  { State::Zs, 12 } },
                { "*",      { State::Zs, 12 } },
                { "/",      { State::Zs, 12 } },
                { "=",      { State::Zs, 12 } },
                { ">",      { State::Zs, 12 } },
                { "<",      { State::Zs, 12 } },
                { ";",      { State::Zs, 12 } },
                { "!",      { State::Zs, 12 } },
                { " ",      { State::Z, 12 } },
                { "\t",     { State::Z, 12 } },
                }},

    { State::U, {
                { "D",      { State::Zs, 13 } },
                { "L",      { State::Zs, 13 } },
                { "(",      { State::Zs, 13 } },
                { "-",      { State::Zs, 13 } },
                { "=",      { State::Z, 14 } },
                { " ",      { State::Z, 13 } },
                { "\t",     { State::Z, 13 } },
                }},

    { State::O, {
                { "(",      { State::Zs, 13 } },
                { "=",      { State::Z, 14 } },
                { " ",      { State::Z, 13 } },
                { "\t",     { State::Z, 13 } },
                }},

    { State::K, {
                { "D",      { State::Zs, 13 } },
                { "L",      { State::Zs, 13 } },
                { "(",      { State::Zs, 13 } },
                { "-",      { State::Zs, 13 } },
                { "*",      { State::Q, 15 } },
                { "/",      { State::W, 15 } },
                { " ",      { State::Z, 13 } },
                { "\t",     { State::Z, 13 } },
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
                { "\t",     { State::Q, 15 } },
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
                { "\t",     { State::Q, 15 } },
                { "\n",     { State::Q, 4  } },
                { "U",      { State::Q, 15 } },
                }},

    { State::W, {
                { "D",      { State::W, 15 } },
                { "L",      { State::W, 15 } },
                { "(",      { State::W, 15 } },
                { ")",      { State::W, 15 } },
                { "[",      { State::W, 15 } },
                { "]",      { State::W, 15 } },
                { "{",      { State::W, 15 } },
                { "}",      { State::W, 15 } },
                { ".",      { State::W, 15 } },
                { ",",      { State::W, 15 } },
                { "+",      { State::W, 15 } },
                { "-",      { State::W, 15 } },
                { "*",      { State::W, 15 } },
                { "/",      { State::W, 15 } },
                { "=",      { State::W, 15 } },
                { ">",      { State::W, 15 } },
                { "<",      { State::W, 15 } },
                { ";",      { State::W, 15 } },
                { "|",      { State::W, 15 } },
                { "!",      { State::W, 15 } },
                { "&",      { State::W, 15 } },
                { " ",      { State::W, 15 } },
                { "\t",     { State::W, 15 } },
                { "\n",     { State::S, 4  } },
                { "#",      { State::Zs, 15 } },
                { "U",      { State::W, 15 } },
                }},

    { State::A, {
                { "|",      { State::Z, 14 } },
                }},

    { State::B, {
                   { "&",      { State::Z, 14 } },
                   }},
    };

#endif // LEXEMEREADER_H
