#include "include/core/Literal.hpp"
#include <cassert>
#include <vector>
#include <memory>
#include <iostream>

struct IterpContext {
    std::vector<std::unique_ptr<Literal>> magasine;
    int curLit = 0;
};

// Определение глобальной VarMap
std::unordered_map<std::string, VarObject> VarMap;

// +
void Sem43(IterpContext& ctx) {
    auto op2 = std::move(ctx.magasine.back());
    ctx.magasine.pop_back();
    auto op1 = std::move(ctx.magasine.back());
    ctx.magasine.pop_back();
    auto result = op2->add(*op1);
    ctx.magasine.push_back(std::move(result));
}
// *
void Sem42(IterpContext& ctx) {
    auto op2 = std::move(ctx.magasine.back());
    ctx.magasine.pop_back();
    auto op1 = std::move(ctx.magasine.back());
    ctx.magasine.pop_back();
    auto result = op2->mul(*op1);
    ctx.magasine.push_back(std::move(result));
}
// -
void Sem45(IterpContext& ctx) {
    auto op2 = std::move(ctx.magasine.back());
    ctx.magasine.pop_back();
    auto op1 = std::move(ctx.magasine.back());
    ctx.magasine.pop_back();
    auto result = op2->sub(*op1);
    ctx.magasine.push_back(std::move(result));
}
// /
void Sem47(IterpContext& ctx) {
    auto op2 = std::move(ctx.magasine.back());
    ctx.magasine.pop_back();
    auto op1 = std::move(ctx.magasine.back());
    ctx.magasine.pop_back();
    auto result = op2->div(*op1);
    ctx.magasine.push_back(std::move(result));
}

