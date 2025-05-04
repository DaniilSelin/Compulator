#include "lexer/semantics/semantics.h"
#include "core/Lexeme.hpp"
#include "lexer.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

// вспомогательные структуры
enum class State {
  S,    // Начальный нетерминал
  C,    // Целое число
  D,    // Символ точки
  I,    // Идентификатор
  E,    // Вещественное число
  U,    // Составная операция сравнения 
  O,    // Логическое НЕ
  K,    // Знак деления
  Q,    // Однострочный комментарий
  M,    // Конец многострочного комментария
  W,    // Однострочный комментарий
  A,    // Логическое ИЛИ
  B,    // Логическое И
  Z, Zs, // Z, Zs – Z*
};

// ДЛЯ ВЫВОДА, УБРАТЬ ПОТОМ
std::ostream& operator<<(std::ostream& os, State state) {
    switch (state) {
        case State::S:
            os << "S";
            break;
        case State::Zs:
            os << "Zs";
            break;
        case State::A:
            os << "A";
            break;
        case State::B:
            os << "B";
            break;
        case State::C:
            os << "C";
            break;
        case State::D:
            os << "D";
            break;
        case State::I:
            os << "I";
            break;
        case State::E:
            os << "E";
            break;
        case State::U:
            os << "U";
            break;
        case State::O:
            os << "O";
            break;
        case State::K:
            os << "K";
            break;
        case State::Q:
            os << "Q";
            break;
        case State::M:
            os << "M";
            break;
        case State::W:
            os << "W";
            break;
        default:
            os << "Unknown";
            break;
    }
    return os;
}

// иначе в мапуне засунуть, енум нехегируемый
namespace std {
    template <>
    struct hash<State> {
        std::size_t operator()(const State& s) const noexcept {
            return static_cast<std::size_t>(s);
        }
    };
}

struct Transition {
    State nextState;
    int action; // Номер семантической программы
};

// Удобная штука для обьявления такого переода: State → (CharClass → Transition)
using InnerMap = std::unordered_map<string, Transition>;
using Table    = std::unordered_map<State, InnerMap>;

