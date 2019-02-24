#ifndef MAIN_H
#define MAIN_H

#include <sstream>
#include <string>
#include <memory>
#include <map>

struct Lexer {
    enum Token {
        No,        /*!< Нет токена */
        Error,     /*!< Неизвестный токен - ошибка */
        Operation, /*!< Операция */
        Variable,  /*!< Переменная */
        Number,    /*!< Число */
    };

    std::istringstream input;
    std::string text;
    Token token;

    explicit Lexer() noexcept;
    explicit Lexer(const char *str) noexcept;
    explicit Lexer(const std::string &str) noexcept;

    Token getNextToken();
    void setInput(const char *str) noexcept;
    void setInput(const std::string &str) noexcept;
};

struct Expression {
    Expression() = default;
    virtual ~Expression() = default;
};

struct Number: Expression {
    int value;
    Number(int aValue)
        : value(aValue)
        {}
};

struct Operation: Expression {
    int operation;
    std::unique_ptr<Expression> lhs;
    std::unique_ptr<Expression> rhs;

    Operation(int aOperation, std::unique_ptr<Expression> aLhs, std::unique_ptr<Expression> aRhs)
        : operation(aOperation)
        , lhs(std::move(aLhs))
        , rhs(std::move(aRhs))
        {}
};

struct Variable: Expression {
    std::string name;

    Variable(const std::string &aName)
        : name(aName)
        {}
};

struct Parser {
    std::string input;
    Lexer lexer;
    std::map<int, int> priority;

    Parser(const char *str);
    Parser(const std::string &str);

    std::unique_ptr<Expression> parseOperand();
    std::unique_ptr<Expression> parseOperation(std::unique_ptr<Expression> lhs);
    std::unique_ptr<Expression> parseOperationTail(std::unique_ptr<Expression> lhs, int op);
    std::unique_ptr<Expression> parse();
};

struct Solver {
    static bool hasVariable(const std::unique_ptr<Expression> &branch);
    static bool simplify(std::unique_ptr<Expression> &ast);
    static bool solve(std::unique_ptr<Expression> &ast, int &result);
    static bool rebuild(std::unique_ptr<Expression> &lhs, std::unique_ptr<Expression> &rhs);

    static bool is_variable(const std::unique_ptr<Expression> &expr);
};

#endif /* MAIN_H */
