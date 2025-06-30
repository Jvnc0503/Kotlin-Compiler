#ifndef SCANNER_H
#define SCANNER_H
#include <string>
#include "token.h"

struct Scanner {
    std::string input;
    int first, current, line = 1, column = 1;

    explicit Scanner(std::string input);

    ~Scanner() = default;

    static bool is_white_space(char c);

    static bool is_id_char(char c);

    Token* nextToken();

    void reset();

    void test();
};

#endif  // SCANNER_H
