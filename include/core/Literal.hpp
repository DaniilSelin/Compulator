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
    virtual std::any get() const { return evaluate(); }
    virtual void set(ValueVariant) { throw std::runtime_error("Set not supported"); }
    virtual std::shared_ptr<Literal> getIdx(const Literal&) const { throw std::runtime_error("getIdx not supported"); }
    virtual std::shared_ptr<Literal> getIdxDirect(int) const { throw std::runtime_error("getIdx not supported"); }

    // Арифметика
    virtual std::shared_ptr<Literal> add(const Literal&) const { throw std::runtime_error("Add not supported"); }
    virtual std::shared_ptr<Literal> sub(const Literal&) const { throw std::runtime_error("Sub not supported"); }
    virtual std::shared_ptr<Literal> mul(const Literal&) const { throw std::runtime_error("Mul not supported"); }
    virtual std::shared_ptr<Literal> div(const Literal&) const { throw std::runtime_error("Div not supported"); }
    virtual std::shared_ptr<Literal> unaryMinus() const { throw std::runtime_error("Div not supported"); }

    // Логика
    virtual std::shared_ptr<Literal> logical_or(const Literal&) const { throw std::runtime_error("Logical OR not supported"); }
    virtual std::shared_ptr<Literal> logical_and(const Literal&) const { throw std::runtime_error("Logical AND not supported"); }
    virtual std::shared_ptr<Literal> logical_not() const { throw std::runtime_error("Logical NOT not supported"); }

    // Сравнения
    virtual std::shared_ptr<Literal> equal(const Literal&) const { throw std::runtime_error("Equality not supported"); }
    virtual std::shared_ptr<Literal> not_equal(const Literal&) const { throw std::runtime_error("Inequality not supported"); }
    virtual std::shared_ptr<Literal> less(const Literal&) const { throw std::runtime_error("Less than not supported"); }
    virtual std::shared_ptr<Literal> less_equal(const Literal&) const { throw std::runtime_error("Less equal not supported"); }
    virtual std::shared_ptr<Literal> greater(const Literal&) const { throw std::runtime_error("Greater than not supported"); }
    virtual std::shared_ptr<Literal> greater_equal(const Literal&) const { throw std::runtime_error("Greater equal not supported"); }

    // Печать
    virtual void printLex(std::ostream &os) const = 0;
    virtual void print(std::ostream &os) const = 0;
    virtual void scan(std::istream &is) { throw std::runtime_error("Scan not supported"); }
};

using PtrLiteral = std::shared_ptr<Literal>;

class Mark : public Literal {
    int value_;
public:
    explicit Mark(int v) : Literal(LiteralType::Mark), value_(v) {}
    ValueVariant evaluate() const override { return value_; }
    std::any get() const override { return value_; }
    void set(ValueVariant v) override {
        value_ = std::get<int>(v);
    }
    void printLex(std::ostream &os) const override { os << "Mark(" << value_ << ")"; }
    void print(std::ostream &os) const override { throw std::runtime_error("Print not supported"); }
};

class Operation : public Literal {
    int value_;
public:
    explicit Operation(int v) : Literal(LiteralType::Operation), value_(v) {}
    ValueVariant evaluate() const override { return value_; }
    std::any get() const override { return value_; }
    void set(ValueVariant v) override {
        value_ = std::get<int>(v);
    }
    void printLex(std::ostream &os) const override { os << "Operation(" << value_ << ")"; }
    void print(std::ostream &os) const override { throw std::runtime_error("Print not supported"); }
};

class Bool : public Literal {
    bool value_;
public:
    explicit Bool(bool v) : Literal(LiteralType::Bool), value_(v) {}

    ValueVariant evaluate() const override { return value_; }
    std::any get() const override { return value_; }

    void printLex(std::ostream &os) const override {
        os << (value_ ? "Bool(1)" : "Bool(0)");
    }

    void print(std::ostream &os) const override {
        os << (value_ ? "1" : "0");
    }

    void scan(std::istream &is) override { is >> value_; }

