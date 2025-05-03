#include "lexer.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::unordered_map<int, std::string> g_lexemeDescriptions;

bool loadLexemeDescriptions() {
    std::ifstream in(LEXEME_JSON_PATH);
    if (!in) {
        std::cerr << "Cannot open file: " << LEXEME_JSON_PATH << std::endl;
        return false;
    }
    json j;
    try {
        in >> j;
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return false;
    }

    g_lexemeDescriptions.clear();
    for (auto& [key, value] : j.items()) {
        g_lexemeDescriptions[std::stoi(key)] = value.get<std::string>();
    }
    return true;
}

void dumpLexemeDescriptions() {
    for (auto& [id, desc] : g_lexemeDescriptions) {
        std::cout << id << ": " << desc << std::endl;
    }
}