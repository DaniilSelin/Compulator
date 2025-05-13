#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <any>
#include <cstdint>
#include <stdexcept>

void sem1(GenContext& ctx) {
    ctx.MarkVector.push_front(static_cast<int>(ctx.OPS.size()));
    ctx.OPS.push_back({typeL::mark, 0});
    ctx.OPS.push_back({typeL::operation, 1109});
}

void sem2(GenContext& ctx) {
    int markPos = ctx.MarkVector.front(); 
    ctx.OPS[markPos].value = static_cast<int>(ctx.OPS.size()) + 2;
    ctx.MarkVector.pop_front();                         
    ctx.MarkVector.push_front(static_cast<int>(ctx.OPS.size()));
    ctx.OPS.push_back({typeL::mark, 0});
    ctx.OPS.push_back({typeL::operation, 1110});
}

void sem3(GenContext& ctx) {
   int markPos = ctx.MarkVector.front();
   ctx.OPS[markPos].value = static_cast<int>(ctx.OPS.size());
   ctx.MarkVector.pop_front();
}

void sem4(GenContext& ctx) {
    ctx.MarkVector.push_front(static_cast<int>(ctx.OPS.size()));
}  

void sem5(GenContext& ctx) {
    int markPos = ctx.MarkVector.front();
    ctx.OPS[markPos].value = static_cast<int>(ctx.OPS.size()) + 2;
    ctx.MarkVector.pop_front();
    int prevMark = ctx.MarkVector.front();
    ctx.OPS.push_back({typeL::mark, prevMark});
    ctx.MarkVector.pop_front();
    ctx.OPS.push_back({typeL::operation, 1110});
}

void sem6(GenContext& ctx) {
    const auto& lex = ctx.prog.at(ctx.curLex);
    std::string name = lex.value;
    if (ctx.VarMap.find(name) != ctx.VarMap.end()) {
        throw std::runtime_error("reassignment " + name);
    } else {
        ctx.VarMap[name] = VarObject{0, ctx.InitReal};
    }
    ctx.MarkVector.push_front(static_cast<int>(ctx.OPS.size()));
    ctx.OPS.push_back({typeL::link, name});
}

void sem7(GenContext& ctx) {
    int markPos = ctx.MarkVector.front();
    std::string name = std::any_cast<std::string>(ctx.OPS[markPos].value);
    ctx.MarkVector.pop_front();
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

void sem8(GenContext& ctx) {
    int markPos = ctx.MarkVector.front();
    std::cout << ctx.MarkVector.size() << std::endl;
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
    ctx.OPS.push_back({typeL::mark, markPos});
    ctx.OPS.push_back({typeL::operation, 1111});
}

void sem9(GenContext& ctx) {
    std::cout << ctx.MarkVector.size() << std::endl;
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

void sem10(GenContext& ctx) {
    ctx.InitReal = true;
}

void sem11(GenContext& ctx) {
     ctx.InitReal = false;
}