    std::shared_ptr<Literal> add(const Literal& other) const override {
        throw std::runtime_error("Addition not supported for Bool");
    }

    std::shared_ptr<Literal> sub(const Literal& other) const override {
        throw std::runtime_error("Subtraction not supported for Bool");
    }

    std::shared_ptr<Literal> mul(const Literal& other) const override {
        throw std::runtime_error("Multiplication not supported for Bool");
    }

    std::shared_ptr<Literal> div(const Literal& other) const override {
        throw std::runtime_error("Division not supported for Bool");
    }

    std::shared_ptr<Literal> logical_and(const Literal& other) const {
        bool otherVal = std::get<bool>(other.evaluate());
        return std::make_shared<Bool>(value_ && otherVal);
    }

    std::shared_ptr<Literal> logical_or(const Literal& other) const {
        bool otherVal = std::get<bool>(other.evaluate());
        return std::make_shared<Bool>(value_ || otherVal);
    }

    std::shared_ptr<Literal> logical_not() const {
        return std::make_shared<Bool>(!value_);
    }

    std::shared_ptr<Literal> equal(const Literal& other) const {
        return std::make_shared<Bool>(value_ == std::get<bool>(other.evaluate()));
    }

    std::shared_ptr<Literal> not_equal(const Literal& other) const {
        return std::make_shared<Bool>(value_ != std::get<bool>(other.evaluate()));
    }
};

class ConstantReal : public Literal {
    double value_;
public:
    explicit ConstantReal(double v) : Literal(LiteralType::ConstantReal), value_(v) {}
    ValueVariant evaluate() const override { return value_; }
    std::any get() const override { return value_; }
    void set(ValueVariant v) override {
        if (!std::holds_alternative<double>(v)) throw std::runtime_error("Type mismatch");
        value_ = std::get<double>(v);
    }

    std::shared_ptr<Literal> add(const Literal& other) const override {
        auto v2var = other.evaluate();
        double result = value_ + (std::holds_alternative<double>(v2var) ? std::get<double>(v2var) : static_cast<double>(std::get<int>(v2var)));
        return std::make_shared<ConstantReal>(result);
    }

    std::shared_ptr<Literal> mul(const Literal& other) const override {
        auto v2var = other.evaluate();
        double result = value_ * (std::holds_alternative<double>(v2var) ? std::get<double>(v2var) : static_cast<double>(std::get<int>(v2var)));
        return std::make_shared<ConstantReal>(result);
    }

    std::shared_ptr<Literal> sub(const Literal& other) const override {
        auto v2var = other.evaluate();
        double result = value_ - (std::holds_alternative<double>(v2var) ? std::get<double>(v2var) : static_cast<double>(std::get<int>(v2var)));
        return std::make_shared<ConstantReal>(result);
    }

    std::shared_ptr<Literal> div(const Literal& other) const override {
        auto v2var = other.evaluate();
        double result = value_ / (std::holds_alternative<double>(v2var) ? std::get<double>(v2var) : static_cast<double>(std::get<int>(v2var)));
        return std::make_shared<ConstantReal>(result);
    }

    std::shared_ptr<Literal> unaryMinus() const override {
        return std::make_shared<ConstantReal>(value_ * (-1));
    }

    std::shared_ptr<Literal> equal(const Literal& other) const override {
        auto v2var = other.evaluate();
        bool result = value_ == (std::holds_alternative<double>(v2var) ? std::get<double>(v2var) : static_cast<double>(std::get<int>(v2var)));
        return std::make_shared<Bool>(result);
    }

    std::shared_ptr<Literal> not_equal(const Literal& other) const override {
        auto v2var = other.evaluate();
        bool result = value_ != (std::holds_alternative<double>(v2var) ? std::get<double>(v2var) : static_cast<double>(std::get<int>(v2var)));
        return std::make_shared<Bool>(result);
    }

    std::shared_ptr<Literal> less(const Literal& other) const override {
        auto v2var = other.evaluate();
        bool result = value_ < (std::holds_alternative<double>(v2var) ? std::get<double>(v2var) : static_cast<double>(std::get<int>(v2var)));
        return std::make_shared<Bool>(result);
    }

