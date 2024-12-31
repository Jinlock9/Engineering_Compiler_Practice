#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "Token.h"

static std::string::iterator current;

enum class CharType {
    Unknown,
    WhiteSpace,
    NumberLiteral,
    StringLiteral,
    IdentifierAndKeyword,
    OperatorAndPunctuator,
};

auto scan(std::string)->std::vector<Token>;

auto getCharType(char c)->CharType;
auto isCharType(char c, CharType type)->bool;
auto scanNumberLiteral()->Token;
auto scanStringLiteral()->Token;
auto scanIdentifierAndKeyword()->Token;
auto scanOperatorAndPunctuator()->Token;

auto printTokenList(std::vector<Token>)->void;

#endif // __SCANNER_H__