#include "GenOPS.h"
#include "core/Literal.hpp"
#include <vector>

std::unordered_map<std::string, VarObject> VarMap;

void GenSem1(GenContext& ctx) {
    ctx.MarkVector.push_front(static_cast<int>(ctx.OPS.size()));
    ctx.OPS.push_back(std::make_shared<Mark>(0));
    ctx.OPS.push_back(std::make_shared<Operation>(1109));
}

void GenSem2(GenContext& ctx) {
    int markPos = ctx.MarkVector.front();
    ctx.OPS[markPos]->set(static_cast<int>(ctx.OPS.size()) + 2);
    ctx.MarkVector.pop_front();
    ctx.MarkVector.push_front(static_cast<int>(ctx.OPS.size()));
    ctx.OPS.push_back(std::make_shared<Mark>(0));
    ctx.OPS.push_back(std::make_shared<Operation>(1110));
}

void GenSem3(GenContext& ctx) {
    int markPos = ctx.MarkVector.front();
    ctx.OPS[markPos]->set(static_cast<int>(ctx.OPS.size()));
    ctx.MarkVector.pop_front();
}

void GenSem4(GenContext& ctx) {
    ctx.MarkVector.push_front(static_cast<int>(ctx.OPS.size()));
}

void GenSem5(GenContext& ctx) {
    int markPos = ctx.MarkVector.front();
    ctx.OPS[markPos]->set(static_cast<int>(ctx.OPS.size()) + 2);
    ctx.MarkVector.pop_front();
    int prevMark = ctx.MarkVector.front();
    ctx.OPS.push_back(std::make_shared<Mark>(prevMark));
    ctx.MarkVector.pop_front();
    ctx.OPS.push_back(std::make_shared<Operation>(1110));
}

void GenSem6(GenContext& ctx) {
    Lexeme lex = ctx.prog.at(ctx.curLex);
    std::string name = lex.value;
    if (VarMap.find(name) != VarMap.end()) {
        std::cout << "[ERROR] Переопределение " + name +
                         " - Позиция[" + std::to_string(lex.row) + ";" + std::to_string(lex.pos) + "]\n";
        std::exit(1);
    }
    else VarMap[name] = VarObject{0, 0, ctx.InitReal};
    ctx.MarkVector.push_front(static_cast<int>(ctx.OPS.size()));
    if (ctx.InitReal) ctx.OPS.push_back(std::make_shared<LinkReal>(name));
    else ctx.OPS.push_back(std::make_shared<LinkInt>(name));
}

void GenSem7(GenContext& ctx) {
    int markPos = ctx.MarkVector.front();
    std::string name = std::any_cast<std::string>(ctx.OPS[markPos]->get());
    VarMap[name].size = 1;
    if (!ctx.InitReal) {
        int* ptr = new int;
        VarMap[name].isReal = false;
        VarMap[name].addr = reinterpret_cast<std::uintptr_t>(ptr);
    } else {
        double* ptr = new double;
        VarMap[name].isReal = true;
        VarMap[name].addr = reinterpret_cast<std::uintptr_t>(ptr);
    }
}

void GenSem8(GenContext& ctx) {
    int markPos = ctx.MarkVector.front();
    std::string name = std::any_cast<std::string>(ctx.OPS[markPos]->get());
    int size = std::any_cast<int>(ctx.OPS[markPos + 1]->get());
    VarMap[name].size = size;
    if (!ctx.InitReal) {
        int* ptr = new int[size];
        VarMap[name].isReal = false;
        VarMap[name].addr = reinterpret_cast<std::uintptr_t>(ptr);
    } else {
        double* ptr = new double[size];
        VarMap[name].isReal = true;
        VarMap[name].addr = reinterpret_cast<std::uintptr_t>(ptr);
    }
    ctx.OPS.push_back(std::make_shared<Operation>(1111));
}

