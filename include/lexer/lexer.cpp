#include "semantics.h"
#include "locale.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <sstream>

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