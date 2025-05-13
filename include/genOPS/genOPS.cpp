#include <genOPS/genOPS.h>
#include <genOPS/semantics.h>
#include "core/Lexeme.hpp"
#include "lexer/lexer.h"
#include <iostream>
#include <cstdlib>
#include <deque>
#include <algorithm>      // для std::all_of
#include <cctype>         // для ::isdigit
#include <stdexcept>      // для std::runtime_error

using GenInnerMap = std::unordered_map<int, GenerationRules>;

std::string loadTextFile(const std::string& filePath) {
    std::ifstream in(filePath);
    if (!in) {
        std::cout << "Не открывается файл: " << filePath << std::endl;
        return std::string();
    }
    std::string output((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    output += "#";
    return output;
}

std::vector<Lexeme> lexemeReader(Context& ctx, const std::string& inputString);

static const std::unordered_map<std::string,GenState> getState = {
    {"A", GenState::A}, {"P", GenState::P}, {"Q", GenState::Q},
    {"B", GenState::B}, {"C", GenState::C}, {"D", GenState::D},
    {"S", GenState::S}, {"U", GenState::U}, {"T", GenState::T},
    {"V", GenState::V}, {"F", GenState::F}, {"G", GenState::G},
    {"H", GenState::H}, {"L", GenState::L}, {"W", GenState::W},
    {"M", GenState::M}, {"X", GenState::X}, {"N", GenState::N},
    {"O", GenState::O}, {"E", GenState::E}, {"Z", GenState::Z},
    {"#", GenState::Z} 
};

auto printLiteral = [](const literal& lit) {
    switch (lit.type) {
        case typeL::link:
            std::cout << "link(" << std::any_cast<std::string>(lit.value) << ") ";
            break;
        case typeL::constant:
            if (lit.value.type() == typeid(int)) {
                std::cout << "const(" << std::any_cast<int>(lit.value) << ") ";
            } else {
                std::cout << "const(" << std::any_cast<double>(lit.value) << ") ";
            } 
            break;
        case typeL::mark:
            std::cout << "mark(" << std::any_cast<int>(lit.value) << ") ";
            break;
        case typeL::operation:
            std::cout << "op(" << std::any_cast<int>(lit.value) << ") ";
            break;
    }
};

// --- Основная функция генерации OPS ---

std::vector<literal> genOPS(std::vector<Lexeme>& prog) {
    GenContext ctx(prog);
    ctx.curLex    = 0;
    ctx.InitReal  = false;
    ctx.OPS.clear();
    ctx.MarkVector.clear();
    ctx.VarMap.clear();

    std::deque<std::string> magazine = {"A", "#"};
    std::deque<int> generator = {0, 0};

    std::cout << "\n[INFO] Начало генерации OPS\n";
    std::cout << "[INFO] Начальное состояние магазина: ";
    for (const auto& s : magazine) std::cout << s << " ";
    std::cout << "\n[INFO] Начальное состояние генератора: ";
    for (const auto& g : generator) std::cout << g << " ";
    std::cout << "\n[INFO] Доступно переходов из A: " << genTransitionTable.at(GenState::A).size() << "\n\n";

    // пока в «магазине» что-то есть
    while (magazine.size() != 1) {
        std::cout << "\n----------------------------------\n";
        std::cout << "[STEP] Текущий индекс лексемы: " << ctx.curLex;
        if (ctx.curLex < prog.size())
            std::cout << ", лексема: " << ctx.prog[ctx.curLex].value
                      << " (num=" << ctx.prog[ctx.curLex].num << ")";
        std::cout << "\n";

        // 1) достаём из генератора
        int code = generator.front();
        generator.pop_front(); 
        std::cout << "[GEN] Взяли из генератора: " << code << "\n";
        if (code != 0) {
            if (auto semFunc = SemanticGenHandlers.find(code); semFunc != SemanticGenHandlers.end()){
                semFunc->second(ctx);
                std::cout << "[GEN] Запустили семамнтическую программу: " << code << "\n";
            } else {
                switch (code) {
                    case 1:
                        if (auto name = ctx.VarMap.find(ctx.prog.at(ctx.curLex).value); name == ctx.VarMap.end()) {
                            throw std::runtime_error("Unutiliaze variable used: " + ctx.prog.at(ctx.curLex).value);
                        }
                        std::cout << "[GEN] Добавили в OPS: " << ctx.prog.at(ctx.curLex).value << "\n";
                        ctx.OPS.push_back({typeL::link, ctx.prog.at(ctx.curLex).value});
                        break;
                    case 2: {
                        int val = std::stoi(ctx.prog.at(ctx.curLex).value);
                        std::cout << "[GEN] Добавили в OPS: " << val << "\n";
                        ctx.OPS.push_back({typeL::constant, val});
                        break;
                    }
                    case 3: {
                        double val = std::stod(ctx.prog.at(ctx.curLex).value);
                        std::cout << "[GEN] Добавили в OPS: " << val << "\n";
                        ctx.OPS.push_back({typeL::constant, val});
                        break;
                    }
                    default:
                        std::cout << "[GEN] Добавили в OPS: " << code << "\n";
                        ctx.OPS.push_back({typeL::operation, code});
                        break;
                }
            }
        }
        // int code = generator.pop_front(); if (code) OPS.push_back(code);

        // 2) достаём с вершины магазина
        std::string sym = magazine.front();
        magazine.pop_front(); 
        std::cout << "[MAG] Взяли с вершины магазина: " << sym << "\n";

        auto elMag = getState.find(sym);
        if (elMag == getState.end()) {
            // терминал
            int num = std::stoi(sym);
            std::cout << "[CHK] Ожидаем терминал: " << num << "\n";
            if (ctx.prog[ctx.curLex].num == num) {
                std::cout << "[CHK] Совпало. Переходим к следующей лексеме.\n";
                ctx.curLex++;
            } else {
                std::cerr << "[ERROR] Не совпал терминал " << sym
                          << " на позиции " << ctx.curLex << "\n";
                throw std::runtime_error("Mismatch terminal " + sym +
                                         " at lexeme #" + std::to_string(ctx.curLex));
            }
        } else { 
            // нетерминал
            std::cout << "[NT] Нетерминал " << sym << " => состояние " << static_cast<int>(elMag->second) << "\n";
            auto itState = genTransitionTable.find(elMag->second);
            if (itState == genTransitionTable.end()) {
                std::cerr << "[ERROR] Нет правил для состояния " << sym << "\n";
                throw std::runtime_error("No rules for state " + sym);
            }
            auto itRule = itState->second.find(ctx.prog[ctx.curLex].num);
            if (itRule == itState->second.end()) {
                std::cerr << "[ERROR] Нет перехода из состояния " << sym
                          << " по лексеме " << ctx.prog[ctx.curLex].num << "\n";
                throw std::runtime_error("No transition for state " + sym +
                                         " on lexeme " + std::to_string(ctx.prog[ctx.curLex].num));
            }
            const GenerationRules& rule = itRule->second;
            std::cout << "[RULE] Применяем правило:\n";
            std::cout << "       pattern: ";
            for (const auto& s : rule.pattern) std::cout << s << " ";
            std::cout << "\n       semGen: ";
            for (const auto& a : rule.semGen) std::cout << a << " ";
            std::cout << "\n";

            magazine.insert(magazine.begin(), rule.pattern.begin(), rule.pattern.end());
            generator.insert(generator.begin(), rule.semGen.begin(), rule.semGen.end());
        }

        // вывод текущего состояния
        std::cout << "[MAG] Состояние магазина: ";
        for (const auto& s : magazine) std::cout << s << " ";
        std::cout << "\n[GEN] Состояние генератора: ";
        for (const auto& g : generator) std::cout << g << " ";
        std::cout << "\n[OPS] Текущий OPS: ";
        for (const auto& op : ctx.OPS) 
            printLiteral(op);
        std::cout << "\n";
    }

    std::cout << "\n[INFO] Генерация завершена. Итоговый OPS:\n";
    for (const auto& op : ctx.OPS) 
        printLiteral(op);
    std::cout << "\n";

    return ctx.OPS;
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