static const Table kTransitionTable = {
    { State::S, {
        { "D",      { State::C, 1 } },
        { "L",      { State::I, 2 } },
        { "(",      { State::Z, 3 } },
        { ")",      { State::Z, 3 } },
        { "[",      { State::Z, 3 } },
        { "]",      { State::Z, 3 } },
        { "{",      { State::Z, 3 } },
        { "}",      { State::Z, 3 } },
        { ",",      { State::Z, 3 } },
        { "+",      { State::Z, 3 } },
        { "-",      { State::Z, 3 } },
        { "*",      { State::Z, 3 } },
        { "/",      { State::K, 2 } },
        { "=",      { State::U, 2 } },
        { ">",      { State::U, 2 } },
        { "<",      { State::U, 2 } },
        { ";",      { State::Z, 3 } },
        { "|",      { State::A, 2 } },
        { "!",      { State::O, 2 } },
        { "&",      { State::B, 2 } },
        { " ",      { State::S, 15 } },
        { "\n",     { State::S, 4 } },
        { "Ʇ",      { State::Z, 5 } },
    }},

    { State::C, {
        { "D",      { State::C, 6 } }, 
        { ")",      { State::Zs, 7 } },
        { "]",      { State::Zs, 7 } },
        { "{",      { State::Zs, 7 } },
        { "}",      { State::Zs, 7 } },
        { ".",      { State::D, 8 } },
        { ",",      { State::Zs, 7 } },
        { "+",      { State::Zs, 7 } },
        { "-",      { State::Zs, 7 } },
        { "*",      { State::Zs, 7 } },
        { "/",      { State::Zs, 7 } },
        { "=",      { State::Zs, 7 } },
        { ">",      { State::Zs, 7 } },
        { "<",      { State::Zs, 7 } },
        { ";",      { State::Zs, 7 } },
        { "!",      { State::Zs, 7 } },
        { " ",      { State::Z, 7 } },
    }},

    { State::D, {
        { "D",      { State::E, 9 } },
    }},

    { State::I, {
        { "D",      { State::I, 10 } },
        { "L",      { State::I, 10 } },
        { "(",      { State::Zs,11 } },
        { ")",      { State::Zs,11 } },
        { "]",      { State::Zs,11 } },
        { "}",      { State::Zs,11 } },
        { ",",      { State::Zs,11 } },
        { "+",      { State::Zs,11 } },
        { "-",      { State::Zs,11 } },
        { "*",      { State::Zs,11 } },
        { "/",      { State::Zs,11 } },
        { "=",      { State::Zs,11 } },
        { ">",      { State::Zs,11 } },
        { "<",      { State::Zs,11 } },
        { ";",      { State::Zs,11 } },
        { "!",      { State::Zs,11 } },
        { " ",      { State::Z, 11 } },
    }},

    { State::U, {
        { "D",      { State::Zs, 13 } },
        { "L",      { State::Zs, 13 } },
        { "(",      { State::Zs, 13 } },
        { "-",      { State::Zs, 13 } },
        { "=",      { State::Z, 14 } },
        { " ",      { State::Z, 13 } }
    }},

    { State::O, {
        { "(",      { State::Zs, 13 } },
        { "=",      { State::Z, 14 } },
        { " ",      { State::Z, 13 } }
    }},

    { State::K, {
        { "D",      { State::Zs, 13 } },
        { "L",      { State::Zs, 13 } },
        { "(",      { State::Zs, 13 } },
        { "-",      { State::Zs, 13 } },
        { "*",      { State::Q, 15 } },//??
        { "/",      { State::W, 15 } },//??
        { " ",      { State::Z, 13 } }
    }},

    { State::Q, {
        { "D",      { State::Q, 15 } },
        { "L",      { State::Q, 15 } },
        { "(",      { State::Q, 15 } },
        { ")",      { State::Q, 15 } },
        { "[",      { State::Q, 15 } },
        { "]",      { State::Q, 15 } },
        { "{",      { State::Q, 15 } },
        { "}",      { State::Q, 15 } },
        { ".",      { State::Q, 15 } },
        { ",",      { State::Q, 15 } },
        { "+",      { State::Q, 15 } },
        { "-",      { State::Q, 15 } },
        { "*",      { State::M, 15 } },
        { "/",      { State::Q, 15 } },
        { "=",      { State::Q, 15 } },
        { ">",      { State::Q, 15 } },
        { "<",      { State::Q, 15 } },
        { ";",      { State::Q, 15 } },
        { "|",      { State::Q, 15 } },
        { "!",      { State::Q, 15 } },
        { "&",      { State::Q, 15 } },
        { " ",      { State::Q, 15 } },
        { "\n",     { State::Q, 4  } },
        { "U",      { State::Q, 15 } },
    }},

    { State::M, {
        { "D",      { State::Q, 15 } },
        { "L",      { State::Q, 15 } },
        { "(",      { State::Q, 15 } },
        { ")",      { State::Q, 15 } },
        { "[",      { State::Q, 15 } },
        { "]",      { State::Q, 15 } },
        { "{",      { State::Q, 15 } },
        { "}",      { State::Q, 15 } },
        { ".",      { State::Q, 15 } },
        { ",",      { State::Q, 15 } },
        { "+",      { State::Q, 15 } },
        { "-",      { State::Q, 15 } },
        { "*",      { State::M, 15 } },
        { "/",      { State::S, 15 } },
        { "=",      { State::Q, 15 } },
        { ">",      { State::Q, 15 } },
        { "<",      { State::Q, 15 } },
        { ";",      { State::Q, 15 } },
        { "|",      { State::Q, 15 } },
        { "!",      { State::Q, 15 } },
        { "&",      { State::Q, 15 } },
        { " ",      { State::Q, 15 } },
        { "\n",     { State::Q, 4  } },
        { "U",      { State::Q, 15 } },
    }},

    { State::W, {
        { "D",      { State::Q, 15 } },
        { "L",      { State::Q, 15 } },
        { "(",      { State::Q, 15 } },
        { ")",      { State::Q, 15 } },
        { "[",      { State::Q, 15 } },
        { "]",      { State::Q, 15 } },
        { "{",      { State::Q, 15 } },
        { "}",      { State::Q, 15 } },
        { ".",      { State::Q, 15 } },
        { ",",      { State::Q, 15 } },
        { "+",      { State::Q, 15 } },
        { "-",      { State::Q, 15 } },
        { "*",      { State::M, 15 } },
        { "/",      { State::S, 15 } },
        { "=",      { State::Q, 15 } },
        { ">",      { State::Q, 15 } },
        { "<",      { State::Q, 15 } },
        { ";",      { State::Q, 15 } },
        { "|",      { State::Q, 15 } },
        { "!",      { State::Q, 15 } },
        { "&",      { State::Q, 15 } },
        { " ",      { State::Q, 15 } },
        { "\n",     { State::S, 4  } },
        { "Ʇ",      { State::Zs, 15 } },
        { "U",      { State::Q, 15 } },
    }},

    { State::A, {
        { "|",      { State::Z, 14 } },
    }},

    { State::B, {
        { "&",      { State::Z, 14 } },
    }},
};


