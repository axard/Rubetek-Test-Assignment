#include "main.h"
#include <iostream>

// Типы
// =============================================================================

// Реализация Lexer
// -----------------------------------------------------------------------------

Lexer::Lexer() noexcept
    : input()
    , text()
    , token(Lexer::No)
{}

Lexer::Lexer(const char *str) noexcept
    : input(str)
    , text()
    , token(Lexer::No)
{}

Lexer::Lexer(const std::string &str) noexcept
    : input(str)
    , text()
    , token(Lexer::No)
{}

Lexer::Token Lexer::getNextToken()
{
    while (input.peek() == ' ') {
        input.get();
    }

    text.clear();
    int symbol = input.get();

    switch (symbol) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '=':
        text += symbol;
        token = Token::Operation;
        break;

    case 'x':
        text += symbol;
        token = Token::Variable;
        break;

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        text += symbol;
        while (isdigit(input.peek())) {
            text += input.get();
        }
        token = Token::Number;
        break;

    case EOF:
        token = Token::No;
        break;

    default:
        text += symbol;
        token = Token::Error;
        break;
    }

    return token;
}

void Lexer::setInput(const std::string &str) noexcept
{
    input = std::istringstream(str);
    text.clear();
    token = Token::No;
}

// Реализация Parser
// -----------------------------------------------------------------------------

Parser::Parser(const char *str)
    : input(str)
    , lexer()
    , priority{{'+', 1},
               {'-', 2},
               {'*', 3},
               {'/', 4},
               {'=', 5}}
{}

Parser::Parser(const std::string &str)
    : input(str)
    , lexer()
    , priority{{'+', 1},
               {'-', 2},
               {'*', 3},
               {'/', 4},
               {'=', 5}}
{}

std::unique_ptr<Expression> Parser::parseOperand()
{
    auto token = lexer.getNextToken();

    switch (token) {
    case Lexer::Number:
        return std::make_unique<Number>(std::stoi(lexer.text));

    case Lexer::Variable:
        return std::make_unique<Variable>(lexer.text);

    default:
        return nullptr;
    }
}

std::unique_ptr<Expression> Parser::parseOperation(std::unique_ptr<Expression> lhs)
{
    if (lhs == nullptr) {
        lhs = parseOperand();
        if (lhs == nullptr) {
            std::cerr << "Ошибка разбора левой части'" << lexer.text << "'. Ожидалось число или переменная" << std::endl;
            return nullptr;
        }

        // Определяем токен операции
        lexer.getNextToken();
        if (lexer.token == Lexer::No) {
            return lhs;
        }

        if (lexer.token != Lexer::Operation) {
            std::cerr << "Ошибка разбора '" << lexer.text << "'. Ожидалась операция" << std::endl;
            return nullptr;
        }
    }

    int op = lexer.text[0];
    return parseOperationTail(std::move(lhs), op);
}

std::unique_ptr<Expression> Parser::parseOperationTail(std::unique_ptr<Expression> lhs, int op)
{
    // Определяем правый операнд
    auto rhs = parseOperand();
    if (rhs == nullptr) {
        std::cerr << "Ошибка разбора правой части'" << lexer.text << "'. Ожидалось число или переменная" << std::endl;
        return nullptr;
    }

    // Теперь надо проверить приоритет следующей операций, чтобы построить правильное дерево
    // Определяем токен следующей операции
    auto nextOpToken = lexer.getNextToken();

    // Если лексер закончил, то создаём операцию и заканчиваем
    if (nextOpToken == Lexer::No) {
        return std::make_unique<Operation>(op, std::move(lhs), std::move(rhs));
    }

    // Если лексер вернул операцию, то сравниваем её приоритет с текущей операцией
    if (nextOpToken == Lexer::Operation) {
        int nextOp = lexer.text[0];
        if (priority[op] < priority[nextOp]) {
            rhs = parseOperationTail(std::move(rhs), nextOp);
        }

        if (rhs == nullptr) {
            return nullptr;
        }

        return std::make_unique<Operation>(op, std::move(lhs), std::move(rhs));
    }

    std::cerr << "Ошибка разбора '" << lexer.text << "'. Ожидалась операция или конец строки" << std::endl;
    return nullptr;
}

std::unique_ptr<Expression> Parser::parse()
{
    auto pos = input.find('=');
    if (std::string::npos == pos) {
        // Парсим не уравнение
        lexer.setInput(input);
        std::unique_ptr<Expression> lhs = nullptr;
        do {
            lhs = parseOperation((lhs) ? std::move(lhs) : nullptr);
        } while ((lexer.token != Lexer::No) && (lhs != nullptr));
        return lhs;
    } else {
        // Парсим левую сторону уравнения
        auto lhsStr = input.substr(0, pos);
        lexer.setInput(lhsStr);
        std::unique_ptr<Expression> lhs = nullptr;
        do {
            lhs = parseOperation((lhs) ? std::move(lhs) : nullptr);
        } while ((lexer.token != Lexer::No) && (lhs != nullptr));

        // Парсим правую сторону уравнения
        auto rhsStr = input.substr(pos + 1, input.size() - pos - 1);
        lexer.setInput(rhsStr);
        std::unique_ptr<Expression> rhs = nullptr;
        do {
            rhs = parseOperation((rhs) ? std::move(rhs) : nullptr);
        } while ((lexer.token != Lexer::No) && (rhs != nullptr));

        return std::make_unique<Operation>('=', std::move(lhs), std::move(rhs));
    }
}

