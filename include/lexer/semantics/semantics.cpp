#include "semantics.h"
#include "core/Lexeme.hpp"
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

// Мапа с булевыми операци и ключевыми словами
extern const std::unordered_map<std::string,int> KeywordMap;

// Семантические функции
void sem1(Context& ctx) {
    ctx.n = ctx.InputSTR[ctx.i] - '0';
}

void sem2(Context& ctx) {
    ctx.name = std::string(1, ctx.InputSTR[ctx.i]);
}

void sem3(Context& ctx) {
    ctx.prog.push_back({int(ctx.InputSTR[ctx.i]), std::string(1, ctx.InputSTR[ctx.i]), ctx.row, ctx.pos});
}

void sem4(Context& ctx) {
    ctx.row++;
    ctx.pos = 0;
}

void sem5(Context& ctx) {
    ctx.prog.push_back({20, std::string(1, ctx.InputSTR[ctx.i]), ctx.row, ctx.pos});
}

void sem6(Context& ctx) {
    ctx.n = 10 * ctx.n + (ctx.InputSTR[ctx.i] - '0');
}

void sem7(Context& ctx) {
    ctx.prog.push_back({2, std::to_string(ctx.n), ctx.row, ctx.pos});
}

void sem8(Context& ctx) {
    ctx.d = 1;
    ctx.x = ctx.n;
}

void sem9(Context& ctx) {
    ctx.d *= 0.1;
    ctx.x += (ctx.InputSTR[ctx.i] - '0') * ctx.d;
}

void sem10(Context& ctx) {
    ctx.name.push_back(ctx.InputSTR[ctx.i]);
}

void sem11(Context& ctx) {
    auto it = KeywordMap.find(ctx.name);
    int code = (it != KeywordMap.end()) ? it->second : 1;
    ctx.prog.push_back({code, ctx.name, ctx.row, ctx.pos});
}

void sem12(Context& ctx) {
    ctx.prog.push_back({3, std::to_string(ctx.x), ctx.row, ctx.pos});
}

void sem13(Context& ctx) {
    ctx.prog.push_back({int(ctx.name[0]), ctx.name, ctx.row, ctx.pos});
}

void sem14(Context& ctx) {
    std::string op = ctx.name + ctx.InputSTR[ctx.i];
    auto it = KeywordMap.find(op);
    if (it != KeywordMap.end()) {
        ctx.prog.push_back({it->second, op, ctx.row, ctx.pos});
    }
}

void sem15(Context& ctx) {
    // Просто скип
}

const std::unordered_map<int, void(*)(Context&)> SemanticHandlers = {
    {1, sem1}, {2, sem2}, {3, sem3}, {4, sem4}, {5, sem5}, {6, sem6},
    {7, sem7}, {8, sem8}, {9, sem9}, {10, sem10}, {11, sem11}, {12, sem12},
    {13, sem13}, {14, sem14}, {15, sem15}
};