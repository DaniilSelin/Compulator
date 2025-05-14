#include "LexemeReader.h"
// Семантические функции
void LexSem1(Context& ctx) {
    ctx.n = ctx.InputSTR[ctx.i] - '0';
}

void LexSem2(Context& ctx) {
    ctx.name = std::string(1, ctx.InputSTR[ctx.i]);
}

void LexSem3(Context& ctx) {
    ctx.prog.push_back(
        {
            static_cast<int>(ctx.InputSTR[ctx.i]),
            std::string(1, ctx.InputSTR[ctx.i]),
            ctx.row,
            ctx.pos
        }
        );
}

void LexSem4(Context& ctx) {
    ctx.row++;
    ctx.pos = 0;
}

void LexSem5(Context& ctx) {
    ctx.prog.push_back(
        {
            22,
            std::string(1, ctx.InputSTR[ctx.i]),
            ctx.row,
            ctx.pos
        }
        );
}

void LexSem6(Context& ctx) {
    ctx.n = 10 * ctx.n + (ctx.InputSTR[ctx.i] - '0');
}

void LexSem7(Context& ctx) {
    ctx.prog.push_back(
        {
            2,
            std::to_string(ctx.n),
            ctx.row,
            ctx.pos
        }
        );
}

void LexSem8(Context& ctx) {
    ctx.d = 1;
    ctx.x = ctx.n;
}

void LexSem9(Context& ctx) {
    ctx.d *= 0.1;
    ctx.x += (ctx.InputSTR[ctx.i] - '0') * ctx.d;
}

void LexSem10(Context& ctx) {
    ctx.name.push_back(ctx.InputSTR[ctx.i]);
}

void LexSem11(Context& ctx) {
    auto it = KeywordMap.find(ctx.name);
    int code = (it != KeywordMap.end()) ? it->second : 1;
    ctx.prog.push_back(
        {
            code,
            ctx.name,
            ctx.row,
            ctx.pos
        }
        );
}

void LexSem12(Context& ctx) {
    ctx.prog.push_back(
        {
            3,
            std::to_string(ctx.x),
            ctx.row,
            ctx.pos
        }
        );
}

void LexSem13(Context& ctx) {
    ctx.prog.push_back(
        {
            static_cast<int>(ctx.name[0]),
            ctx.name,
            ctx.row,
            ctx.pos
        }
        );
}

void LexSem14(Context& ctx) {
    std::string op = ctx.name + ctx.InputSTR[ctx.i];
    auto it = KeywordMap.find(op);
    if (it != KeywordMap.end()) {
        ctx.prog.push_back(
            {
                it->second,
                op,
                ctx.row,
                ctx.pos
            }
            );
    }
}

void LexSem15(Context& ctx) {} // Пропуск

std::vector<Lexeme> lexemeReader(Context& ctx, const std::string& inputString) {
    ctx.InputSTR = inputString;
    State state = State::S;
    for (; ctx.i < inputString.size(); ctx.i++, ctx.pos++) {
        std::string term(1, ctx.InputSTR[ctx.i]);
        if (isalpha(term[0])) term = "L";
        if (isdigit(term[0])) term = "D";
        if (state == State::W || state == State::Q || state == State::M)
            if (term != "*" && term != "/" && term != "\n") term = "U";
        InnerMap transfers = kTransitionTable.at(state);
        auto transfer = transfers.find(term);
        if (transfer == transfers.end()) break;
        state = transfer->second.nextState;
        SemanticHandlers.at(transfer->second.action)(ctx);
        if (state == State::Zs) { state = State::S; ctx.pos--; ctx.i--; }
        if (state == State::Z) state = State::S;
    }
    return ctx.prog;
}
