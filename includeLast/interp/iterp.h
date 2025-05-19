#include "core/Literal.hpp"
#include <vector>
#include <cmath>

struct IterpContext {
	std::deque<std::shared_ptr<Literal>> magasine;
	std::vector<std::shared_ptr<Literal>> ops;
    int curLit = 0;
	// Конструктор
    IterpContext(std::vector<std::shared_ptr<Literal>> OPS): ops(std::move(OPS)) {}
};

// =
void InterpSem61(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    op1->set(op2->evaluate());
}

// +
void InterpSem43(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op2->add(*op1);
    ctx.magasine.push_back(std::move(result));
}

// *
void InterpSem42(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op2->mul(*op1);
    ctx.magasine.push_back(std::move(result));
}

// -
void InterpSem45(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->sub(*op2);
    ctx.magasine.push_back(std::move(result));
}

// /
void InterpSem47(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->div(*op2);
    ctx.magasine.push_back(std::move(result));
}

// ||
void InterpSem8(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->logical_or(*op2);
    ctx.magasine.push_back(std::move(result));
}

// &&
void InterpSem9(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->logical_and(*op2);
    ctx.magasine.push_back(std::move(result));
}

// !
void InterpSem33(IterpContext& ctx) {
    auto op = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op->logical_not();
    ctx.magasine.push_back(std::move(result));
}

// <
void InterpSem60(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->less(*op2);
    ctx.magasine.push_back(std::move(result));
}

// <=
void InterpSem7(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->less(*op2);
    ctx.magasine.push_back(std::move(result));
}

// !=
void InterpSem5(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->not_equal(*op2);
    ctx.magasine.push_back(std::move(result));
}

// == 
void InterpSem4(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->equal(*op2);
    ctx.magasine.push_back(std::move(result));
}

// >=
void InterpSem6(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->greater_equal(*op2);
    ctx.magasine.push_back(std::move(result));
}

// >
void InterpSem62(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->greater(*op2);
    ctx.magasine.push_back(std::move(result));
}

// - унарный
void InterpSem1100(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op2->unaryMinus();
    ctx.magasine.push_back(std::move(result));
}


// scan
void InterpSem1101(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    op2->scan(std::cin);
}

// print
void InterpSem1102(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    op2->print(std::cout);
    //std::cout << "\n";// Переход на новую строку
}

// abs
void InterpSem1103(IterpContext& ctx) {
    auto op = ctx.magasine.back(); ctx.magasine.pop_back();
    double v = std::holds_alternative<double>(op->evaluate())
                   ? std::get<double>(op->evaluate())
                   : static_cast<double>(std::get<int>(op->evaluate())); // Преобразование int->real
    ctx.magasine.push_back(std::make_shared<ConstantReal>(std::fabs(v)));
}

// sqrt
void InterpSem1104(IterpContext& ctx) {
    auto op = ctx.magasine.back(); ctx.magasine.pop_back();
    double x = std::holds_alternative<double>(op->evaluate())
                 ? std::get<double>(op->evaluate())
                 : static_cast<double>(std::get<int>(op->evaluate())); // Преобразование int->real
    ctx.magasine.push_back(std::make_shared<ConstantReal>(std::sqrt(x)));
}

// ln
void InterpSem1105(IterpContext& ctx) {
    auto op = ctx.magasine.back(); ctx.magasine.pop_back();
    double x = std::holds_alternative<double>(op->evaluate())
                 ? std::get<double>(op->evaluate())
                 : static_cast<double>(std::get<int>(op->evaluate())); // Преобразование int->real
    ctx.magasine.push_back(std::make_shared<ConstantReal>(std::log(x)));
}

// lg
void InterpSem1106(IterpContext& ctx) {
    auto op = ctx.magasine.back(); ctx.magasine.pop_back();
    double x = std::holds_alternative<double>(op->evaluate())
                 ? std::get<double>(op->evaluate())
                 : static_cast<double>(std::get<int>(op->evaluate())); // Преобразование int->real
    ctx.magasine.push_back(std::make_shared<ConstantReal>(std::log10(x)));
}

// e
void InterpSem1107(IterpContext& ctx) {
    auto op = ctx.magasine.back(); ctx.magasine.pop_back();
    double x = std::holds_alternative<double>(op->evaluate())
                 ? std::get<double>(op->evaluate())
                 : static_cast<double>(std::get<int>(op->evaluate())); // Преобразование int->real
    ctx.magasine.push_back(std::make_shared<ConstantReal>(std::exp(x)));
}

// i - индексация
void InterpSem1108(IterpContext& ctx) {
    auto idx = ctx.magasine.back(); ctx.magasine.pop_back();
    auto arrary = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = arrary->getIdx(*idx);
    ctx.magasine.push_back(std::move(result));
}

// jf - переход при условии false
void InterpSem1109(IterpContext& ctx) {
    auto jmpTo = ctx.magasine.back(); ctx.magasine.pop_back();
    auto cond = ctx.magasine.back(); ctx.magasine.pop_back();
    bool b = std::get<bool>(cond->evaluate());
    if (!b) {
        int target = std::any_cast<int>(jmpTo->get());
        ctx.curLit = target - 1; // Учитывая ctx.Curlex в iterp.cpp
    }
}

// j - безусловный переход
void InterpSem1110(IterpContext& ctx) {
    auto jmpTo = ctx.magasine.back(); ctx.magasine.pop_back();
    int target = std::get<int>(jmpTo->evaluate());
    ctx.curLit = target - 1;
}

// ar - инициализация массива
void InterpSem1111(IterpContext& ctx) {
    auto arrary = ctx.magasine.front(); ctx.magasine.pop_front();
    auto sizeV = ctx.magasine.front(); ctx.magasine.pop_front();
    int size = std::get<int>(sizeV->evaluate());
    if (ctx.magasine.size() != size) throw std::runtime_error("[ERROR] Несовпадение количества элеметов с размером массива");
    for (int i = 0; i < size; i++) {
        auto op = ctx.magasine.front(); ctx.magasine.pop_front();
        arrary->getIdxDirect(i)->set(op->evaluate());
    }
}

const std::unordered_map<int, void(*)(IterpContext&)> SemanticIterpHandlers = {
    {61, InterpSem61},      // =
    {43, InterpSem43},      // +
    {45, InterpSem45},      // -
    {42, InterpSem42},      // *
    {47, InterpSem47},      // /
    {8, InterpSem8},        // ||
    {9, InterpSem9},        // &&
    {33, InterpSem33},      // !
    {60, InterpSem60},      // <
    {7, InterpSem7},        // <=
    {5, InterpSem5},        // !=
    {4, InterpSem4},        // ==
    {6, InterpSem6},        // >=
    {62, InterpSem62},      // >
    {1100, InterpSem1100},  // -k       | -‘
    {1101, InterpSem1101},  // scan     | s
    {1102, InterpSem1102},  // print    | p
    {1103, InterpSem1103},  // abs      | m
    {1104, InterpSem1104},  // sqrt     | r
    {1105, InterpSem1105},  // ln       | ln
    {1106, InterpSem1106},  // lg       | lg
    {1107, InterpSem1107},  // e        | e
    {1108, InterpSem1108},  // [index]  | i
    {1109, InterpSem1109},  // false->  | jf
    {1110, InterpSem1110},  //  ->      | j
    {1111, InterpSem1111},  // {1,2}    | ar
};
