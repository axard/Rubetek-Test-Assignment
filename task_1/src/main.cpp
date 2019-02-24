#include "main.h"

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

std::unique_ptr<Expression> Parser::parse()
{
    return std::make_unique<Expression>();
}

// Точка входа
// =============================================================================

#ifndef UNIT_TEST

int main(int argc, char *argv[])
{
    return 0;
}

#endif /* UNIT_TEST */
