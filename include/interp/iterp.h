#include "core/Literal.hpp"
#include <vector>
#include <cmath>

struct IterpContext {
	std::vector<std::unique_ptr<Literal>> magasine;
	std::vector<std::unique_ptr<Literal>> ops;
	int curLit;

	// Конструктор
	IterpContext(std::vector<std::unique_ptr<Literal>> OPS): ops(std::move(OPS)) {
		curLit = 0;
	};
};

// =
void Sem61(IterpContext& ctx) {
    auto op2 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto op1 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    op1->set(op2->evaluate());
}

// +
void Sem43(IterpContext& ctx) {
    auto op2 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto op1 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto result = op2->add(*op1);
    ctx.magasine.push_back(std::move(result));
}

// *
void Sem42(IterpContext& ctx) {
    auto op2 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto op1 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto result = op2->mul(*op1);
    ctx.magasine.push_back(std::move(result));
}

// -
void Sem45(IterpContext& ctx) {
    auto op2 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto op1 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto result = op1->sub(*op2);
    ctx.magasine.push_back(std::move(result));
}

// /
void Sem47(IterpContext& ctx) {
    auto op2 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto op1 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto result = op1->div(*op2);
    ctx.magasine.push_back(std::move(result));
}

// ||
void Sem8(IterpContext& ctx) {
    auto op2 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto op1 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto result = op1->logical_or(*op2);
    ctx.magasine.push_back(std::move(result));
}

// &&
void Sem9(IterpContext& ctx) {
    auto op2 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto op1 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto result = op1->logical_and(*op2);
    ctx.magasine.push_back(std::move(result));
}

// !
void Sem33(IterpContext& ctx) {
    auto op = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto result = op->logical_not();
    ctx.magasine.push_back(std::move(result));
}

// <
void Sem60(IterpContext& ctx) {
    auto op2 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto op1 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto result = op1->less(*op2);
    std::cout << "\nResult Bool: ";
    result->printLex(std::cout);
    std::cout << "\n";
    ctx.magasine.push_back(std::move(result));
}

// <=
void Sem7(IterpContext& ctx) {
    auto op2 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto op1 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto result = op1->less(*op2);
    std::cout << "\nResult Bool: ";
    result->printLex(std::cout);
    std::cout << "\n";
    ctx.magasine.push_back(std::move(result));
}

// !=
void Sem5(IterpContext& ctx) {
    auto op2 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto op1 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto result = op1->not_equal(*op2);
    std::cout << "\nResult Bool: ";
    result->printLex(std::cout);
    std::cout << "\n";
    ctx.magasine.push_back(std::move(result));
}

// == 
void Sem4(IterpContext& ctx) {
    auto op2 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto op1 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto result = op1->equal(*op2);
    std::cout << "\nResult Bool: ";
    result->printLex(std::cout);
    std::cout << "\n";
    ctx.magasine.push_back(std::move(result));
}

// >=
void Sem6(IterpContext& ctx) {
    auto op2 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto op1 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto result = op1->greater_equal(*op2);
    std::cout << "\nResult Bool: ";
    result->printLex(std::cout);
    std::cout << "\n";
    ctx.magasine.push_back(std::move(result));
}

// >
void Sem62(IterpContext& ctx) {
    auto op2 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto op1 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto result = op1->greater(*op2);
    std::cout << "\nResult Bool: ";
    result->printLex(std::cout);
    std::cout << "\n";
    ctx.magasine.push_back(std::move(result));
}

// - унарный
void Sem1100(IterpContext& ctx) {

}


// scan
void Sem1101(IterpContext& ctx) {
    std::cout << "\nRSCAN ";
    auto op2 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    op2->scan(std::cin);
}

// print
void Sem1102(IterpContext& ctx) {
    auto op2 = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    op2->print(std::cout);
    std::cout << std::endl;
}

// modul
void Sem1103(IterpContext& ctx) {

}

// sqrt
void Sem1104(IterpContext& ctx) {

}

// ln
void Sem1105(IterpContext& ctx) {

}

// lg
void Sem1106(IterpContext& ctx) {

}

// e
void Sem1107(IterpContext& ctx) {

}

// i
void Sem1108(IterpContext& ctx) {
}

// jf
void Sem1109(IterpContext& ctx) {
    auto jmpTo = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    auto cond = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    bool b = std::get<bool>(cond->evaluate());
    if (b) {
        int target = std::get<int>(jmpTo->evaluate());
        ctx.curLit = target;
    }
}

// j
void Sem1110(IterpContext& ctx) {
    auto jmpTo = std::move(ctx.magasine.back()); ctx.magasine.pop_back();
    int target = std::get<int>(jmpTo->evaluate());
    ctx.curLit = target;
}

// ar
void Sem1111(IterpContext& ctx) {
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