#include "core/Literal.hpp"
#include "interp/semantics.h"
#include <vector>
#include <cmath>

void interpretation(std::vector<std::shared_ptr<Literal>>&&);

void InterpSem61(IterpContext&);
void InterpSem43(IterpContext&);
void InterpSem45(IterpContext&);
void InterpSem42(IterpContext&);
void InterpSem47(IterpContext&);
void InterpSem8(IterpContext&);
void InterpSem9(IterpContext&);
void InterpSem33(IterpContext&);
void InterpSem60(IterpContext&);
void InterpSem7(IterpContext&);
void InterpSem5(IterpContext&);
void InterpSem4(IterpContext&);
void InterpSem6(IterpContext&);
void InterpSem62(IterpContext&);
void InterpSem1100(IterpContext&);
void InterpSem1101(IterpContext&);
void InterpSem1102(IterpContext&);
void InterpSem1103(IterpContext&);
void InterpSem1104(IterpContext&);
void InterpSem1105(IterpContext&);
void InterpSem1106(IterpContext&);
void InterpSem1107(IterpContext&);
void InterpSem1108(IterpContext&);
void InterpSem1109(IterpContext&);
void InterpSem1110(IterpContext&);
void InterpSem1111(IterpContext&);

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