#ifndef __MAIN_H__
#define __MAIN_H__

#include <string>
#include <vector>

#include "Token.h"

auto scan(std::string)->std::vector<Token>;
auto printTokenList(std::vector<Token>)->void;

#endif // __MAIN_H__