    std::shared_ptr<Literal> less_equal(const Literal& other) const override {
        auto v2var = other.evaluate();
        bool result = value_ <= (std::holds_alternative<double>(v2var) ? std::get<double>(v2var) : static_cast<double>(std::get<int>(v2var)));
        return std::make_shared<Bool>(result);
    }

    std::shared_ptr<Literal> greater(const Literal& other) const override {
        auto v2var = other.evaluate();
        bool result = value_ > (std::holds_alternative<double>(v2var) ? std::get<double>(v2var) : static_cast<double>(std::get<int>(v2var)));
        return std::make_shared<Bool>(result);
    }

    std::shared_ptr<Literal> greater_equal(const Literal& other) const override {
        auto v2var = other.evaluate();
        bool result = value_ >= (std::holds_alternative<double>(v2var) ? std::get<double>(v2var) : static_cast<double>(std::get<int>(v2var)));
        return std::make_shared<Bool>(result);
    }

    void printLex(std::ostream &os) const override { os << "constR(" << value_ << ")"; }
    void print(std::ostream &os) const override { os << value_; }
    void scan(std::istream &is) override { is >> value_; }
};

class ConstantInt : public Literal {
    int value_;
public:
    explicit ConstantInt(int v) : Literal(LiteralType::ConstantInt), value_(v) {}
    ValueVariant evaluate() const override { return value_; }
    std::any get() const override { return value_; }
    void set(ValueVariant v) override {
        if (!std::holds_alternative<int>(v)) throw std::runtime_error("Type mismatch");
        value_ = std::get<int>(v);
    }

    std::shared_ptr<Literal> add(const Literal& other) const override {
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            // int + int
            int result = value_ + std::get<int>(v2);
            return std::make_shared<ConstantInt>(result);
        } else {
            // int + real
            double result = static_cast<double>(value_) + std::get<double>(v2);
            return std::make_shared<ConstantReal>(result);
        }    
    }

    std::shared_ptr<Literal> mul(const Literal& other) const override {
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<ConstantInt>(value_ * std::get<int>(v2));
        } else {
            double result = static_cast<double>(value_) * std::get<double>(v2);
            return std::make_shared<ConstantReal>(result);
        }
    }

    std::shared_ptr<Literal> sub(const Literal& other) const override {
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            int result = value_ - std::get<int>(v2);
            return std::make_shared<ConstantInt>(result);
        } else {
            double result = static_cast<double>(value_) - std::get<double>(v2);
            return std::make_shared<ConstantReal>(result);
        }
    }

    std::shared_ptr<Literal> div(const Literal& other) const override {
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<ConstantInt>(value_ / std::get<int>(v2));
        } else {
            double result = static_cast<double>(value_) / std::get<double>(v2);
            return std::make_shared<ConstantReal>(result);
        }
    }

    std::shared_ptr<Literal> unaryMinus() const override {
        return std::make_shared<ConstantInt>(value_ * (-1));
    }

    std::shared_ptr<Literal> equal(const Literal& other) const override {
        auto v2 = other.evaluate();

        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<Bool>(value_ == std::get<int>(v2));
        } else {
            double rhs = std::get<double>(v2);
            return std::make_shared<Bool>(static_cast<double>(value_) == rhs);
        }
    }

    std::shared_ptr<Literal> not_equal(const Literal& other) const override {
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<Bool>(value_ != std::get<int>(v2));
        } else {
            double rhs = std::get<double>(v2);
            return std::make_shared<Bool>(static_cast<double>(value_) != rhs);
        }
    }

    std::shared_ptr<Literal> less(const Literal& other) const override {
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<Bool>(value_ < std::get<int>(v2));
        } else {
            double rhs = std::get<double>(v2);
            return std::make_shared<Bool>(static_cast<double>(value_) < rhs);
        }
    }

    std::shared_ptr<Literal> less_equal(const Literal& other) const override {
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<Bool>(value_ <= std::get<int>(v2));
        } else {
            double rhs = std::get<double>(v2);
            return std::make_shared<Bool>(static_cast<double>(value_) <= rhs);
        }
    }

    std::shared_ptr<Literal> greater(const Literal& other) const override {
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<Bool>(value_ > std::get<int>(v2));
        } else {
            double rhs = std::get<double>(v2);
            return std::make_shared<Bool>(static_cast<double>(value_) >rhs);
        }
    }

    std::shared_ptr<Literal> greater_equal(const Literal& other) const override {
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<Bool>(value_ >= std::get<int>(v2));
        } else {
            double rhs = std::get<double>(v2);
            return std::make_shared<Bool>(static_cast<double>(value_) >= rhs);
        }
    }

    void printLex(std::ostream &os) const override { os << "constI(" << value_ << ")"; }
    void print(std::ostream &os) const override { os << value_; }
    void scan(std::istream &is) override { is >> value_; }
};

