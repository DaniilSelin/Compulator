#ifndef LITERAL_HPP
#define LITERAL_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>
#include <memory>
#include <any>

struct VarObject {
    std::uintptr_t addr;
    int size;
    bool isReal;
};

extern std::unordered_map<std::string, VarObject> VarMap;

enum class LiteralType {
    LinkInt,
    LinkReal,
    Operation,
    Mark,
    ConstantInt,
    ConstantReal,
    IndirectIdxInt,
    IndirectIdxReal,
    Bool,
};

class Literal {
protected:
    LiteralType type_;
public:
    using ValueVariant = std::variant<int, double, std::uintptr_t, bool>;

    explicit Literal(LiteralType t) : type_(t) {}
    virtual ~Literal() = default;

    LiteralType type() const { return type_; }

    virtual ValueVariant evaluate() const = 0;
    virtual std::any get() const = 0;//{ return evaluate(); }
    virtual void set(ValueVariant) { throw std::runtime_error("Не поддерживается функция установки значения"); }
    virtual std::shared_ptr<Literal> getIdx(const Literal&) const { throw std::runtime_error("Не поддерживается функция получения индекса"); }
    virtual std::shared_ptr<Literal> getIdxDirect(int) const { throw std::runtime_error("Не поддерживается функция получения индекса напрямую"); }

    // Арифметика
    virtual std::shared_ptr<Literal> add(const Literal&) const { throw std::runtime_error("+ не поддерживается"); }
    virtual std::shared_ptr<Literal> sub(const Literal&) const { throw std::runtime_error("- не поддерживается"); }
    virtual std::shared_ptr<Literal> mul(const Literal&) const { throw std::runtime_error("* не поддерживается"); }
    virtual std::shared_ptr<Literal> div(const Literal&) const { throw std::runtime_error("/ не поддерживается"); }
    virtual std::shared_ptr<Literal> unaryMinus() const { throw std::runtime_error("\'-\' не поддерживается"); }

    // Логика
    virtual std::shared_ptr<Literal> logical_or(const Literal&) const { throw std::runtime_error("|| не поддерживается"); }
    virtual std::shared_ptr<Literal> logical_and(const Literal&) const { throw std::runtime_error("&& не поддерживается"); }
    virtual std::shared_ptr<Literal> logical_not() const { throw std::runtime_error("\'!\' не поддерживается"); }

    // Сравнения
    virtual std::shared_ptr<Literal> equal(const Literal&) const { throw std::runtime_error("== не поддерживается"); }
    virtual std::shared_ptr<Literal> not_equal(const Literal&) const { throw std::runtime_error("!= не поддерживается"); }
    virtual std::shared_ptr<Literal> less(const Literal&) const { throw std::runtime_error("< не поддерживается"); }
    virtual std::shared_ptr<Literal> less_equal(const Literal&) const { throw std::runtime_error("<= не поддерживается"); }
    virtual std::shared_ptr<Literal> greater(const Literal&) const { throw std::runtime_error("> не поддерживается"); }
    virtual std::shared_ptr<Literal> greater_equal(const Literal&) const { throw std::runtime_error(">= не поддерживается"); }

    // Печать
    virtual void printLex(std::ostream &os) const = 0;
    virtual void print(std::ostream &os) const = 0;
    virtual void scan(std::istream &is) { throw std::runtime_error("Ввод не поддерживается"); }
};

using PtrLiteral = std::shared_ptr<Literal>;

class Mark : public Literal {
    int value_;
public:
    explicit Mark(int v) : Literal(LiteralType::Mark), value_(v) {}
    ValueVariant evaluate() const override { return value_; }
    std::any get() const override { return value_; }

    void set(ValueVariant v) override { value_ = std::get<int>(v); }
    void printLex(std::ostream &os) const override { os << "Метка [" << value_ << "]" << "\n"; }
    void print(std::ostream &os) const override { throw std::runtime_error("Вывод не поддерживается"); }
};

class Operation : public Literal {
    int value_;
public:
    explicit Operation(int v) : Literal(LiteralType::Operation), value_(v) {}
    ValueVariant evaluate() const override { return value_; }
    std::any get() const override { return value_; }
    void set(ValueVariant v) override { value_ = std::get<int>(v); }
    void printLex(std::ostream &os) const override { os << "Операция [" << value_ << "]" << "\n"; }
    void print(std::ostream &os) const override { throw std::runtime_error("Вывод не поддерживается"); }
};

