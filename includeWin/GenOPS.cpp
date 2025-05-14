#include "GenOPS.h"
void GenSem1(GenContext& ctx) {
    ctx.MarkVector.push_front(static_cast<int>(ctx.OPS.size()));
    ctx.OPS.push_back({typeLexeme::mark, 0});
    ctx.OPS.push_back({typeLexeme::operation, 1109});
}

void GenSem2(GenContext& ctx) {
    int markPos = ctx.MarkVector.front();
    ctx.OPS[markPos].value = static_cast<int>(ctx.OPS.size()) + 2;
    ctx.MarkVector.pop_front();
    ctx.MarkVector.push_front(static_cast<int>(ctx.OPS.size()));
    ctx.OPS.push_back({typeLexeme::mark, 0});
    ctx.OPS.push_back({typeLexeme::operation, 1110});
}

void GenSem3(GenContext& ctx) {
    int markPos = ctx.MarkVector.front();
    ctx.OPS[markPos].value = static_cast<int>(ctx.OPS.size());
    ctx.MarkVector.pop_front();
}

void GenSem4(GenContext& ctx) {
    ctx.MarkVector.push_front(static_cast<int>(ctx.OPS.size()));
}

void GenSem5(GenContext& ctx) {
    int markPos = ctx.MarkVector.front();
    ctx.OPS[markPos].value = static_cast<int>(ctx.OPS.size()) + 2;
    ctx.MarkVector.pop_front();
    int prevMark = ctx.MarkVector.front();
    ctx.OPS.push_back({typeLexeme::mark, prevMark});
    ctx.MarkVector.pop_front();
    ctx.OPS.push_back({typeLexeme::operation, 1110});
}

void GenSem6(GenContext& ctx) {
    Lexeme lex = ctx.prog.at(ctx.curLex);
    std::string name = lex.value;
    if (ctx.VarMap.find(name) != ctx.VarMap.end()) {
        std::cout << "[ERROR] Переопределение " + name +
                         " - Позиция[" + std::to_string(lex.row) + ";" + std::to_string(lex.pos) + "]\n";
        std::exit(1);
    }
    else ctx.VarMap[name] = VarObject{0, ctx.InitReal};
    ctx.MarkVector.push_front(static_cast<int>(ctx.OPS.size()));
    ctx.OPS.push_back({typeLexeme::link, name});
}

void GenSem7(GenContext& ctx) {
    int markPos = ctx.MarkVector.front();
    std::string name = std::any_cast<std::string>(ctx.OPS[markPos].value);
    if (!ctx.InitReal) {
        int* ptr = new int;
        ctx.VarMap[name].IsReal = false;
        ctx.VarMap[name].addr = reinterpret_cast<std::uintptr_t>(ptr);
    } else {
        double* ptr = new double;
        ctx.VarMap[name].IsReal = true;
        ctx.VarMap[name].addr = reinterpret_cast<std::uintptr_t>(ptr);
    }
}

void GenSem8(GenContext& ctx) {
    int markPos = ctx.MarkVector.front();
    std::string name = std::any_cast<std::string>(ctx.OPS[markPos].value);
    int size = std::any_cast<int>(ctx.OPS[markPos + 1].value);
    if (!ctx.InitReal) {
        int* ptr = new int[size];
        ctx.VarMap[name].IsReal = false;
        ctx.VarMap[name].addr = reinterpret_cast<std::uintptr_t>(ptr);
    } else {
        double* ptr = new double[size];
        ctx.VarMap[name].IsReal = true;
        ctx.VarMap[name].addr = reinterpret_cast<std::uintptr_t>(ptr);
    }
    ctx.OPS.push_back({typeLexeme::mark, markPos});
    ctx.OPS.push_back({typeLexeme::operation, 1111});
}

void GenSem9(GenContext& ctx) {
    int markPos = ctx.MarkVector.front();
    ctx.MarkVector.pop_front();
    std::string name = std::any_cast<std::string>(ctx.OPS[markPos].value);
    std::cout << ctx.VarMap[name].addr << std::endl;
    if (ctx.VarMap[name].addr == 0) {
        int distance = static_cast<int>(ctx.OPS.size()) - markPos;
        if (distance == 1) {
            if (!ctx.InitReal) {
                int* ptr = new int;
                ctx.VarMap[name].IsReal = false;
                ctx.VarMap[name].addr = reinterpret_cast<std::uintptr_t>(ptr);
            } else {
                double* ptr = new double;
                ctx.VarMap[name].IsReal = true;
                ctx.VarMap[name].addr = reinterpret_cast<std::uintptr_t>(ptr);
            }
            ctx.OPS.pop_back();
        } else {
            int size = std::any_cast<int>(ctx.OPS[markPos + 1].value);
            if (!ctx.InitReal) {
                int* ptr = new int[size];
                ctx.VarMap[name].IsReal = false;
                ctx.VarMap[name].addr = reinterpret_cast<std::uintptr_t>(ptr);
            } else {
                double* ptr = new double[size];
                ctx.VarMap[name].IsReal = true;
                ctx.VarMap[name].addr = reinterpret_cast<std::uintptr_t>(ptr);
            }
            ctx.OPS.pop_back();
            ctx.OPS.pop_back();
        }
    }
}

