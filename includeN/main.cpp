#include "additionalFunction.h"
#include "locale.h"
#include "Windows.h"

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
    case State::Z:
        os << "Z";
        break;
    default:
        os << "Unknown";
        break;
    }
    return os;
}

void lexemeReader(Context& ctx, const std::string& inputString) {
    ctx.InputSTR = inputString;
    State state = State::S;

    for (; ctx.i < inputString.size(); ctx.i++, ctx.pos++) {
        std::cout << "Обрабатываемый символ: '" << ctx.InputSTR[ctx.i] << " (Состояние: " << state << ") на позиции [" << ctx.row << " ; " << ctx.pos << "]\n";

        std::string term(1, ctx.InputSTR[ctx.i]);

        // проверка на Letter || Digit
        if (isalpha(term[0])) term = "L";
        if (isdigit(term[0])) term = "D";
        if (state == State::W || state == State::Q || state == State::M) {
            if (term != "*" && term != "/" && term != "\n") {
                term = "U"; // Комментарий, ищем конец комментария
            }
        }

        std::cout << "Полученное правило: '" << term << "'\n";

        InnerMap transfers = kTransitionTable.at(state);
        auto transfer = transfers.find(term);
        if (transfer == transfers.end()) {
            std::cout << "Не обнаружены действия для правила: '" << term << "'\n";
            break;
        }

        Transition tr = transfer->second;
        std::cout << "Переход обнаружен: действие - " << tr.nextState << "/" << tr.action << "\n";

        state = tr.nextState;
        SemanticHandlers.at(tr.action)(ctx);

        if (state == State::Zs) {
            state = State::S;
            ctx.pos--; ctx.i--;
        }
        if (state == State::Z) {
            state = State::S;
        };
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());
    std::string input = "input.txt";
    std::string lexemeInput = "lexeme.json";
    std::unordered_map<int, std::string> g_lexemeDescriptions;
    if (!loadLexemeDescriptions(g_lexemeDescriptions, lexemeInput)) {
        std::cout << "Проблема с загрузкой лексем\n";
        return 1;
    }
    std::string inputString;
    inputString = loadTextFile(input);
    std::cout << inputString << std::endl;
    Context ctx;
    lexemeReader(ctx, inputString);
    outputResult(ctx.prog, g_lexemeDescriptions);

	return 0;
}