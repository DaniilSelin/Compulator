#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <any>

using namespace std;

struct Lexeme{
    int num;
    string value;
    int row;
    int pos;
};

void PrintInt(std::any arg) {
    int value = std::any_cast<int>(arg);
    std::cout << "Integer: " << value << std::endl;
}

void PrintString(std::any arg) {
    std::string value = std::any_cast<std::string>(arg);
    std::cout << "String: " << value << std::endl;
}

std::map<std::string, std::function<void(std::any)>> myMap;

int main() {
    // Передаем функции по имени
    myMap["printInt"] = PrintInt;
    myMap["printString"] = PrintString;

    // Вызов
    myMap["printInt"](42);
    myMap["printString"](std::string("Hello"));

    return 0;
}