class IndirectIdxReal : public Literal {
    std::uintptr_t value_;
public:
    explicit IndirectIdxReal(std::uintptr_t addr)
      : Literal(LiteralType::IndirectIdxReal), value_(addr) {}

    ValueVariant evaluate() const override {
        return *reinterpret_cast<double*>(value_);
    }
    std::any get() const override { return value_; }

    void set(ValueVariant v) override {
        double* ptr = reinterpret_cast<double*>(value_);

        if (std::holds_alternative<int>(v)) {
            *ptr = static_cast<double>(std::get<int>(v));
        } else {
            *ptr = std::get<double>(v);
        }
    }

    std::shared_ptr<Literal> add(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<ConstantReal>(lhs + rhs);
    }

    std::shared_ptr<Literal> mul(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<ConstantReal>(lhs * rhs);
    }

    std::shared_ptr<Literal> sub(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<ConstantReal>(lhs - rhs);
    }

    std::shared_ptr<Literal> div(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<ConstantReal>(lhs / rhs);
    }

    std::shared_ptr<Literal> unaryMinus() const override {
        double lhs = *reinterpret_cast<double*>(value_);
        return std::make_shared<ConstantReal>(lhs * (-1));
    }

    std::shared_ptr<Literal> equal(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<Bool>(lhs == rhs);
    }

    std::shared_ptr<Literal> not_equal(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<Bool>(lhs != rhs);
    }

    std::shared_ptr<Literal> less(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<Bool>(lhs < rhs);
    }

    std::shared_ptr<Literal> less_equal(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<Bool>(lhs <= rhs);
    }

    std::shared_ptr<Literal> greater(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<Bool>(lhs > rhs);
    }

    std::shared_ptr<Literal> greater_equal(const Literal& other) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<Bool>(lhs >= rhs);
    }

    void printLex(std::ostream &os) const override {
        os << "idxR(addr=" << reinterpret_cast<void*>(value_) << ")";
    }
    void print(std::ostream &os) const override {
        double lhs = *reinterpret_cast<double*>(value_);
        os << lhs;
    }
    void scan(std::istream &is) override { 
        double* ptr = reinterpret_cast<double*>(value_);
        is >> *ptr; 
    }
};

class IndirectIdxInt : public Literal {
    std::uintptr_t value_;
public:
    explicit IndirectIdxInt(std::uintptr_t addr)
      : Literal(LiteralType::IndirectIdxInt), value_(addr) {}

    ValueVariant evaluate() const override {
        return *reinterpret_cast<int*>(value_);
    }
    std::any get() const override { return value_; }

    void set(ValueVariant v) override {
        int* ptr = reinterpret_cast<int*>(value_);

        if (std::holds_alternative<double>(v)) {
            *ptr = static_cast<int>(std::get<double>(v));
        } else {
            *ptr = std::get<int>(v);
        }
    }

