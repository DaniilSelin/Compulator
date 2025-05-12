#include <genOPS/genOPS.h>
#include "core/Lexeme.hpp"
#include "lexer/lexer.h"
#include <iostream>
#include <cstdlib>

std::string loadTextFile(const std::string& filePath);
std::vector<Lexeme> lexemeReader(Context& ctx, const std::string& inputString);

// --- Вспомогательный маппинг строк → GenState ---
GenState stateFromString(const std::string& s) {
    static const std::unordered_map<std::string,GenState> mp = {
        {"A", GenState::A}, {"P", GenState::P}, {"Q", GenState::Q},
        {"B", GenState::B}, {"C", GenState::C}, {"D", GenState::D},
        {"S", GenState::S}, {"U", GenState::U}, {"T", GenState::T},
        {"V", GenState::V}, {"F", GenState::F}, {"G", GenState::G},
        {"H", GenState::H}, {"L", GenState::L}, {"W", GenState::W},
        {"M", GenState::M}, {"X", GenState::X}, {"N", GenState::N},
        {"O", GenState::O}, {"E", GenState::E}, {"Z", GenState::Z},
        {"Ʇ", GenState::Z}  // конец программы можно отнести к Z
    };
    auto it = mp.find(s);
    if (it == mp.end()) {
        throw std::runtime_error("Unknown non-terminal: " + s);
    }
    return it->second;
}

struct GenContext {
    const std::vector<Lexeme>& prog;
    std::deque<std::string> magazine;
    std::deque<std::string> generator;
    size_t currentIdxProg;
};

// --- Основная функция генерации OPS ---

std::vector<std::string> genOPS(const std::vector<Lexeme>& prog) {
    GenContext ctx{ prog,
        /* magazin */   {"A", "Ʇ"},
        /* generator */ {"", ""}, 
        /* idx */       0
    };
    std::vector<std::string> OPS;

    auto tryParseInt = [](const std::string& s, int& out) {
        try {
            size_t pos;
            int v = std::stoi(s, &pos);
            if (pos == s.size()) {
                out = v;
                return true;
            }
        } catch (...) {}
        return false;
    };

    while (!ctx.magazine.empty()) {
        // 1) Взять из генератора символ и добавить в OPS (если не пустой)
        if (!ctx.generator.empty()) {
            std::string sym = ctx.generator.front();
            ctx.generator.pop_front();
            if (!sym.empty()) {
                OPS.push_back(sym);
            }
        }

        // 2) Если в программе закончились лексемы — выходим
        if (ctx.currentIdxProg >= ctx.prog.size()) break;
        const Lexeme& curLex = ctx.prog[ctx.currentIdxProg];

        // 3) Обработать первый символ в магазине
        std::string top = ctx.magazine.front();
        int num;
        if (tryParseInt(top, num) && num == curLex.num) {
            // терминал совпал с номером лексемы
            ctx.magazine.pop_front();
            ctx.currentIdxProg++;
        } else {
            // нетерминал или несовпадение — берём правило
            ctx.magazine.pop_front();
            // преобразуем top → GenState
            GenState st = stateFromString(top);
            auto itState = genTransitionTable.find(st);
            if (itState == genTransitionTable.end())
                throw std::runtime_error("No rules for state " + top);
            auto itRule = itState->second.find(curLex.num);
            if (itRule == itState->second.end())
                throw std::runtime_error("No transition for state " + top +
                                         " on lexeme " + std::to_string(curLex.num));
            const GenerationRules& R = itRule->second;
            // вставляем pattern в начало магазина (в том же порядке)
            for (auto rit = R.pattern.rbegin(); rit != R.pattern.rend(); ++rit) {
                if (!rit->empty())
                    ctx.magazine.push_front(*rit);
            }
            // вставляем action в начало генератора
            for (auto rit = R.action.rbegin(); rit != R.action.rend(); ++rit) {
                if (!rit->empty())
                    ctx.generator.push_front(*rit);
            }
        }
    }

    return OPS;
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

    std::cout << "Parsed " << prog.size() << " lexemes:" << std::endl;
	printLexemes(prog);

	genOPS(prog);    
    return 0;
}