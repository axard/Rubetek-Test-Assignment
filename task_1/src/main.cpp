#include "main.h"
#include <iostream>

// Типы
// =============================================================================

// Реализация Lexer
// -----------------------------------------------------------------------------

Lexer::Lexer(const std::string &str) noexcept
    :input(str)
    ,text()
    ,token(Lexer::No)
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

// Реализация Parser
// -----------------------------------------------------------------------------

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
    std::unique_ptr<Expression> lhs = nullptr;
    do {
        lhs = parseOperation((lhs) ? std::move(lhs) : nullptr);
    } while ((lexer.token != Lexer::No) && (lhs != nullptr));
    return lhs;
}

// Точка входа
// =============================================================================

#ifndef UNIT_TEST

int main(int argc, char *argv[])
{
    return 0;
}

#endif /* UNIT_TEST */
