#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include "../Parser/Node.h"
#include "Code.h"

auto generate(Program*)->std::tuple<std::vector<Code>, std::map<std::string, size_t>>;

auto getLocal(std::string)->size_t;
auto setLocal(std::string)->void;
auto initBlock()->void;
auto pushBlock()->void;
auto popBlock()->void;
auto writeCode(Instruction)->size_t;
auto writeCode(Instruction, std::any)->size_t;
auto patchAddress(size_t)->void;
auto patchOperand(size_t, size_t)->void;

auto printObjectCode(std::tuple<std::vector<Code>, std::map<std::string, size_t>>)->void;

#endif // __GENERATOR_H__