// Тесты
void testAdd() {
    // Тест 1: int + int
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantInt>(2));
        ctx.magasine.push_back(std::make_unique<ConstantInt>(3));
        Sem43(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<int>(res) == 5);
    }
    // Тест 2: real + real
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantReal>(2.5));
        ctx.magasine.push_back(std::make_unique<ConstantReal>(3.5));
        Sem43(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res) == 6.0);
    }
    // Тест 3: int + real
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantInt>(2));
        ctx.magasine.push_back(std::make_unique<ConstantReal>(3.5));
        Sem43(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res) == 5.5);
    }
    // Тест 4: real + int
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantReal>(3.5));
        ctx.magasine.push_back(std::make_unique<ConstantInt>(2));
        Sem43(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res) == 5.5);
    }

    // Тест 5: LinkInt + ConstantInt
    {
        // создаём реальную переменную int
        int* pInt = new int(10);
        VarMap["x"] = VarObject{reinterpret_cast<uintptr_t>(pInt), sizeof(int), /*isReal=*/false};
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<LinkInt>("x"));
        ctx.magasine.push_back(std::make_unique<ConstantInt>(5));
        Sem43(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<int>(res) == 15);
        delete pInt;
    }

    // Тест 6: LinkReal + ConstantReal
    {
        // создаём реальную переменную double
        double* pReal = new double(1.5);
        VarMap["y"] = VarObject{reinterpret_cast<uintptr_t>(pReal), sizeof(double), /*isReal=*/true};
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<LinkReal>("y"));
        ctx.magasine.push_back(std::make_unique<ConstantReal>(2.5));
        Sem43(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res) == 4.0);
        delete pReal;
    }

    // Тест 7: ConstantInt + LinkInt
    {
        int* pInt = new int(7);
        VarMap["a"] = VarObject{reinterpret_cast<uintptr_t>(pInt), sizeof(int), /*isReal=*/false};
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantInt>(3));
        ctx.magasine.push_back(std::make_unique<LinkInt>("a"));
        Sem43(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<int>(res) == 10);
        delete pInt;
    }

    // Тест 8: ConstantInt + LinkReal
    {
        double* pReal = new double(2.5);
        VarMap["b"] = VarObject{reinterpret_cast<uintptr_t>(pReal), sizeof(double), /*isReal=*/true};
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantInt>(4));
        ctx.magasine.push_back(std::make_unique<LinkReal>("b"));
        Sem43(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res) == 6.5);
        delete pReal;
    }

    // Тест 9: ConstantReal + LinkInt
    {
        int* pInt = new int(8);
        VarMap["c"] = VarObject{reinterpret_cast<uintptr_t>(pInt), sizeof(int), /*isReal=*/false};
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantReal>(1.5));
        ctx.magasine.push_back(std::make_unique<LinkInt>("c"));
        Sem43(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res) == 9.5);
        delete pInt;
    }

    // Тест 10: LinkInt + LinkReal
    {
        int* pInt = new int(5);
        VarMap["d"] = VarObject{reinterpret_cast<uintptr_t>(pInt), sizeof(int), /*isReal=*/false};
        double* pReal = new double(2.0);
        VarMap["e"] = VarObject{reinterpret_cast<uintptr_t>(pReal), sizeof(double), /*isReal=*/true};
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<LinkInt>("d"));
        ctx.magasine.push_back(std::make_unique<LinkReal>("e"));
        Sem43(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res) == 7.0);
        delete pInt;
        delete pReal;
    }

    // Тест 11: LinkReal + LinkInt
    {
        int* pInt2 = new int(3);
        VarMap["f"] = VarObject{reinterpret_cast<uintptr_t>(pInt2), sizeof(int), /*isReal=*/false};
        double* pReal2 = new double(4.0);
        VarMap["g"] = VarObject{reinterpret_cast<uintptr_t>(pReal2), sizeof(double), /*isReal=*/true};
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<LinkReal>("g"));
        ctx.magasine.push_back(std::make_unique<LinkInt>("f"));
        Sem43(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res) == 7.0);
        delete pInt2;
        delete pReal2;
    }

    // Тест 12: IndirectIdxInt + ConstantInt
    {
        int* arr = new int[3]{10,20,30};
        auto addr = reinterpret_cast<std::uintptr_t>(&arr[1]);
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<IndirectIdxInt>(addr));
        ctx.magasine.push_back(std::make_unique<ConstantInt>(5));
        Sem43(ctx);
        auto res12 = ctx.magasine.back()->evaluate();
        assert(std::get<int>(res12) == 25);
        delete[] arr;
    }

    // Тест 13: ConstantReal + IndirectIdxReal
    {
        double* arr = new double[2]{1.5,2.5};
        auto addrR = reinterpret_cast<std::uintptr_t>(&arr[0]);
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantReal>(3.0));
        ctx.magasine.push_back(std::make_unique<IndirectIdxReal>(addrR));
        Sem43(ctx);
        auto res13 = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res13) == 4.5);
        delete[] arr;
    }
    // Тест 14: IndirectIdxInt + LinkReal
    {
        int* arr2 = new int[2]{5,7};
        double* d = new double(2.0);
        VarMap["hr"] = VarObject{reinterpret_cast<std::uintptr_t>(d), sizeof(double), true};
        auto addr2 = reinterpret_cast<std::uintptr_t>(&arr2[1]);
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<IndirectIdxInt>(addr2));
        ctx.magasine.push_back(std::make_unique<LinkReal>("hr"));
        Sem43(ctx);
        auto res14 = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res14) == 9.0);
        delete[] arr2;
        delete d;
    }
    // Тест 15: IndirectIdxReal + IndirectIdxInt
    {
        double* arrR = new double[2]{1.0,3.0};
        int* arrI = new int[2]{4,6};
        auto addrR2 = reinterpret_cast<std::uintptr_t>(&arrR[1]);
        auto addrI2 = reinterpret_cast<std::uintptr_t>(&arrI[0]);
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<IndirectIdxReal>(addrR2));
        ctx.magasine.push_back(std::make_unique<IndirectIdxInt>(addrI2));
        Sem43(ctx);
        auto res15 = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res15) == 7.0);
        delete[] arrR;
        delete[] arrI;
    }

    std::cout << "Tests add passed!" << std::endl;
}

