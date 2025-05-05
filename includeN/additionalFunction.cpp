#include "additionalFunction.h"

// Семантические функции
void sem1(Context& ctx) {
    ctx.n = ctx.InputSTR[ctx.i] - '0';
}

void sem2(Context& ctx) {
    ctx.name = std::string(1, ctx.InputSTR[ctx.i]);
}

void sem3(Context& ctx) {
    ctx.prog.push_back(
        {
        static_cast<int>(ctx.InputSTR[ctx.i]),
        std::string(1, ctx.InputSTR[ctx.i]),
        ctx.row,
        ctx.pos
        }
    );
}

void sem4(Context& ctx) {
    ctx.row++;
    ctx.pos = 0;
}

void sem5(Context& ctx) {
    ctx.prog.push_back(
        {
        20,
        std::string(1, ctx.InputSTR[ctx.i]),
        ctx.row,
        ctx.pos
        }
    );
}

void sem6(Context& ctx) {
    ctx.n = 10 * ctx.n + (ctx.InputSTR[ctx.i] - '0');
}

void sem7(Context& ctx) {
    ctx.prog.push_back(
        {
        2,
        std::to_string(ctx.n),
        ctx.row,
        ctx.pos
        }
    );
}

void sem8(Context& ctx) {
    ctx.d = 1;
    ctx.x = ctx.n;
}

void sem9(Context& ctx) {
    ctx.d *= 0.1;
    ctx.x += (ctx.InputSTR[ctx.i] - '0') * ctx.d;
}

void sem10(Context& ctx) {
    ctx.name.push_back(ctx.InputSTR[ctx.i]);
}

void sem11(Context& ctx) {
    auto it = KeywordMap.find(ctx.name);
    int code = (it != KeywordMap.end()) ? it->second : 1;
    ctx.prog.push_back(
        {
        code,
        ctx.name,
        ctx.row,
        ctx.pos
        }
    );
}

void sem12(Context& ctx) {
    ctx.prog.push_back(
        {
        3,
        std::to_string(ctx.x),
        ctx.row,
        ctx.pos
        }
    );
}

void sem13(Context& ctx) {
    ctx.prog.push_back(
        {
        static_cast<int>(ctx.name[0]),
        ctx.name,
        ctx.row,
        ctx.pos
        }
    );
}

void sem14(Context& ctx) {
    std::string op = ctx.name + ctx.InputSTR[ctx.i];
    auto it = KeywordMap.find(op);
    if (it != KeywordMap.end()) {
        ctx.prog.push_back(
            {
            it->second,
            op,
            ctx.row,
            ctx.pos
            }
        );
    }
}

void sem15(Context& ctx) {} // Пропуск

std::string loadTextFile(const std::string& filePath) {
    std::ifstream in(filePath);
    if (!in) {
        std::cout << "Не открывается файл: " << filePath << std::endl;
        return std::string();
    }
    std::string output((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    output += static_cast<char>('Ʇ'); // 0xE1 = Ʇ
    return output;
}

bool loadLexemeDescriptions(std::unordered_map<int, std::string>& g_lexemeDescriptions, const std::string &filePath) {
    std::ifstream in(filePath);
    if (!in) {
        std::cerr << "Не открывается файл: " << filePath << std::endl;
        return false;
    }
    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string content = buffer.str();
    nlohmann::json j;   
    try {
        j = nlohmann::json::parse(content);
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Ошибка разбора JSON: " << e.what() << std::endl;
        return false;
    }
    g_lexemeDescriptions.clear();

    for (auto it = j.begin(); it != j.end(); ++it) {
        int key = std::stoi(it.key());
        std::string value = it.value().get<std::string>();
        g_lexemeDescriptions[key] = value;
    }

    return true;
}

void dumpLexemeDescriptions(const std::unordered_map<int, std::string>& g_lexemeDescriptions) {
    for (const auto& pair : g_lexemeDescriptions) std::cout << pair.first << " : " << pair.second << std::endl;
}
void outputResult(const std::vector<Lexeme>& output, const std::unordered_map<int, std::string>& g_lexemeDescriptions) {
    std::cout << "\n============ Результаты ============" << std::endl;
    std::cout << std::setw(4) << "Номер"
        << " | " << std::setw(12) << "Значение"
        << " | " << std::setw(12) << "Позиция"
        << " | " << std::setw(30) << "Описание" << "\n";
    std::cout << std::string(4 + 3 + 12 + 3 + 12 + 3 + 30, '-') << "\n";
    for (auto& lx : output) {
        auto it = g_lexemeDescriptions.find(lx.num);
        const std::string& desc = (it != g_lexemeDescriptions.end() ? it->second : "<Нет описания>");

        std::cout << std::setw(4) << lx.num
            << " | " << std::setw(12) << std::quoted(lx.value)
            << " | " << std::setw(12)
            << ("(" + std::to_string(lx.row) + "," + std::to_string(lx.pos) + ")")
            << " | "
            // переключаемся на выравнивание влево
            << std::left
            << std::setw(40) << desc
            << std::right
            << "\n";
    }
}