void GenSem9(GenContext& ctx) {
    int markPos = ctx.MarkVector.front();
    ctx.MarkVector.pop_front();
    std::string name = std::any_cast<std::string>(ctx.OPS[markPos]->get());
    if (VarMap[name].addr == 0) {
        int distance = static_cast<int>(ctx.OPS.size()) - markPos;
        if (distance == 1) {
            VarMap[name].size = 1;
            if (!ctx.InitReal) {
                int* ptr = new int;
                VarMap[name].isReal = false;
                VarMap[name].addr = reinterpret_cast<std::uintptr_t>(ptr);
            } else {
                double* ptr = new double;
                VarMap[name].isReal = true;
                VarMap[name].addr = reinterpret_cast<std::uintptr_t>(ptr);
            }
            ctx.OPS.pop_back();
        } else {
            int size = std::any_cast<int>(ctx.OPS[markPos + 1]->get());
            VarMap[name].size = size;
            if (!ctx.InitReal) {
                int* ptr = new int[size];
                VarMap[name].isReal = false;
                VarMap[name].addr = reinterpret_cast<std::uintptr_t>(ptr);
            } else {
                double* ptr = new double[size];
                VarMap[name].isReal = true;
                VarMap[name].addr = reinterpret_cast<std::uintptr_t>(ptr);
            }
            ctx.OPS.pop_back();
            ctx.OPS.pop_back();
        }
    }
}

void GenSem10(GenContext& ctx) {
    ctx.InitReal = false;
}

void GenSem11(GenContext& ctx) {
    ctx.InitReal = true;
}

std::vector<std::shared_ptr<Literal>> genOPS(std::vector<Lexeme>& prog) {
    GenContext ctx(prog);

    std::deque<std::string> magazine = {"A", "#"};
    std::deque<int> generator = {0, 0};

    // пока в «магазине» что-то есть
    while (magazine.size() > 1) {
        // 1) достаём из генератора
        int code = generator.front();
        generator.pop_front();
        if (code != 0) {
            auto semFunc = SemanticGenHandlers.find(code);
            if (semFunc != SemanticGenHandlers.end()) semFunc->second(ctx);
            else {
                switch (code) {
                    case 1: {
                        auto var = VarMap.find(ctx.prog.at(ctx.curLex).value);
                        if (var == VarMap.end())  {
                            std::cout << "[ERROR] Используется неинициализированная переменная: " + ctx.prog.at(ctx.curLex).value
                                             + " - Позиция [" + std::to_string(ctx.prog.at(ctx.curLex).row) + ";" + std::to_string(ctx.prog.at(ctx.curLex).pos) + "]\n";
                            std::exit(1);
                        }
                        if (var->second.isReal) ctx.OPS.push_back(std::make_shared<LinkReal>(ctx.prog.at(ctx.curLex).value));
                        else ctx.OPS.push_back(std::make_shared<LinkInt>(ctx.prog.at(ctx.curLex).value));
                        break;
                    }
                    case 2: {
                        int val = std::stoi(ctx.prog.at(ctx.curLex).value);
                        ctx.OPS.push_back(std::make_shared<ConstantInt>(val));
                        break;
                    }
                    case 3: {
                        double val = std::stod(ctx.prog.at(ctx.curLex).value);
                        ctx.OPS.push_back(std::make_shared<ConstantReal>(val));
                        break;
                    }
                    default: {
                        ctx.OPS.push_back(std::make_shared<Operation>(code));
                        break;
                    }
                }
            }
        }
        std::string sym = magazine.front();
        magazine.pop_front();
        auto elMag = getState.find(sym);
        if (elMag == getState.end()) { // Терминал
            int num = std::stoi(sym);
            if (ctx.prog[ctx.curLex].num == num) ctx.curLex++;
            else {
                std::cout << "[ERROR] Не совпал терминал " << sym
                          << " Лексема: " << ctx.curLex
                          << " Позиция: [" << ctx.prog.at(ctx.curLex).row << ";" << ctx.prog.at(ctx.curLex).pos << "]\n";
                std::exit(1);
            }
        } else {
            auto itState = genTransitionTable.find(elMag->second);
            if (itState == genTransitionTable.end()) {
                std::cout << "[ERROR] Нет правил для состояния " << sym << "\n";
                std::exit(1);
            }
            auto itRule = itState->second.find(ctx.prog[ctx.curLex].num);
            if (itRule == itState->second.end()) {
                std::cout << "[ERROR] Нет перехода из состояния " << sym
                          << " Лексема: " << ctx.prog[ctx.curLex].num
                          << " Позиция: [" << ctx.prog.at(ctx.curLex).row << ";" << ctx.prog.at(ctx.curLex).pos << "]\n";
                std::exit(1);
            }
            const GenRules rule = itRule->second;
            magazine.insert(magazine.begin(), rule.pattern.begin(), rule.pattern.end());
            generator.insert(generator.begin(), rule.semGen.begin(), rule.semGen.end());
        }
    }
    return std::move(ctx.OPS);
}
