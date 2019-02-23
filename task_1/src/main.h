#ifndef MAIN_H
#define MAIN_H

#include <sstream>
#include <string>

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

#endif /* MAIN_H */
