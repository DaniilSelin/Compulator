#include "core/Literal.hpp"
#include <vector>

struct IterpContext {
    std::deque<std::shared_ptr<Literal>> magasine;
    std::vector<std::shared_ptr<Literal>> ops;
    int curLit = 0;
    // Конструктор
    IterpContext(std::vector<std::shared_ptr<Literal>> OPS): ops(std::move(OPS)) {}
};