int main(int argc, char* argv[]) {
    // 1. Читаем весь файл
    // 
    std::string input;
    {
        std::ifstream in(argc>1?argv[1]:"-", std::ios::binary);
        if (!in) {
            std::cerr<<"Cannot open input\n";
            return 1;
        }
        input.assign(std::istreambuf_iterator<char>(in),
                     std::istreambuf_iterator<char>());
        input.push_back(static_cast<char>(0xE1)); // 'Ʇ'
    }
    //

    std::cout << input << std::endl;

    // 2. Подготовка данных для контекста
    Context ctx;
    ctx.InputSTR = input;
    State state = State::S;

    for (; ctx.i < input.size(); ctx.i++, ctx.pos++) {
        std::cout << "progessing character: '" << ctx.InputSTR[ctx.i] << "' (ASCII: " << static_cast<int>(ctx.InputSTR[ctx.i]) << ") at position (" << ctx.row << ", " << ctx.pos << ")\n";

        std::string term(1, ctx.InputSTR[ctx.i]);

        // проверка на D||L
        if (isalpha(term[0])) term = "L";
        if (isdigit(term[0])) term = "D";
        if (state == State::W || state == State::Q || state == State::M) {
            if (term != "*" && term != "/" && term != "\n") {
                term = "U"; // глушим все кроме возможного конца
            }
        }

        std::cout << "Generated term: '" << term << "'\n";

        InnerMap transfers = kTransitionTable.at(state);
        auto transfer = transfers.find(term);
        if (transfer == transfers.end()) {
            std::cout << "No exact transition found for term: '" << term << "'\n";
            break;
        }

        Transition tr = transfer->second;
        std::cout << "Transition found: action - " << tr.nextState << "/" << tr.action << "\n";

        state = tr.nextState;
        SemanticHandlers.at(tr.action)(ctx);

        if (state == State::Zs) {
            state = State::S;
            ctx.pos--; ctx.i--;
        }
        if (state == State::Z) {
            state = State::S;
        };
        if (state == State::W) {

        }
    }

    // 6. Вывод результата
    std::cout << "\n=== Lexer Results ===\n";
    std::cout << std::setw(4) << "Num" 
              << " | " << std::setw(12) << "Value"
              << " | " << std::setw(10) << "Position (Row, Col)" << "\n";
    std::cout << "---------------------------------------------\n";

    for (auto &lx : ctx.prog) {
        std::cout << std::setw(4) << lx.num
                  << " | " << std::setw(12) << std::quoted(lx.value)
                  << " | (" << std::setw(2) << lx.row << ", " << std::setw(2) << lx.pos << ")\n";
    }
    return 0;
}