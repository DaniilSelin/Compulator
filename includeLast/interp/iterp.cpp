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
        // switch (lit->type()) {
        //     case LiteralType::Operation: {
        //         int opCode = std::get<int>(lit->evaluate());
        //         auto handlerIt = SemanticIterpHandlers.find(opCode);
        //         handlerIt->second(ctx);
        //         break;
        //     }
        //     default: {
        //         ctx.magasine.push_back(ctx.ops[ctx.curLit]);
        //         break;
        //     }
        // }
        ctx.curLit++;
    }
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    Context ctx; // Создаём контекст
    const char* filePath = argv[1];
    
    std::string inputString = loadTextFile(filePath);
    std::vector<Lexeme> prog = lexemeReader(ctx, inputString);

    std::cout << prog;//printLexemes(prog);

	std::vector<std::shared_ptr<Literal>> ops = genOPS(prog);

	std::cout << "\n[INFO] Генерация завершена. Итоговый OPS:\n";
	for (const auto& op : ops) { op->printLex(std::cout); std::cout << " "; }
	std::cout << "\n";
	
	std::cout << "\n[INFO] Программа начинает работу\n";
	interpretation(std::move(ops));

    std::cout << "\n[INFO] Программа закончила работу\n";
    return 0;
}
