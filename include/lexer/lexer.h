#pragma once

#include <string>
#include <unordered_map>

// Путь к JSON-файлу с описаниями лексем
static const std::string LEXEME_JSON_PATH = "resources/lexemes/lexemesDesc.json";

// Пока не покажем лекс. анализ, описания оставляем, потом уберём
extern std::unordered_map<int, std::string> g_lexemeDescriptions;

// грузит лексемы с описаниеями
bool loadLexemeDescriptions();