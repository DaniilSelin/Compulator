#include "genOPS/GenOPS.h"
#include "lexer/lexer.h"
#include "interp/AddFunc.h"
#include "interp/iterp.h"
#include "core/Literal.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path> [-v]" << std::endl;
        return 1;
    }

    Context ctx;
    const char* filePath = argv[1];

    // Флаг для включения verbose-режима
    bool verbose = false;
    if (argc >= 3 && std::string(argv[2]) == "-v") {
        verbose = true;
    }

    std::string inputString = loadTextFile(filePath);
    std::vector<Lexeme> prog = lexemeReader(ctx, inputString);

    if (verbose) {
        std::cout << "Parsed " << prog.size() << " lexemes:" << std::endl;
        std::cout << prog;
    }

    std::vector<std::shared_ptr<Literal>> ops = genOPS(prog);

    if (verbose) {
        std::cout << "\n[INFO] Генерация завершена. Итоговый OPS:\n";
        for (const auto& op : ops) {
            op->printLex(std::cout);
            std::cout << " ";
        }
        std::cout << "\n";
    }

    interpretation(std::move(ops));
    return 0;
}
