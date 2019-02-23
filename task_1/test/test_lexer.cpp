#include <main.h>
#include <vector>
#include <cstdlib>
#include <iostream>

std::ostream& operator << (std::ostream &os, const Lexer::Token &token)
{
    switch (token) {
    case Lexer::No:
        os << "Lexer::No";
        break;

    case Lexer::Number:
        os << "Lexer::Number";
        break;

    case Lexer::Variable:
        os << "Lexer::Variable";
        break;

    case Lexer::Operation:
        os << "Lexer::Operation";
        break;

    case Lexer::Error:
        os << "Lexer::Error";
        break;
    }

    return os;
}

int main(int argc, char *argv[])
{
    Lexer lexer("1 + 2 + 3/4 = 7*x");
    std::vector<Lexer::Token> tokens{
        Lexer::Number,
        Lexer::Operation,
        Lexer::Number,
        Lexer::Operation,
        Lexer::Number,
        Lexer::Operation,
        Lexer::Number,
        Lexer::Operation,
        Lexer::Number,
        Lexer::Operation,
        Lexer::Variable,
        Lexer::No
    };

    bool error = false;
    for (auto token : tokens) {
        auto _token = lexer.getNextToken();
        error = token != _token;
        if (error) {
            std::cerr << "get      " << _token << std::endl
                      << "expected " << token << std::endl
                      << "on text '" << lexer.text << "'" << std::endl;
            break;
        }
    }

    return (error) ? EXIT_FAILURE : EXIT_SUCCESS;
}