class Bool : public Literal {
    bool value_;
public:
    explicit Bool(bool v) : Literal(LiteralType::Bool), value_(v) {}
    ValueVariant evaluate() const override { return value_; }
    std::any get() const override { return value_; }

    // std::shared_ptr<Literal> add(const Literal& other) const override {
    //     throw std::runtime_error("Addition not supported for Bool");
    // }

    // std::shared_ptr<Literal> sub(const Literal& other) const override {
    //     throw std::runtime_error("Subtraction not supported for Bool");
    // }

    // std::shared_ptr<Literal> mul(const Literal& other) const override {
    //     throw std::runtime_error("Multiplication not supported for Bool");
    // }

    // std::shared_ptr<Literal> div(const Literal& other) const override {
    //     throw std::runtime_error("Division not supported for Bool");
    // }

    std::shared_ptr<Literal> logical_and(const Literal& other) const { return std::make_shared<Bool>(value_ && otherValstd::get<bool>(other.evaluate())); }
    std::shared_ptr<Literal> logical_or(const Literal& other) const { return std::make_shared<Bool>(value_ || std::get<bool>(other.evaluate())); }
    std::shared_ptr<Literal> logical_not() const { return std::make_shared<Bool>(!value_); }
    std::shared_ptr<Literal> equal(const Literal& other) const { return std::make_shared<Bool>(value_ == std::get<bool>(other.evaluate())); }
    std::shared_ptr<Literal> not_equal(const Literal& other) const { return std::make_shared<Bool>(value_ != std::get<bool>(other.evaluate())); }

    void printLex(std::ostream &os) const override { os << (value_ ? "Bool(1)" : "Bool(0)"); }
    void print(std::ostream &os) const override { os << (value_ ? "1" : "0") << "\n"; }
    void scan(std::istream &is) override { is >> value_; }
};

class ConstantReal : public Literal {
    double value_;
public:
    explicit ConstantReal(double v) : Literal(LiteralType::ConstantReal), value_(v) {}
    ValueVariant evaluate() const override { return value_; }
    std::any get() const override { return value_; }
    void set(ValueVariant v) override {
        if (!std::holds_alternative<double>(v)) throw std::runtime_error("Несовпадение типов");
        value_ = std::get<double>(v);
    }

    std::shared_ptr<Literal> add(const Literal& other) const override {
        auto v2var = other.evaluate();
        double result = value_ + (std::holds_alternative<double>(v2var)
                                    ? std::get<double>(v2var)
                                    : static_cast<double>(std::get<int>(v2var))); // Преобразование int->real
        return std::make_shared<ConstantReal>(result);
    }
    std::shared_ptr<Literal> mul(const Literal& other) const override {
        auto v2var = other.evaluate();
        double result = value_ * (std::holds_alternative<double>(v2var)
                                    ? std::get<double>(v2var)
                                    : static_cast<double>(std::get<int>(v2var))); // Преобразование int->real
        return std::make_shared<ConstantReal>(result);
    }
    std::shared_ptr<Literal> sub(const Literal& other) const override {
        auto v2var = other.evaluate();
        double result = value_ - (std::holds_alternative<double>(v2var)
                                    ? std::get<double>(v2var)
                                    : static_cast<double>(std::get<int>(v2var))); // Преобразование int->real
        return std::make_shared<ConstantReal>(result);
    }
    std::shared_ptr<Literal> div(const Literal& other) const override {
        auto v2var = other.evaluate();
        double result = value_ / (std::holds_alternative<double>(v2var)
                                    ? std::get<double>(v2var)
                                    : static_cast<double>(std::get<int>(v2var))); // Преобразование int->real
        return std::make_shared<ConstantReal>(result);
    }
    std::shared_ptr<Literal> unaryMinus() const override { return std::make_shared<ConstantReal>(value_ * (-1)); }
    std::shared_ptr<Literal> equal(const Literal& other) const override {
        auto v2var = other.evaluate();
        bool result = value_ == (std::holds_alternative<double>(v2var)
                                    ? std::get<double>(v2var)
                                    : static_cast<double>(std::get<int>(v2var))); // Преобразование int->real
        return std::make_shared<Bool>(result);
    }
    std::shared_ptr<Literal> not_equal(const Literal& other) const override {
        auto v2var = other.evaluate();
        bool result = value_ != (std::holds_alternative<double>(v2var)
                                    ? std::get<double>(v2var)
                                    : static_cast<double>(std::get<int>(v2var))); // Преобразование int->real
        return std::make_shared<Bool>(result);
    }
    std::shared_ptr<Literal> less(const Literal& other) const override {
        auto v2var = other.evaluate();
        bool result = value_ < (std::holds_alternative<double>(v2var)
                                    ? std::get<double>(v2var)
                                    : static_cast<double>(std::get<int>(v2var))); // Преобразование int->real
        return std::make_shared<Bool>(result);
    }
    std::shared_ptr<Literal> less_equal(const Literal& other) const override {
        auto v2var = other.evaluate();
        bool result = value_ <= (std::holds_alternative<double>(v2var)
                                    ? std::get<double>(v2var)
                                    : static_cast<double>(std::get<int>(v2var))); // Преобразование int->real
        return std::make_shared<Bool>(result);
    }
    std::shared_ptr<Literal> greater(const Literal& other) const override {
        auto v2var = other.evaluate();
        bool result = value_ > (std::holds_alternative<double>(v2var)
                                    ? std::get<double>(v2var)
                                    : static_cast<double>(std::get<int>(v2var))); // Преобразование int->real
        return std::make_shared<Bool>(result);
    }
    std::shared_ptr<Literal> greater_equal(const Literal& other) const override {
        auto v2var = other.evaluate();
        bool result = value_ >= (std::holds_alternative<double>(v2var)
                                    ? std::get<double>(v2var)
                                    : static_cast<double>(std::get<int>(v2var))); // Преобразование int->real
        return std::make_shared<Bool>(result);
    }

