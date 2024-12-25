#ifndef __PARSER_H__
#define __PARSER_H__

#include <vector>

#include "../Scanner/Token.h"
#include "Node.h"

auto parseFunction()->Function *;
auto skipCurrent(Kind)->void;
auto parseBlock()->std::vector<Statement *>;
auto parseVariable()->Variable *;

#endif // __PARSER_H__