void GenSem10(GenContext& ctx) {
    ctx.InitReal = true;
}

void GenSem11(GenContext& ctx) {
    ctx.InitReal = false;
}

std::vector<Literal> genOPS(std::vector<Lexeme>& prog) {
    GenContext ctx(prog);

    std::deque<std::string> magazine = {"A", "#"};
    std::deque<int> generator = {0, 0};

    std::cout << "[INFO] Начало генерации OPS";
    std::cout << "\n[INFO] Начальное состояние магазина: ";
    for (const auto& s : magazine) std::cout << s << " ";
    std::cout << "\n[INFO] Начальное состояние генератора: ";
    for (const auto& g : generator) std::cout << g << " ";
    std::cout << "\n[INFO] Доступно переходов из A: " << genTransitionTable.at(GenState::A).size() << "\n";

    // пока в «магазине» что-то есть
    while (magazine.size() > 1) { // != 1
        std::cout << "----------------------------------\n";
        std::cout << "[STEP] Текущий индекс лексемы: " << ctx.curLex;
        if (ctx.curLex < prog.size())
            std::cout << ", лексема: " << ctx.prog[ctx.curLex].value
                      << " (номер=" << ctx.prog[ctx.curLex].num << ")\n";

        // 1) достаём из генератора
        int code = generator.front();
        generator.pop_front();
        std::cout << "[GEN] Взяли из генератора: " << code << "\n";
        if (code != 0) {
            auto semFunc = SemanticGenHandlers.find(code);
            if (semFunc != SemanticGenHandlers.end()){
                semFunc->second(ctx);
                std::cout << "[GEN] Запустили семамнтическую программу: " << code << "\n";
            } else {
                switch (code) {
                    case 1: {
                        auto name = ctx.VarMap.find(ctx.prog.at(ctx.curLex).value);
                        if (name == ctx.VarMap.end())  {
                            std::cout << "[ERROR] Используется неинициализированная переменная: " + ctx.prog.at(ctx.curLex).value
                                             + " - Позиция [" + std::to_string(ctx.prog.at(ctx.curLex).row) + ";" + std::to_string(ctx.prog.at(ctx.curLex).pos) + "]\n";
                            std::exit(1);
                        }
                        std::cout << "[GEN] Добавили в OPS: " << ctx.prog.at(ctx.curLex).value << "\n";
                        ctx.OPS.push_back({typeLexeme::link, ctx.prog.at(ctx.curLex).value});
                        break;
                    }
                    case 2: {
                        int val = std::stoi(ctx.prog.at(ctx.curLex).value);
                        std::cout << "[GEN] Добавили в OPS: " << val << "\n";
                        ctx.OPS.push_back({typeLexeme::constant, val});
                        break;
                    }
                    case 3: {
                        double val = std::stod(ctx.prog.at(ctx.curLex).value);
                        std::cout << "[GEN] Добавили в OPS: " << val << "\n";
                        ctx.OPS.push_back({typeLexeme::constant, val});
                        break;
                    }
                    default: {
                        std::cout << "[GEN] Добавили в OPS: " << code << "\n";
                        ctx.OPS.push_back({typeLexeme::operation, code});
                        break;
                    }
                }
            }
        }

        // 2) достаём с вершины магазина
        std::string sym = magazine.front();
        magazine.pop_front();
        std::cout << "[MAG] Взяли с вершины магазина: " << sym << "\n";

        auto elMag = getState.find(sym);
        if (elMag == getState.end()) { // Терминал
            int num = std::stoi(sym);
            std::cout << "[CHK] Ожидаем терминал: " << num << "\n";
            if (ctx.prog[ctx.curLex].num == num) {
                std::cout << "[CHK] Совпало. Переходим к следующей лексеме.\n";
                ctx.curLex++;
            } else {
                std::cout << "[ERROR] Не совпал терминал " << sym
                          << " Лексема: " << ctx.curLex
                          << " Позиция: [" << ctx.prog.at(ctx.curLex).row << ";" << ctx.prog.at(ctx.curLex).pos << "]\n";
                std::exit(1);
            }
        } else { // Нетерминал
            std::cout << "[NT] Нетерминал " << sym << " => состояние " << static_cast<int>(elMag->second) << "\n";
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
        for (const auto& op : ctx.OPS) std::cout << op;
        std::cout << "\n";
    }

    std::cout << "\n[INFO] Генерация завершена. Итоговый OPS:\n";
    for (const auto& op : ctx.OPS) std::cout << op;
    std::cout << "\n";

    return ctx.OPS;
}