    void printLex(std::ostream &os) const override { os << "Вещественная константа [" << value_ << "]"; }
    void print(std::ostream &os) const override { os << value_ << "\n"; }
    void scan(std::istream &is) override { is >> value_; }
};

class ConstantInt : public Literal {
    int value_;
public:
    explicit ConstantInt(int v) : Literal(LiteralType::ConstantInt), value_(v) {}
    ValueVariant evaluate() const override { return value_; }
    std::any get() const override { return value_; }
    void set(ValueVariant v) override {
        if (!std::holds_alternative<int>(v)) throw std::runtime_error("Несовпадение типов");
        value_ = std::get<int>(v);
    }

    std::shared_ptr<Literal> add(const Literal& other) const override {
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<ConstantInt>(value_ + std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<ConstantReal>(static_cast<double>(value_) + std::get<double>(v2));
    }
    std::shared_ptr<Literal> mul(const Literal& other) const override {
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<ConstantInt>(value_ * std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<ConstantReal>(static_cast<double>(value_) * std::get<double>(v2));
    }
    std::shared_ptr<Literal> sub(const Literal& other) const override {
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<ConstantInt>(value_ - std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<ConstantReal>(static_cast<double>(value_) - std::get<double>(v2));
    }
    std::shared_ptr<Literal> div(const Literal& other) const override {
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<ConstantInt>(value_ / std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<ConstantReal>(static_cast<double>(value_) / std::get<double>(v2));
    }
    std::shared_ptr<Literal> unaryMinus() const override { return std::make_shared<ConstantInt>(value_ * (-1)); }
    std::shared_ptr<Literal> equal(const Literal& other) const override {
        auto v2 = other.evaluate();

        if (std::holds_alternative<int>(v2)) return std::make_shared<Bool>(value_ == std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<Bool>(static_cast<double>(value_) == std::get<double>(v2));
    }
    std::shared_ptr<Literal> not_equal(const Literal& other) const override {
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<Bool>(value_ != std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<Bool>(static_cast<double>(value_) != std::get<double>(v2));
    }
    std::shared_ptr<Literal> less(const Literal& other) const override {
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<Bool>(value_ < std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<Bool>(static_cast<double>(value_) < std::get<double>(v2));
    }
    std::shared_ptr<Literal> less_equal(const Literal& other) const override {
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<Bool>(value_ <= std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<Bool>(static_cast<double>(value_) <= std::get<double>(v2));
    }
    std::shared_ptr<Literal> greater(const Literal& other) const override {
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<Bool>(value_ > std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<Bool>(static_cast<double>(value_) > std::get<double>(v2));
    }
    std::shared_ptr<Literal> greater_equal(const Literal& other) const override {
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<Bool>(value_ >= std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<Bool>(static_cast<double>(value_) >= std::get<double>(v2));
    }

    void printLex(std::ostream &os) const override { os << "Константа целого типа [" << value_ << "]"; }
    void print(std::ostream &os) const override { os << value_ << "\n"; }
    void scan(std::istream &is) override { is >> value_; }
};

class IndirectIdxReal : public Literal {
    std::uintptr_t value_;
public:
    explicit IndirectIdxReal(std::uintptr_t addr) : Literal(LiteralType::IndirectIdxReal), value_(addr) {}
    ValueVariant evaluate() const override { return *reinterpret_cast<double*>(value_); }
    std::any get() const override { return value_; }
    void set(ValueVariant v) override {
        double* ptr = reinterpret_cast<double*>(value_);
        if (std::holds_alternative<int>(v)) *ptr = static_cast<double>(std::get<int>(v)); // Int на входе
        else *ptr = std::get<double>(v); // Real на входе
    }

    std::shared_ptr<Literal> add(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<ConstantReal>(lhs + rhs);
    }
    std::shared_ptr<Literal> mul(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<ConstantReal>(lhs * rhs);
    }
    std::shared_ptr<Literal> sub(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<ConstantReal>(lhs - rhs);
    }
    std::shared_ptr<Literal> div(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<ConstantReal>(lhs / rhs);
    }
    std::shared_ptr<Literal> unaryMinus() const override { return std::make_shared<ConstantReal>(*reinterpret_cast<double*>(value_) * (-1)); }
    std::shared_ptr<Literal> equal(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<Bool>(lhs == rhs);
    }
    std::shared_ptr<Literal> not_equal(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<Bool>(lhs != rhs);
    }
    std::shared_ptr<Literal> less(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<Bool>(lhs < rhs);
    }
    std::shared_ptr<Literal> less_equal(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<Bool>(lhs <= rhs);
    }
    std::shared_ptr<Literal> greater(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<Bool>(lhs > rhs);
    }
    std::shared_ptr<Literal> greater_equal(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<Bool>(lhs >= rhs);
    }

    void printLex(std::ostream &os) const override { os << "Ссылка на вещественный массив [Адрес = " << reinterpret_cast<void*>(value_) << "]"; }
    void print(std::ostream &os) const override { os << *reinterpret_cast<double*>(value_) << "\n"; }
    void scan(std::istream &is) override { 
        double* ptr = reinterpret_cast<double*>(value_);
        is >> *ptr; 
    }
};

class IndirectIdxInt : public Literal {
    std::uintptr_t value_;
public:
    explicit IndirectIdxInt(std::uintptr_t addr) : Literal(LiteralType::IndirectIdxInt), value_(addr) {}
    ValueVariant evaluate() const override { return *reinterpret_cast<int*>(value_); }
    std::any get() const override { return value_; }
    void set(ValueVariant v) override {
        int* ptr = reinterpret_cast<int*>(value_);
        if (std::holds_alternative<double>(v)) *ptr = static_cast<int>(std::get<double>(v));
        else *ptr = std::get<int>(v);
    }

    std::shared_ptr<Literal> add(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<ConstantInt>(lhs + std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<ConstantReal>(static_cast<double>(lhs) + std::get<double>(v2));
    }
    std::shared_ptr<Literal> mul(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<ConstantInt>(lhs * std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<ConstantReal>(static_cast<double>(lhs) * std::get<double>(v2));
    }
    std::shared_ptr<Literal> sub(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2))return std::make_shared<ConstantInt>(lhs - std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<ConstantReal>(static_cast<double>(lhs) - std::get<double>(v2));
    }
    std::shared_ptr<Literal> div(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<ConstantInt>(lhs / std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<ConstantReal>(static_cast<double>(lhs) / std::get<double>(v2));
    }
    std::shared_ptr<Literal> unaryMinus() const override { return std::make_shared<ConstantInt>(*reinterpret_cast<int*>(value_) * (-1)); }
    std::shared_ptr<Literal> equal(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<Bool>(lhs == std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<Bool>(static_cast<double>(lhs) == std::get<double>(v2));
    }
    std::shared_ptr<Literal> not_equal(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<Bool>(lhs != std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<Bool>(static_cast<double>(lhs) != std::get<double>(v2));
    }
    std::shared_ptr<Literal> less(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<Bool>(lhs < std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<Bool>(static_cast<double>(lhs) < std::get<double>(v2));
    }
    std::shared_ptr<Literal> less_equal(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<Bool>(lhs <= std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<Bool>(static_cast<double>(lhs) <= std::get<double>(v2));
    }
    std::shared_ptr<Literal> greater(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<Bool>(lhs > std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<Bool>(static_cast<double>(lhs) > std::get<double>(v2));
    }
    std::shared_ptr<Literal> greater_equal(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<Bool>(lhs >= std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<Bool>(static_cast<double>(lhs) >= std::get<double>(v2));
    }

    void printLex(std::ostream &os) const override { os << "Ссылка на целочисленный массив [Адрес = " << reinterpret_cast<void*>(value_) << "]"; }
    void print(std::ostream &os) const override { os << *reinterpret_cast<int*>(value_) << "\n"; }
    void scan(std::istream &is) override { 
        int* ptr = reinterpret_cast<int*>(value_);
        is >> *ptr; 
    }
};

class LinkReal : public Literal {
    std::string name_;
public:
    explicit LinkReal(std::string n) : Literal(LiteralType::LinkReal), name_(std::move(n)) {}
    ValueVariant evaluate() const override {
        auto it = VarMap.find(name_);
        double v = *reinterpret_cast<double*>(it->second.addr);
        return v;
    }
    std::any get() const override { return name_; }
    void set(ValueVariant v) override {
        auto it = VarMap.find(name_);
        double* ptr = reinterpret_cast<double*>(it->second.addr);
        if (std::holds_alternative<int>(v)) *ptr = static_cast<double>(std::get<int>(v));
        else *ptr = std::get<double>(v);
    }

    std::shared_ptr<Literal> getIdx(const Literal& other) const override { 
        auto it = VarMap.find(name_);
        int sizeT = it->second.size;
        double* basePtr = reinterpret_cast<double*>(it->second.addr);
        auto idxVar = other.evaluate();
        int idx;
        if (std::holds_alternative<int>(idxVar)) idx = std::get<int>(idxVar);
        else throw std::runtime_error("Индекс обязан быть целочисленным");
        if (idx < 0 || idx >= sizeT) throw std::out_of_range("Индекс выходит за границы массива");
        double* elemPtr = basePtr + idx;
        return std::make_shared<IndirectIdxReal>(reinterpret_cast<std::uintptr_t>(elemPtr));
    }
    std::shared_ptr<Literal> getIdxDirect(int idx) const override { 
        auto it = VarMap.find(name_);
        double* basePtr = reinterpret_cast<double*>(it->second.addr);
        double* elemPtr = basePtr + idx;
        return std::make_shared<IndirectIdxReal>(reinterpret_cast<std::uintptr_t>(elemPtr));
    }
    std::shared_ptr<Literal> add(const Literal& other) const override {
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<ConstantReal>(lhs + rhs);
    }
    std::shared_ptr<Literal> mul(const Literal& other) const override {
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<ConstantReal>(lhs * rhs);
    }
    std::shared_ptr<Literal> sub(const Literal& other) const override {
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<ConstantReal>(lhs - rhs);
    }
    std::shared_ptr<Literal> div(const Literal& other) const override {
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<ConstantReal>(lhs / rhs);
    }
    std::shared_ptr<Literal> unaryMinus() const override { return std::make_shared<ConstantReal>(std::get<double>(evaluate()) * (-1)); }
    std::shared_ptr<Literal> equal(const Literal& other) const override {
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<Bool>(lhs == rhs);
    }
    std::shared_ptr<Literal> not_equal(const Literal& other) const override {
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<Bool>(lhs != rhs);
    }
    std::shared_ptr<Literal> less(const Literal& other) const override {
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<Bool>(lhs < rhs);
    }
    std::shared_ptr<Literal> less_equal(const Literal& other) const override {
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<Bool>(lhs <= rhs);
    }
    std::shared_ptr<Literal> greater(const Literal& other) const override {
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<Bool>(lhs > rhs);
    }
    std::shared_ptr<Literal> greater_equal(const Literal& other) const override {
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2)); // Преобразование int->real
        return std::make_shared<Bool>(lhs >= rhs);
    }

    void printLex(std::ostream &os) const override { os << "Ссылка на вещественную переменную [" << name_ << "]"; }
    void print(std::ostream &os) const override {
        auto it = VarMap.find(name_);
        double* ptr = reinterpret_cast<double*>(it->second.addr);
        os << *ptr << "\n";
    }
    void scan(std::istream &is) override { 
        auto it = VarMap.find(name_);
        double* ptr = reinterpret_cast<double*>(it->second.addr);
        is >> *ptr; 
    }
};

class LinkInt : public Literal {
    std::string name_;
public:
    explicit LinkInt(std::string n) : Literal(LiteralType::LinkInt), name_(std::move(n)) {}
    ValueVariant evaluate() const override {
        auto it = VarMap.find(name_);
        int v = *reinterpret_cast<int*>(it->second.addr);
        return v;
    }
    std::any get() const override { return name_; }
    void set(ValueVariant v) override {
        auto it = VarMap.find(name_);
        int* ptr = reinterpret_cast<int*>(it->second.addr);
        if (std::holds_alternative<double>(v)) *ptr = static_cast<int>(std::get<double>(v));
        else *ptr = std::get<int>(v);
    }

    std::shared_ptr<Literal> getIdx(const Literal& other) const override { 
        auto it = VarMap.find(name_);
        int sizeT = it->second.size;
        int* basePtr = reinterpret_cast<int*>(it->second.addr);
        auto idxVar = other.evaluate();
        int idx;
        if (std::holds_alternative<int>(idxVar))idx = std::get<int>(idxVar);
        else throw std::runtime_error("Индекс обязан быть целочисленным");
        if (idx < 0 || idx >= sizeT) throw std::out_of_range("Индекс выходит за границы массива");
        int* elemPtr = basePtr + idx;
        return std::make_shared<IndirectIdxInt>(reinterpret_cast<std::uintptr_t>(elemPtr));
    }
    std::shared_ptr<Literal> getIdxDirect(int idx) const override { 
        auto it = VarMap.find(name_);
        int* basePtr = reinterpret_cast<int*>(it->second.addr);
        int* elemPtr = basePtr + idx;
        return std::make_shared<IndirectIdxInt>(reinterpret_cast<std::uintptr_t>(elemPtr));
    }
    std::shared_ptr<Literal> add(const Literal& other) const override {
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<ConstantInt>(lhs + std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<ConstantReal>(static_cast<double>(lhs) + std::get<double>(v2));
    }
    std::shared_ptr<Literal> mul(const Literal& other) const override {
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2))return std::make_shared<ConstantInt>(lhs * std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<ConstantReal>(static_cast<double>(lhs) * std::get<double>(v2));
    }
    std::shared_ptr<Literal> sub(const Literal& other) const override {
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<ConstantInt>(lhs - std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<ConstantReal>(static_cast<double>(lhs) - std::get<double>(v2));
    }
    std::shared_ptr<Literal> div(const Literal& other) const override {
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<ConstantInt>(lhs / std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<ConstantReal>(static_cast<double>(lhs) / std::get<double>(v2));
    }
    std::shared_ptr<Literal> unaryMinus() const override { return std::make_shared<ConstantInt>(std::get<int>(evaluate()) * (-1)); }
    std::shared_ptr<Literal> equal(const Literal& other) const override {
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<Bool>(lhs == std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<Bool>(static_cast<double>(lhs) == std::get<double>(v2));
    }
    std::shared_ptr<Literal> not_equal(const Literal& other) const override {
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<Bool>(lhs != std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<Bool>(static_cast<double>(lhs) != std::get<double>(v2));
    }
    std::shared_ptr<Literal> less(const Literal& other) const override {
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<Bool>(lhs < std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<Bool>(static_cast<double>(lhs) < std::get<double>(v2));
    }
    std::shared_ptr<Literal> less_equal(const Literal& other) const override {
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<Bool>(lhs <= std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<Bool>(static_cast<double>(lhs) <= std::get<double>(v2));
    }
    std::shared_ptr<Literal> greater(const Literal& other) const override {
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<Bool>(lhs > std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<Bool>(static_cast<double>(lhs) > std::get<double>(v2));
    }
    std::shared_ptr<Literal> greater_equal(const Literal& other) const override {
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) return std::make_shared<Bool>(lhs >= std::get<int>(v2));
        // Преобразование int->real при int и real
        else return std::make_shared<Bool>(static_cast<double>(lhs) >= std::get<double>(v2));
    }

    void printLex(std::ostream &os) const override { os << "Ссылка на целочисленную переменную [" << name_ << "]"; }
    void print(std::ostream &os) const override {
        auto it = VarMap.find(name_);
        int* ptr = reinterpret_cast<int*>(it->second.addr);
        os << *ptr << "\n";
    }
    void scan(std::istream &is) override { 
        auto it = VarMap.find(name_);
        int* ptr = reinterpret_cast<int*>(it->second.addr);
        is >> *ptr; 
    }
};

#endif