// Реализация Solver
// -----------------------------------------------------------------------------

bool Solver::hasVariable(const std::unique_ptr<Expression> &branch)
{
    auto variable = dynamic_cast<Variable*>(branch.get());
    if (variable != nullptr) {
        return true;
    }

    auto operation = dynamic_cast<Operation*>(branch.get());
    if (operation == nullptr) {
        return false;
    }

    auto &lhs = operation->lhs;
    auto &rhs = operation->rhs;

    return hasVariable(lhs) || hasVariable(rhs);
}

bool Solver::simplify(std::unique_ptr<Expression> &ast)
{
    auto operation = dynamic_cast<Operation*>(ast.get());
    if (operation == nullptr) {
        return false;
    }

    simplify(operation->lhs);
    simplify(operation->rhs);

    auto lhs = dynamic_cast<Number*>(operation->lhs.get());
    auto rhs = dynamic_cast<Number*>(operation->rhs.get());

    if ((lhs == nullptr) || (rhs == nullptr)) {
        return false;
    }

    int lhsNum = lhs->value;
    int rhsNum = rhs->value;
    int result;

    switch (operation->operation) {
    case '+':
        result = lhsNum + rhsNum;
        break;

    case '-':
        result = lhsNum - rhsNum;
        break;

    case '*':
        result = lhsNum * rhsNum;
        break;

    case '/':
        if (rhsNum == 0) {
            std::cerr << "Ошибка деления на ноль" << std::endl;
            return false;
        }
        result = lhsNum / rhsNum;
        break;
    }

    ast = std::make_unique<Number>(result);
    return true;
}

bool Solver::solve(std::unique_ptr<Expression> &ast, int &result)
{
    auto operation = dynamic_cast<Operation*>(ast.get());
    if (operation == nullptr) {
        return false;
    }

    if (operation->operation != '=') {
        return false;
    }

    if (! hasVariable(ast)) {
        return false;
    }

    if (hasVariable(operation->rhs)) {
        ast = std::make_unique<Operation>(operation->operation,
                                          std::move(operation->rhs),
                                          std::move(operation->lhs));
    }

    if (! rebuild(operation->lhs, operation->rhs)) {
        return false;
    }

    if (! simplify(operation->rhs)) {
        return false;
    }

    auto answer = dynamic_cast<Number*>(operation->rhs.get());
    result = answer->value;
    return true;
}

bool Solver::rebuild(std::unique_ptr<Expression> &lhs, std::unique_ptr<Expression> &rhs)
{
    while (! is_variable(lhs)) {
        auto operation = dynamic_cast<Operation*>(lhs.get());
        if (operation == nullptr) {
            return false;
        }

        enum { Right, Left } varside = (hasVariable(operation->lhs)) ? Left : Right;

        switch (operation->operation) {
        case '+':
            if (varside == Left) {
                rhs = std::make_unique<Operation>('-', std::move(rhs), std::move(operation->rhs));
                lhs = std::move(operation->lhs);
            } else {
                rhs = std::make_unique<Operation>('-', std::move(rhs), std::move(operation->lhs));
                lhs = std::move(operation->rhs);
            }
            break;

        case '-':
            if (varside == Left) {
                rhs = std::make_unique<Operation>('+', std::move(rhs), std::move(operation->rhs));
                lhs = std::move(operation->lhs);
            } else {
                rhs = std::make_unique<Operation>('-', std::move(operation->lhs), std::move(rhs));
                lhs = std::move(operation->rhs);
            }
            break;

        case '*':
            if (varside == Left) {
                rhs = std::make_unique<Operation>('/', std::move(rhs), std::move(operation->rhs));
                lhs = std::move(operation->lhs);
            } else {
                rhs = std::make_unique<Operation>('/', std::move(rhs), std::move(operation->lhs));
                lhs = std::move(operation->rhs);
            }
            break;

        case '/':
            if (varside == Left) {
                rhs = std::make_unique<Operation>('*', std::move(rhs), std::move(operation->rhs));
                lhs = std::move(operation->lhs);
            } else {
                rhs = std::make_unique<Operation>('/', std::move(operation->lhs), std::move(rhs));
                lhs = std::move(operation->rhs);
            }
            break;
        }
    }

    return true;
}

bool Solver::is_variable(const std::unique_ptr<Expression> &expr)
{
    auto var = dynamic_cast<const Variable*>(expr.get());
    return var != nullptr;
}

// Точка входа
// =============================================================================

#ifndef UNIT_TEST

int main(int argc, char *argv[])
{
    std::cout << "Введите уравнение: ";
    std::string input;
    std::getline(std::cin, input);

    Parser p(input);
    auto ast = p.parse();
    int result;
    auto solved = Solver::solve(ast, result);
    if (solved) {
        std::cout << "Ответ: " << result << std::endl;
    }
    return (solved) ? EXIT_SUCCESS : EXIT_FAILURE;
}

#endif /* UNIT_TEST */
