#ifndef ENUMCLASSES_H
#define ENUMCLASSES_H

enum class typeLexeme {
    link,           // Ссылка
    operation,      // Операция
    mark,           // Метка
    constant        // Константа
};

enum class GenState {
    A,
    P,
    Q,
    B,
    C,
    D,
    S,
    U,
    T,
    V,
    F,
    G,
    H,
    L,
    W,
    M,
    X,
    N,
    O,
    E,
    Z
};

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
    Z,    // Z
    Zs,   // Zs => Z*
};

#endif // ENUMCLASSES_H
