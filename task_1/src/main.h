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

    Lexer(const std::string &str) noexcept;

    Token getNextToken();
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
    Lexer lexer;
    std::map<int, int> priority;

    Parser(const std::string &str)
        : lexer(str)
        , priority{{'+', 1},
                   {'-', 2},
                   {'*', 3},
                   {'/', 4},
                   {'=', 5}}
        {}

    std::unique_ptr<Expression> parseOperand();
    std::unique_ptr<Expression> parseOperation(std::unique_ptr<Expression> lhs);
    std::unique_ptr<Expression> parseOperationTail(std::unique_ptr<Expression> lhs, int op);
    std::unique_ptr<Expression> parse();
};

#endif /* MAIN_H */