void testMul() {
    // Тест 1: int * int
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantInt>(2));
        ctx.magasine.push_back(std::make_unique<ConstantInt>(3));
        Sem42(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<int>(res) == 6);
    }
    // Тест 2: real * real
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantReal>(2.5));
        ctx.magasine.push_back(std::make_unique<ConstantReal>(4.0));
        Sem42(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res) == 10.0);
    }
    // Тест 3: int * real
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantInt>(2));
        ctx.magasine.push_back(std::make_unique<ConstantReal>(3.5));
        Sem42(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res) == 7.0);
    }
    // Тест 4: real * int
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantReal>(1.5));
        ctx.magasine.push_back(std::make_unique<ConstantInt>(4));
        Sem42(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res) == 6.0);
    }
    // Тест 5: LinkInt * ConstantInt
    {
        int* pInt = new int(6);
        VarMap["x"] = VarObject{reinterpret_cast<uintptr_t>(pInt), sizeof(int), false};
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<LinkInt>("x"));
        ctx.magasine.push_back(std::make_unique<ConstantInt>(3));
        Sem42(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<int>(res) == 18);
        delete pInt;
    }
    // Тест 6: LinkReal * ConstantReal
    {
        double* pReal = new double(1.5);
        VarMap["y"] = VarObject{reinterpret_cast<uintptr_t>(pReal), sizeof(double), true};
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<LinkReal>("y"));
        ctx.magasine.push_back(std::make_unique<ConstantReal>(2.0));
        Sem42(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res) == 3.0);
        delete pReal;
    }
    // Тест 7: ConstantInt * LinkInt
    {
        int* pInt = new int(5);
        VarMap["a"] = VarObject{reinterpret_cast<uintptr_t>(pInt), sizeof(int), false};
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantInt>(4));
        ctx.magasine.push_back(std::make_unique<LinkInt>("a"));
        Sem42(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<int>(res) == 20);
        delete pInt;
    }
    // Тест 8: ConstantInt * LinkReal
    {
        double* pReal = new double(2.0);
        VarMap["b"] = VarObject{reinterpret_cast<uintptr_t>(pReal), sizeof(double), true};
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantInt>(3));
        ctx.magasine.push_back(std::make_unique<LinkReal>("b"));
        Sem42(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res) == 6.0);
        delete pReal;
    }
    // Тест 9: ConstantReal * LinkInt
    {
        int* pInt = new int(4);
        VarMap["c"] = VarObject{reinterpret_cast<uintptr_t>(pInt), sizeof(int), false};
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantReal>(2.5));
        ctx.magasine.push_back(std::make_unique<LinkInt>("c"));
        Sem42(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res) == 10.0);
        delete pInt;
    }
    // Тест 10: LinkInt * LinkReal
    {
        int* pInt = new int(2);
        double* pReal = new double(3.0);
        VarMap["d"] = VarObject{reinterpret_cast<uintptr_t>(pInt), sizeof(int), false};
        VarMap["e"] = VarObject{reinterpret_cast<uintptr_t>(pReal), sizeof(double), true};
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<LinkInt>("d"));
        ctx.magasine.push_back(std::make_unique<LinkReal>("e"));
        Sem42(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res) == 6.0);
        delete pInt;
        delete pReal;
    }
    // Тест 11: LinkReal * LinkInt
    {
        int* pInt = new int(3);
        double* pReal = new double(1.5);
        VarMap["f"] = VarObject{reinterpret_cast<uintptr_t>(pInt), sizeof(int), false};
        VarMap["g"] = VarObject{reinterpret_cast<uintptr_t>(pReal), sizeof(double), true};
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<LinkReal>("g"));
        ctx.magasine.push_back(std::make_unique<LinkInt>("f"));
        Sem42(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res) == 4.5);
        delete pInt;
        delete pReal;
    }
    // Тест 12: IndirectIdxInt * ConstantInt
    {
        int* arr = new int[2]{3, 7};
        auto addr = reinterpret_cast<std::uintptr_t>(&arr[1]);
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<IndirectIdxInt>(addr));
        ctx.magasine.push_back(std::make_unique<ConstantInt>(2));
        Sem42(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<int>(res) == 14);
        delete[] arr;
    }
    // Тест 13: ConstantReal * IndirectIdxReal
    {
        double* arr = new double[2]{1.5, 2.0};
        auto addr = reinterpret_cast<std::uintptr_t>(&arr[0]);
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantReal>(3.0));
        ctx.magasine.push_back(std::make_unique<IndirectIdxReal>(addr));
        Sem42(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res) == 4.5);
        delete[] arr;
    }
    // Тест 14: IndirectIdxInt * LinkReal
    {
        int* arr = new int[2]{2, 5};
        double* realVal = new double(2.0);
        VarMap["hr"] = VarObject{reinterpret_cast<uintptr_t>(realVal), sizeof(double), true};
        auto addr = reinterpret_cast<std::uintptr_t>(&arr[1]);
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<IndirectIdxInt>(addr));
        ctx.magasine.push_back(std::make_unique<LinkReal>("hr"));
        Sem42(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res) == 10.0);
        delete[] arr;
        delete realVal;
    }
    // Тест 15: IndirectIdxReal * IndirectIdxInt
    {
        double* rarr = new double[2]{1.0, 2.5};
        int* iarr = new int[2]{2, 3};
        auto addrR = reinterpret_cast<std::uintptr_t>(&rarr[1]);
        auto addrI = reinterpret_cast<std::uintptr_t>(&iarr[0]);
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<IndirectIdxReal>(addrR));
        ctx.magasine.push_back(std::make_unique<IndirectIdxInt>(addrI));
        Sem42(ctx);
        auto res = ctx.magasine.back()->evaluate();
        assert(std::get<double>(res) == 5.0);
        delete[] rarr;
        delete[] iarr;
    }

    std::cout << "Tests mul passed!" << std::endl;
}