    std::shared_ptr<Literal> add(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<ConstantInt>(lhs + std::get<int>(v2));
        } else {
            double result = static_cast<double>(lhs) + std::get<double>(v2);
            return std::make_shared<ConstantReal>(result);
        }
    }

    std::shared_ptr<Literal> mul(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<ConstantInt>(lhs * std::get<int>(v2));
        } else {
            double result = static_cast<double>(lhs) * std::get<double>(v2);
            return std::make_shared<ConstantReal>(result);
        }
    }

    std::shared_ptr<Literal> sub(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<ConstantInt>(lhs - std::get<int>(v2));
        } else {
            double result = static_cast<double>(lhs) - std::get<double>(v2);
            return std::make_shared<ConstantReal>(result);
        }
    }

    std::shared_ptr<Literal> div(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<ConstantInt>(lhs / std::get<int>(v2));
        } else {
            double result = static_cast<double>(lhs) / std::get<double>(v2);
            return std::make_shared<ConstantReal>(result);
        }
    }

    std::shared_ptr<Literal> unaryMinus() const override {
        int lhs = *reinterpret_cast<int*>(value_);
        return std::make_shared<ConstantInt>(lhs * (-1));
    }

    std::shared_ptr<Literal> equal(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<Bool>(lhs == std::get<int>(v2));
        } else {
            bool result = static_cast<double>(lhs) == std::get<double>(v2);
            return std::make_shared<Bool>(result);
        }
    }

    std::shared_ptr<Literal> not_equal(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<Bool>(lhs != std::get<int>(v2));
        } else {
            bool result = static_cast<double>(lhs) != std::get<double>(v2);
            return std::make_shared<Bool>(result);
        }
    }

    std::shared_ptr<Literal> less(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<Bool>(lhs < std::get<int>(v2));
        } else {
            bool result = static_cast<double>(lhs) < std::get<double>(v2);
            return std::make_shared<Bool>(result);
        }
    }

    std::shared_ptr<Literal> less_equal(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<Bool>(lhs <= std::get<int>(v2));
        } else {
            bool result = static_cast<double>(lhs) <= std::get<double>(v2);
            return std::make_shared<Bool>(result);
        }
    }

    std::shared_ptr<Literal> greater(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<Bool>(lhs > std::get<int>(v2));
        } else {
            bool result = static_cast<double>(lhs) > std::get<double>(v2);
            return std::make_shared<Bool>(result);
        }
    }

    std::shared_ptr<Literal> greater_equal(const Literal& other) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<Bool>(lhs >= std::get<int>(v2));
        } else {
            bool result = static_cast<double>(lhs) >= std::get<double>(v2);
            return std::make_shared<Bool>(result);
        }
    }

    void printLex(std::ostream &os) const override {
        os << "idxI(addr=" << reinterpret_cast<void*>(value_) << ")";
    }
    void print(std::ostream &os) const override {
        int lhs = *reinterpret_cast<int*>(value_);
        os << lhs;
    }
    void scan(std::istream &is) override { 
        int* ptr = reinterpret_cast<int*>(value_);
        is >> *ptr; 
    }
};

class LinkReal : public Literal {
    std::string name_;
public:
    explicit LinkReal(std::string n)
      : Literal(LiteralType::LinkReal), name_(std::move(n)) {}

    ValueVariant evaluate() const override {
        auto it = VarMap.find(name_);
        double v = *reinterpret_cast<double*>(it->second.addr);
        return v;
    }
    std::any get() const override { return name_; }

    std::shared_ptr<Literal> getIdx(const Literal& other) const override { 
        auto it = VarMap.find(name_);
        int sizeT = it->second.size;
        double* basePtr = reinterpret_cast<double*>(it->second.addr);

        auto idxVar = other.evaluate();
        int idx;
        if (std::holds_alternative<int>(idxVar)) {
            idx = std::get<int>(idxVar);
        } else {
            throw std::runtime_error("Index must be integer");
        }

        if (idx < 0 || idx >= sizeT) {
            throw std::out_of_range("Index out of range ");
        }

        double* elemPtr = basePtr + idx;
        return std::make_shared<IndirectIdxReal>(
            reinterpret_cast<std::uintptr_t>(elemPtr)
        );
    }

