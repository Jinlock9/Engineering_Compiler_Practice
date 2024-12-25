#include "Token.h"

#include <map>
#include <iomanip>

static std::map<std::string, Kind> stringToKind = {
    {"#unknown",    Kind::Unknown},
    {"#EndOfToken", Kind::EndOfToken},
    
    {"null",        Kind::NullLiteral},
    {"true",        Kind::TrueLiteral},
    {"false",       Kind::FalseLiteral},
    {"#Number",     Kind::NumberLiteral},
    {"#String",     Kind::StringLiteral},
    {"#identifier", Kind::Identifier},

    {"funciton",    Kind::Function},
    {"return",      Kind::Return},
    {"var",         Kind::Variable},
    {"for",         Kind::For},
    {"break",       Kind::Break},
    {"continue",    Kind::Continue},
    {"if",          Kind::If},
    {"elif",        Kind::Elif},
    {"else",        Kind::Else},
    {"print",       Kind::Print},
    {"printLine",   Kind::PrintLine},

    {"and",         Kind::LogicalAnd},
    {"or",          Kind::LogicalOr},

    {"=",           Kind::Assignment},

    {"+",           Kind::Add},
    {"-",           Kind::Subtract},
    {"*",           Kind::Multiply},
    {"/",           Kind::Divide},
    {"%",           Kind::Modulo},

    {"==",          Kind::Equal},
    {"!=",          Kind::NotEqual},
    {"<",           Kind::LessThan},
    {">",           Kind::GreaterThan},
    {"<=",          Kind::LessOrEqual},
    {">=",          Kind::GreaterOrEqual},

    {",",           Kind::Comma},
    {":",           Kind::Colon},
    {";",           Kind::Semicolon},
    {"(",           Kind::LeftParen},
    {")",           Kind::RightParen},
    {"{",           Kind::LeftBrace},
    {"}",           Kind::RightBrace},
    {"[",           Kind::LeftBracket},
    {"]",           Kind::RightBracket},
};

static auto kindToString = [] {
  std::map<Kind, std::string> result;
  for (auto& [key, value] : stringToKind)
    result[value] = key;
  return result;
}();

auto toKind(std::string string)->Kind {
  if (stringToKind.count(string))
    return stringToKind.at(string);
  return Kind::Unknown;
}

auto toString(Kind type)->std::string {
  if (kindToString.count(type))
    return kindToString.at(type);
  return "";
}

auto operator<<(std::ostream& stream, Token& token)->std::ostream& {
  return stream << std::setw(12) << std::left << toString(token.kind) << token.string;
}