#include "genOPS/GenOPS.h"
#include "lexer/lexer.h"
#include "interp/AddFunc.h"
#include "interp/iterp.h"
#include "core/Literal.hpp"

void interpretation(std::vector<std::shared_ptr<Literal>>&& ops) {
    IterpContext ctx(std::move(ops));
    while (ctx.curLit < ctx.ops.size()) {
        auto& lit = ctx.ops[ctx.curLit];
        if(lit->type() == LiteralType::Operation) {
            int opCode = std::get<int>(lit->evaluate());
            auto handlerIt = SemanticIterpHandlers.find(opCode);
            handlerIt->second(ctx);
        }
        else ctx.magasine.push_back(ctx.ops[ctx.curLit]);
        ctx.curLit++;
    }
}