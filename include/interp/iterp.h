#include "core/Literal.hpp"
#include <vector>
#include <cmath>

struct IterpContext {
	std::deque<std::shared_ptr<Literal>> magasine;
	std::vector<std::shared_ptr<Literal>> ops;
	int curLit;

	// Конструктор
	IterpContext(std::vector<std::shared_ptr<Literal>> OPS): ops(std::move(OPS)) {
		curLit = 0;
	};
};

// =
void Sem61(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    op1->set(op2->evaluate());
}

// +
void Sem43(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op2->add(*op1);
    ctx.magasine.push_back(std::move(result));
}

// *
void Sem42(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op2->mul(*op1);
    ctx.magasine.push_back(std::move(result));
}

// -
void Sem45(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->sub(*op2);
    ctx.magasine.push_back(std::move(result));
}

// /
void Sem47(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->div(*op2);
    ctx.magasine.push_back(std::move(result));
}

// ||
void Sem8(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->logical_or(*op2);
    ctx.magasine.push_back(std::move(result));
}

// &&
void Sem9(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->logical_and(*op2);
    ctx.magasine.push_back(std::move(result));
}

// !
void Sem33(IterpContext& ctx) {
    auto op = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op->logical_not();
    ctx.magasine.push_back(std::move(result));
}

// <
void Sem60(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->less(*op2);
    ctx.magasine.push_back(std::move(result));
}

// <=
void Sem7(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->less(*op2);
    ctx.magasine.push_back(std::move(result));
}

// !=
void Sem5(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->not_equal(*op2);
    ctx.magasine.push_back(std::move(result));
}

// == 
void Sem4(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->equal(*op2);
    ctx.magasine.push_back(std::move(result));
}

// >=
void Sem6(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->greater_equal(*op2);
    ctx.magasine.push_back(std::move(result));
}

// >
void Sem62(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto op1 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op1->greater(*op2);
    ctx.magasine.push_back(std::move(result));
}

// - унарный
void Sem1100(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = op2->unaryMinus();
    ctx.magasine.push_back(std::move(result));
}


// scan
void Sem1101(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    op2->scan(std::cin);
}

// print
void Sem1102(IterpContext& ctx) {
    auto op2 = ctx.magasine.back(); ctx.magasine.pop_back();
    op2->print(std::cout);
    std::cout << std::endl;// костыль
}

// modul
void Sem1103(IterpContext& ctx) {
    auto op = ctx.magasine.back(); ctx.magasine.pop_back();
    double v = std::holds_alternative<double>(op->evaluate())
                   ? std::get<double>(op->evaluate())
                   : static_cast<double>(std::get<int>(op->evaluate()));
    ctx.magasine.push_back(std::make_shared<ConstantReal>(std::fabs(v)));
}

// sqrt
void Sem1104(IterpContext& ctx) {
    auto op = ctx.magasine.back(); ctx.magasine.pop_back();
    double x = std::holds_alternative<double>(op->evaluate())
                 ? std::get<double>(op->evaluate())
                 : static_cast<double>(std::get<int>(op->evaluate()));
    ctx.magasine.push_back(std::make_shared<ConstantReal>(std::sqrt(x)));
}

// ln
void Sem1105(IterpContext& ctx) {
    auto op = ctx.magasine.back(); ctx.magasine.pop_back();
    double x = std::holds_alternative<double>(op->evaluate())
                 ? std::get<double>(op->evaluate())
                 : static_cast<double>(std::get<int>(op->evaluate()));
    ctx.magasine.push_back(std::make_shared<ConstantReal>(std::log(x)));
}

// lg
void Sem1106(IterpContext& ctx) {
    auto op = ctx.magasine.back(); ctx.magasine.pop_back();
    double x = std::holds_alternative<double>(op->evaluate())
                 ? std::get<double>(op->evaluate())
                 : static_cast<double>(std::get<int>(op->evaluate()));
    ctx.magasine.push_back(std::make_shared<ConstantReal>(std::log10(x)));
}

// e
void Sem1107(IterpContext& ctx) {
    auto op = ctx.magasine.back(); ctx.magasine.pop_back();
    double x = std::holds_alternative<double>(op->evaluate())
                 ? std::get<double>(op->evaluate())
                 : static_cast<double>(std::get<int>(op->evaluate()));
    ctx.magasine.push_back(std::make_shared<ConstantReal>(std::exp(x)));
}

// i
void Sem1108(IterpContext& ctx) {
    auto idx = ctx.magasine.back(); ctx.magasine.pop_back();
    auto arrary = ctx.magasine.back(); ctx.magasine.pop_back();
    auto result = arrary->getIdx(*idx);
    ctx.magasine.push_back(std::move(result));
}

// jf
void Sem1109(IterpContext& ctx) {
    auto jmpTo = ctx.magasine.back(); ctx.magasine.pop_back();
    auto cond = ctx.magasine.back(); ctx.magasine.pop_back();
    bool b = std::get<bool>(cond->evaluate());
    if (!b) {
        int target = std::any_cast<int>(jmpTo->get());
        ctx.curLit = target - 1; // Учитывая ctx.Curlex в iterp.cpp
    }
}

// j
void Sem1110(IterpContext& ctx) {
    auto jmpTo = ctx.magasine.back(); ctx.magasine.pop_back();
    int target = std::get<int>(jmpTo->evaluate());
    ctx.curLit = target - 1;
}

// ar
void Sem1111(IterpContext& ctx) {
    auto arrary = ctx.magasine.front(); ctx.magasine.pop_front();
    auto sizeV = ctx.magasine.front(); ctx.magasine.pop_front();
    int size = std::get<int>(sizeV->evaluate());
    if (ctx.magasine.size() != size) throw std::runtime_error("Initialization not match the size");
    for (int i = 0; i < size; i++) {
        auto op = ctx.magasine.front(); ctx.magasine.pop_front();
        arrary->getIdxDirect(i)->set(op->evaluate());
    }
}

const std::unordered_map<int, void(*)(IterpContext&)> SemanticIterpHandlers = {
    {61, Sem61},
    {43, Sem43},
    {45, Sem45},
    {42, Sem42},
    {47, Sem47},
    {8, Sem8},
    {9, Sem9},
    {33, Sem33},
    {60, Sem60},
    {7, Sem7},
    {5, Sem5},
    {4, Sem4},
    {6, Sem6},
    {62, Sem62},
    {1100, Sem1100},
    {1101, Sem1101},
    {1102, Sem1102},
    {1103, Sem1103},
    {1104, Sem1104},
    {1105, Sem1105},
    {1106, Sem1106},
    {1107, Sem1107},
    {1108, Sem1108},
    {1109, Sem1109},
    {1110, Sem1110},
    {1111, Sem1111},
};