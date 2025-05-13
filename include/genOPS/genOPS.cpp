#include <genOPS/genOPS.h>
#include "core/Lexeme.hpp"
#include "lexer/lexer.h"
#include <iostream>
#include <cstdlib>
#include <deque>
#include <algorithm>      // для std::all_of
#include <cctype>         // для ::isdigit
#include <stdexcept>      // для std::runtime_error

std::string loadTextFile(const std::string& filePath) {
    std::ifstream in(filePath);
    if (!in) {
        std::cout << "Не открывается файл: " << filePath << std::endl;
        return std::string();
    }
    std::string output((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    output += static_cast<char>(0xA7B1); // 0xE1 = Ʇ
    return output;
}

std::vector<Lexeme> lexemeReader(Context& ctx, const std::string& inputString);

GenState stateFromString(const std::string& s) {
    static const std::unordered_map<std::string,GenState> mp = {
        {"A", GenState::A}, {"P", GenState::P}, {"Q", GenState::Q},
        {"B", GenState::B}, {"C", GenState::C}, {"D", GenState::D},
        {"S", GenState::S}, {"U", GenState::U}, {"T", GenState::T},
        {"V", GenState::V}, {"F", GenState::F}, {"G", GenState::G},
        {"H", GenState::H}, {"L", GenState::L}, {"W", GenState::W},
        {"M", GenState::M}, {"X", GenState::X}, {"N", GenState::N},
        {"O", GenState::O}, {"E", GenState::E}, {"Z", GenState::Z},
        {"Ʇ", GenState::Z} 
    };
    auto it = mp.find(s);
    if (it == mp.end()) {
        throw std::runtime_error("Unknown non-terminal: " + s);
    }
    return it->second;
}

bool isNumber(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

struct literal {
    typeLiteral type;
    int value;
};

// --- Основная функция генерации OPS ---

std::vector<int> genOPS(const std::vector<Lexeme>& prog) {
    std::deque<std::string> magazine;
    std::deque<int>         generator;
    std::vector<int>        OPS;
    int                     curLex = 0;

    // инициализация
    magazine.push_back("A");
    magazine.push_back("Ʇ");
    generator.push_back(0);
    generator.push_back(0);

    std::cout << "\n[INFO] Начало генерации OPS\n";
    std::cout << "[INFO] Начальное состояние магазина: ";
    for (const auto& s : magazine) std::cout << s << " ";
    std::cout << "\n[INFO] Начальное состояние генератора: ";
    for (const auto& g : generator) std::cout << g << " ";
    std::cout << "\n[INFO] Доступно переходов из A: " << genTransitionTable.at(GenState::A).size() << "\n\n";
    
    // пока в «магазине» что-то есть
    while (!magazine.empty()) {
        std::cout << "\n----------------------------------\n";
        std::cout << "[STEP] Текущий индекс лексемы: " << curLex;
        if (curLex < prog.size())
            std::cout << ", лексема: " << prog[curLex] << " (num=" << prog[curLex].num << ")";
        std::cout << "\n";

        // 1) достаём из генератора
        if (!generator.empty()) {
            int code = generator.front();
            generator.pop_front();
            std::cout << "[GEN] Взяли из генератора: " << code << "\n";
            if (code != 0) {
                OPS.push_back(code);
                std::cout << "[GEN] Добавили в OPS: " << code << "\n";
            }
        }

        if (magazine.empty()) break;

        // 2) достаём с вершины магазина
        std::string sym = magazine.front();
        magazine.pop_front();
        std::cout << "[MAG] Взяли с вершины магазина: " << sym << "\n";

        if (isNumber(sym)) {
            // терминал
            int num = std::stoi(sym);
            std::cout << "[CHK] Ожидаем терминал: " << num << "\n";
            if (curLex < prog.size() && prog[curLex].num == num) {
                std::cout << "[CHK] Совпало. Переходим к следующей лексеме.\n";
                ++curLex;
            } else {
                std::cerr << "[ERROR] Не совпал терминал " << sym
                          << " на позиции " << curLex << "\n";
                throw std::runtime_error("Mismatch terminal " + sym +
                                         " at lexeme #" + std::to_string(curLex));
            }
        } else {
            // нетерминал
            GenState st = stateFromString(sym);
            std::cout << "[NT] Нетерминал " << sym << " => состояние " << static_cast<int>(st) << "\n";

            auto itState = genTransitionTable.find(st);
            if (itState == genTransitionTable.end()) {
                std::cerr << "[ERROR] Нет правил для состояния " << sym << "\n";
                throw std::runtime_error("No rules for state " + sym);
            }

            int lexNum = (curLex < prog.size()) ? prog[curLex].num : -1;
            auto itRule = itState->second.find(lexNum);
            if (itRule == itState->second.end()) {
                std::cerr << "[ERROR] Нет перехода из состояния " << sym
                          << " по лексеме " << lexNum << "\n";
                throw std::runtime_error("No transition for state " + sym +
                                         " on lexeme " + std::to_string(lexNum));
            }

            const GenerationRules& rule = itRule->second;
            std::cout << "[RULE] Применяем правило:\n";
            std::cout << "       pattern: ";
            for (const auto& s : rule.pattern) std::cout << s << " ";
            std::cout << "\n       action: ";
            for (const auto& a : rule.action) std::cout << a << " ";
            std::cout << "\n";

            // 2.1) в магазин
            for (auto it = rule.pattern.rbegin(); it != rule.pattern.rend(); ++it) {
                if (!it->empty()) {
                    magazine.push_front(*it);
                    std::cout << "[MAG] -> " << *it << "\n";
                }
            }

            // 2.2) в генератор
            for (auto it = rule.action.rbegin(); it != rule.action.rend(); ++it) {
                generator.push_front(*it);
                std::cout << "[GEN] -> " << *it << "\n";
            }
        }

        // вывод текущего состояния
        std::cout << "[MAG] Состояние магазина: ";
        for (const auto& s : magazine) std::cout << s << " ";
        std::cout << "\n[GEN] Состояние генератора: ";
        for (const auto& g : generator) std::cout << g << " ";
        std::cout << "\n[OPS] Текущий OPS: ";
        for (const auto& op : OPS) std::cout << op << " ";
        std::cout << "\n";
    }

    std::cout << "\n[INFO] Генерация завершена. Итоговый OPS:\n";
    for (const auto& op : OPS) std::cout << op << " ";
    std::cout << "\n";

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