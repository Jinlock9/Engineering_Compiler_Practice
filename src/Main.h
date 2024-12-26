#ifndef __MAIN_H__
#define __MAIN_H__

#include <string>
#include <vector>

#include "../Scanner/Token.h"
#include "../Scanner/Scanner.h"
#include "../Parser/Node.h"

auto scan(std::string)->std::vector<Token>;
auto parse(std::vector<Token>)->Program *;
auto interpret(Program *)->void;

auto printTokenList(std::vector<Token>)->void;
auto printSyntaxTree(Program*)->void;

#endif // __MAIN_H__