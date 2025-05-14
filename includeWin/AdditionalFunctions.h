#ifndef ADDITIONALFUNCTIONS_H
#define ADDITIONALFUNCTIONS_H
#include "Structs.h"
inline std::string loadTextFile(const std::string& filePath) {
    std::ifstream in(filePath);
    if (!in) {
        std::cout << "Не открывается файл: " << filePath << std::endl;
        return std::string();
    }
    std::string output((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    output += "#";
    return output;
}
// Перегрузка оператора вывода для литерала
inline std::ostream& operator<<(std::ostream& os, const Literal& lit) {
    switch (lit.type) {
        case typeLexeme::link : {
            os << "Ссылка (" << std::any_cast<std::string>(lit.value) << ") ";
            break;
        }
        case typeLexeme::constant : {
            if (lit.value.type() == typeid(int)) os << "Константа int (" << std::any_cast<int>(lit.value) << ") ";
            else os << "Константа double (" << std::any_cast<double>(lit.value) << ") ";
            break;
        }
        case typeLexeme::mark : {
            os << "Метка (" << std::any_cast<int>(lit.value) << ") ";
            break;
        }
        case typeLexeme::operation : {
            os << "Операция (" << std::any_cast<int>(lit.value) << ") ";
            break;
        }
        default : {
            os << "Ошибка. Неправильный тип лексемы.";
            break;
        }
    }
    return os;
}

#endif // ADDITIONALFUNCTIONS_H
