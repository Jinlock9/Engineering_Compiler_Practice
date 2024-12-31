#ifndef __MAIN_H__
#define __MAIN_H__

#include <tuple>
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "../Scanner/Token.h"
#include "../Scanner/Scanner.h"
#include "../Parser/Node.h"
#include "../Generator/Code.h"

auto scan(std::string)->std::vector<Token>;
auto parse(std::vector<Token>)->Program*;
auto interpret(Program *)->void;
auto generate(Program*)->std::tuple<std::vector<Code>, std::map<std::string, size_t>>;

auto printTokenList(std::vector<Token>)->void;
auto printSyntaxTree(Program*)->void;
auto printObjectCode(std::tuple<std::vector<Code>, std::map<std::string, size_t>>)->void;

#endif // __MAIN_H__