#ifndef __MACHINE_H__
#define __MACHINE_H__

#include <any>
#include <map>
#include <list>
#include <vector>
#include <functional>
#include <cmath>

#include "../Interpreter/Datatype.h"
#include "../Generator/Code.h"

struct StackFrame {
    std::vector<std::any> variables;
    std::vector<std::any> operandStack;
    size_t instructionPointer = 0;
};

auto execute(std::tuple<std::vector<Code>, std::map<std::string, size_t>>)->void;

auto pushOperand(std::any value)->void;
auto peekOperand()->std::any;
auto popOperand()->std::any;
auto collectGarbage()->void;
auto markObject(std::any)->void;
auto sweepObject()->void;

#endif // __MACHINE_H__