void testSub() {
    auto makeLinkInt = [](const std::string& name, int value) {
        int* ptr = new int(value);
        VarMap[name] = VarObject{reinterpret_cast<uintptr_t>(ptr), sizeof(int), false};
        return std::make_unique<LinkInt>(name);
    };

    auto makeLinkReal = [](const std::string& name, double value) {
        double* ptr = new double(value);
        VarMap[name] = VarObject{reinterpret_cast<uintptr_t>(ptr), sizeof(double), false};
        return std::make_unique<LinkReal>(name);
    };

    // 1: int - int
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantInt>(2));
        ctx.magasine.push_back(std::make_unique<ConstantInt>(5));
        Sem45(ctx);
        assert(std::get<int>(ctx.magasine.back()->evaluate()) == 3);
    }

    // 2: int - real
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantInt>(2));
        ctx.magasine.push_back(std::make_unique<ConstantReal>(5.5));
        Sem45(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 3.5);
    }

    // 3: real - int
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantReal>(2.5));
        ctx.magasine.push_back(std::make_unique<ConstantInt>(5));
        Sem45(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 2.5);
    }

    // 4: real - real
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantReal>(1.5));
        ctx.magasine.push_back(std::make_unique<ConstantReal>(5.5));
        Sem45(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 4.0);
    }

    // 5: VariableInt - ConstantInt
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::move(makeLinkInt("x", 2)));
        ctx.magasine.push_back(std::make_unique<ConstantInt>(5));
        Sem45(ctx);
        assert(std::get<int>(ctx.magasine.back()->evaluate()) == 3);
    }

    // 6: ConstantInt - VariableInt
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantInt>(5));
        ctx.magasine.push_back(std::move(makeLinkInt("x", 10)));
        Sem45(ctx);
        assert(std::get<int>(ctx.magasine.back()->evaluate()) == 5);
    }

    // 7: VariableReal - ConstantReal
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::move(makeLinkReal("y", 2.5)));
        ctx.magasine.push_back(std::make_unique<ConstantReal>(5.5));
        Sem45(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 3.0);
    }

    // 8: ConstantReal - VariableReal
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantReal>(2.5));
        ctx.magasine.push_back(std::move(makeLinkReal("y", 5.5)));
        Sem45(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 3.0);
    }

    // 9: VariableInt - VariableInt
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::move(makeLinkInt("xx", 2)));
        ctx.magasine.push_back(std::move(makeLinkInt("x", 7)));
        Sem45(ctx);
        assert(std::get<int>(ctx.magasine.back()->evaluate()) == 5);
    }

    // 10: VariableReal - VariableReal
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::move(makeLinkReal("yy", 3.3)));
        ctx.magasine.push_back(std::move(makeLinkReal("y", 8.8)));
        Sem45(ctx);
        // так как неточность есть у double
        assert(std::abs(std::get<double>(ctx.magasine.back()->evaluate()) - 5.5) < 1e-9);
    }

    // 11: VariableInt - ConstantReal
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::move(makeLinkInt("x", 2)));
        ctx.magasine.push_back(std::make_unique<ConstantReal>(5.5));
        Sem45(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 3.5);
    }

    // 12: ConstantReal - VariableInt
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantReal>(5.5));
        ctx.magasine.push_back(std::move(makeLinkInt("x", 7)));
        Sem45(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 1.5);
    }

    // 13: VariableReal - ConstantInt
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::move(makeLinkReal("y", 2.5)));
        ctx.magasine.push_back(std::make_unique<ConstantInt>(6));
        Sem45(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 3.5);
    }

    // 14: ConstantInt - VariableReal
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantInt>(3));
        ctx.magasine.push_back(std::move(makeLinkReal("y", 7.5)));
        Sem45(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 4.5);
    }

    // 15: VariableInt - VariableReal
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::move(makeLinkInt("x", 3)));
        ctx.magasine.push_back(std::move(makeLinkReal("y", 7.5)));
        Sem45(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 4.5);
    }

    std::cout << "Tests sub passed!" << std::endl;
}

