#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>
#include <iostream>

#include "Token.h"

enum class Kind {
    // Define the lexicon
    Unknown, EndOfToken,

    NullLiteral,
    TrueLiteral, FalseLiteral,
    NumberLiteral, StringLiteral,
    Identifier,

    Function, Return,
    Variable,
    For, Break, Continue,
    If, Elif, Else,
    Print, PrintLine,

    LogicalAnd, LogicalOr,
    Assignment,
    Add, Subtract,
    Multiply, Divide, Modulo,
    Equal, NotEqual,
    LessThan, GreaterThan,
    LessOrEqual, GreaterOrEqual,

    Comma, Colon, Semicolon,
    LeftParen, RightParen,
    LeftBrace, RightBrace,
    LeftBracket, RightBracket,
};

auto toKind(std::string)->Kind;
auto toString(Kind)->std::string;

struct Token {
    // Token Structure
    Kind kind = Kind::Unknown;
    std::string string;
};

auto operator<<(std::ostream&, Token&)->std::ostream&;

#endif // __TOKEN_H__