    std::shared_ptr<Literal> getIdxDirect(int idx) const override { 
        auto it = VarMap.find(name_);
        int sizeT = it->second.size;
        double* basePtr = reinterpret_cast<double*>(it->second.addr);
        double* elemPtr = basePtr + idx;
        return std::make_shared<IndirectIdxReal>(
            reinterpret_cast<std::uintptr_t>(elemPtr)
        );
    }

    void set(ValueVariant v) override {
        auto it = VarMap.find(name_);
        double* ptr = reinterpret_cast<double*>(it->second.addr);

        if (std::holds_alternative<int>(v)) {
            *ptr = static_cast<double>(std::get<int>(v));
        } else {
            *ptr = std::get<double>(v);
        }
    }

    std::shared_ptr<Literal> add(const Literal& other) const override {
        // real + any → real
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<ConstantReal>(lhs + rhs);
    }

    std::shared_ptr<Literal> mul(const Literal& other) const override {
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<ConstantReal>(lhs * rhs);
    }

    std::shared_ptr<Literal> sub(const Literal& other) const override {
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<ConstantReal>(lhs - rhs);
    }

    std::shared_ptr<Literal> div(const Literal& other) const override {
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<ConstantReal>(lhs / rhs);
    }

    std::shared_ptr<Literal> unaryMinus() const override {
        double lhs = std::get<double>(evaluate());
        return std::make_shared<ConstantReal>(lhs * (-1));
    }

    std::shared_ptr<Literal> equal(const Literal& other) const override {
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<Bool>(lhs == rhs);
    }

    std::shared_ptr<Literal> not_equal(const Literal& other) const override {
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<Bool>(lhs != rhs);
    }

    std::shared_ptr<Literal> less(const Literal& other) const override {
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<Bool>(lhs < rhs);
    }

    std::shared_ptr<Literal> less_equal(const Literal& other) const override {
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<Bool>(lhs <= rhs);
    }

    std::shared_ptr<Literal> greater(const Literal& other) const override {
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<Bool>(lhs > rhs);
    }

    std::shared_ptr<Literal> greater_equal(const Literal& other) const override {
        double lhs = std::get<double>(evaluate());
        auto v2 = other.evaluate();
        double rhs = std::holds_alternative<double>(v2)
                       ? std::get<double>(v2)
                       : static_cast<double>(std::get<int>(v2));
        return std::make_shared<Bool>(lhs >= rhs);
    }

    void printLex(std::ostream &os) const override {
        os << "linkR(" << name_ << ")";
    }
    void print(std::ostream &os) const override {
        auto it = VarMap.find(name_);
        double* ptr = reinterpret_cast<double*>(it->second.addr);
        os << *ptr;
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
    explicit LinkInt(std::string n)
      : Literal(LiteralType::LinkInt), name_(std::move(n)) {}

    ValueVariant evaluate() const override {
        auto it = VarMap.find(name_);
        int v = *reinterpret_cast<int*>(it->second.addr);
        return v;
    }
    std::any get() const override { return name_; }

    std::shared_ptr<Literal> getIdx(const Literal& other) const override { 
        auto it = VarMap.find(name_);
        int sizeT = it->second.size;
        int* basePtr = reinterpret_cast<int*>(it->second.addr);

        auto idxVar = other.evaluate();
        int idx;
        if (std::holds_alternative<int>(idxVar)) {
            idx = std::get<int>(idxVar);
        } else {
            throw std::runtime_error("Index must be integer");
        }

        if (idx < 0 || idx >= sizeT) {
            throw std::out_of_range("Index out of range ");
        }

        int* elemPtr = basePtr + idx;
        return std::make_shared<IndirectIdxInt>(
            reinterpret_cast<std::uintptr_t>(elemPtr)
        );
    }

    std::shared_ptr<Literal> getIdxDirect(int idx) const override { 
        auto it = VarMap.find(name_);
        int sizeT = it->second.size;
        int* basePtr = reinterpret_cast<int*>(it->second.addr);
        int* elemPtr = basePtr + idx;
        return std::make_shared<IndirectIdxInt>(
            reinterpret_cast<std::uintptr_t>(elemPtr)
        );
    }

    void set(ValueVariant v) override {
        auto it = VarMap.find(name_);
        int* ptr = reinterpret_cast<int*>(it->second.addr);

        if (std::holds_alternative<double>(v)) {
            *ptr = static_cast<int>(std::get<double>(v));
        } else {
            *ptr = std::get<int>(v);
        }
    }

    std::shared_ptr<Literal> add(const Literal& other) const override {
        // int + int → ConstantInt, иначе → ConstantReal
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<ConstantInt>(lhs + std::get<int>(v2));
        } else {
            double result = static_cast<double>(lhs) + std::get<double>(v2);
            return std::make_shared<ConstantReal>(result);
        }
    }