void testDiv() {
    auto makeLinkInt = [](const std::string& name, int value) {
        int* ptr = new int(value);
        VarMap[name] = VarObject{reinterpret_cast<uintptr_t>(ptr), sizeof(int), false};
        return std::make_unique<LinkInt>(name);
    };

    auto makeLinkReal = [](const std::string& name, double value) {
        double* ptr = new double(value);
        VarMap[name] = VarObject{reinterpret_cast<uintptr_t>(ptr), sizeof(double), false};
        return std::make_unique<LinkReal>(name);
    };

    // 1: int / int
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantInt>(2));
        ctx.magasine.push_back(std::make_unique<ConstantInt>(6));
        Sem47(ctx);
        assert(std::get<int>(ctx.magasine.back()->evaluate()) == 3); // 6 / 2
    }

    // 2: int / real
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantInt>(2));
        ctx.magasine.push_back(std::make_unique<ConstantReal>(5.0));
        Sem47(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 2.5);
    }

    // 3: real / int
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantReal>(2.0));
        ctx.magasine.push_back(std::make_unique<ConstantInt>(6));
        Sem47(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 3.0);
    }

    // 4: real / real
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantReal>(2.0));
        ctx.magasine.push_back(std::make_unique<ConstantReal>(4.0));
        Sem47(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 2.0);
    }

    // 5: VariableInt / ConstantInt
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::move(makeLinkInt("x", 2)));
        ctx.magasine.push_back(std::make_unique<ConstantInt>(10));
        Sem47(ctx);
        assert(std::get<int>(ctx.magasine.back()->evaluate()) == 5);
    }

    // 6: ConstantInt / VariableInt
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantInt>(4));
        ctx.magasine.push_back(std::move(makeLinkInt("x", 12)));
        Sem47(ctx);
        assert(std::get<int>(ctx.magasine.back()->evaluate()) == 3);
    }

    // 7: VariableReal / ConstantReal
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::move(makeLinkReal("y", 2.5)));
        ctx.magasine.push_back(std::make_unique<ConstantReal>(10.0));
        Sem47(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 4.0);
    }

    // 8: ConstantReal / VariableReal
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantReal>(5.0));
        ctx.magasine.push_back(std::move(makeLinkReal("y", 10.0)));
        Sem47(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 2.0);
    }

    // 9: VariableInt / VariableInt
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::move(makeLinkInt("xx", 4)));
        ctx.magasine.push_back(std::move(makeLinkInt("x", 20)));
        Sem47(ctx);
        assert(std::get<int>(ctx.magasine.back()->evaluate()) == 5);
    }

    // 10: VariableReal / VariableReal
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::move(makeLinkReal("yy", 2.0)));
        ctx.magasine.push_back(std::move(makeLinkReal("y", 6.0)));
        Sem47(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 3.0);
    }

    // 11: VariableInt / ConstantReal
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::move(makeLinkInt("x", 2)));
        ctx.magasine.push_back(std::make_unique<ConstantReal>(5.0));
        Sem47(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 2.5);
    }

    // 12: ConstantReal / VariableInt
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantReal>(2.0));
        ctx.magasine.push_back(std::move(makeLinkInt("x", 6)));
        Sem47(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 3.0);
    }

    // 13: VariableReal / ConstantInt
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::move(makeLinkReal("y", 2.0)));
        ctx.magasine.push_back(std::make_unique<ConstantInt>(8));
        Sem47(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 4.0);
    }

    // 14: ConstantInt / VariableReal
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::make_unique<ConstantInt>(2));
        ctx.magasine.push_back(std::move(makeLinkReal("y", 10.0)));
        Sem47(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 5.0);
    }

    // 15: VariableInt / VariableReal
    {
        IterpContext ctx;
        ctx.magasine.push_back(std::move(makeLinkInt("x", 2)));
        ctx.magasine.push_back(std::move(makeLinkReal("y", 6.0)));
        Sem47(ctx);
        assert(std::get<double>(ctx.magasine.back()->evaluate()) == 3.0);
    }

    std::cout << "Tests div passed!" << std::endl;
}

int main() {

    testAdd();
    testMul();
    testSub();
    testDiv();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}