    std::shared_ptr<Literal> mul(const Literal& other) const override {
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<ConstantInt>(lhs * std::get<int>(v2));
        } else {
            double result = static_cast<double>(lhs) * std::get<double>(v2);
            return std::make_shared<ConstantReal>(result);
        }
    }

    std::shared_ptr<Literal> sub(const Literal& other) const override {
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<ConstantInt>(lhs - std::get<int>(v2));
        } else {
            double result = static_cast<double>(lhs) - std::get<double>(v2);
            return std::make_shared<ConstantReal>(result);
        }
    }

    std::shared_ptr<Literal> div(const Literal& other) const override {
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<ConstantInt>(lhs / std::get<int>(v2));
        } else {
            double result = static_cast<double>(lhs) / std::get<double>(v2);
            return std::make_shared<ConstantReal>(result);
        }
    }

    std::shared_ptr<Literal> unaryMinus() const override {
        int lhs = std::get<int>(evaluate());
        return std::make_shared<ConstantInt>(lhs * (-1));    
    }

    std::shared_ptr<Literal> equal(const Literal& other) const override {
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<Bool>(lhs == std::get<int>(v2));
        } else {
            bool result = static_cast<double>(lhs) == std::get<double>(v2);
            return std::make_shared<Bool>(result);
        }
    }

    std::shared_ptr<Literal> not_equal(const Literal& other) const override {
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<Bool>(lhs != std::get<int>(v2));
        } else {
            bool result = static_cast<double>(lhs) != std::get<double>(v2);
            return std::make_shared<Bool>(result);
        }
    }

    std::shared_ptr<Literal> less(const Literal& other) const override {
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<Bool>(lhs < std::get<int>(v2));
        } else {
            bool result = static_cast<double>(lhs) < std::get<double>(v2);
            return std::make_shared<Bool>(result);
        }
    }

    std::shared_ptr<Literal> less_equal(const Literal& other) const override {
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<Bool>(lhs <= std::get<int>(v2));
        } else {
            bool result = static_cast<double>(lhs) <= std::get<double>(v2);
            return std::make_shared<Bool>(result);
        }
    }

    std::shared_ptr<Literal> greater(const Literal& other) const override {
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<Bool>(lhs > std::get<int>(v2));
        } else {
            bool result = static_cast<double>(lhs) > std::get<double>(v2);
            return std::make_shared<Bool>(result);
        }
    }

    std::shared_ptr<Literal> greater_equal(const Literal& other) const override {
        int lhs = std::get<int>(evaluate());
        auto v2 = other.evaluate();
        if (std::holds_alternative<int>(v2)) {
            return std::make_shared<Bool>(lhs >= std::get<int>(v2));
        } else {
            bool result = static_cast<double>(lhs) >= std::get<double>(v2);
            return std::make_shared<Bool>(result);
        }
    }

    void printLex(std::ostream &os) const override {
        os << "linkI(" << name_ << ")";
    }
    void print(std::ostream &os) const override {
        auto it = VarMap.find(name_);
        int* ptr = reinterpret_cast<int*>(it->second.addr);
        os << *ptr;
    }
    void scan(std::istream &is) override { 
        auto it = VarMap.find(name_);
        int* ptr = reinterpret_cast<int*>(it->second.addr);
        is >> *ptr; 
    }
};

#endif