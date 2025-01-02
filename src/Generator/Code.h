#ifndef __CODE_H__
#define __CODE_H__

#include <any>
#include <string>
#include <iostream>

enum class Instruction {
    Exit,
    Call, Alloca, Return,
    Jump, ConditionJump,
    Print, PrintLine,

    LogicalOr, LogicalAnd,
    Add, Subtract,
    Multiply, Divide, Modulo,
    Equal, NotEqual,
    LessThan, GreaterThan,
    LessOrEqual, GreaterOrEqual,
    Absolute, ReverseSign,

    GetElement, SetElement,
    GetGlobal, SetGlobal,
    GetLocal, SetLocal,

    PushNull, PushBoolean,
    PushNumber, PushString,
    PushArray, PushMap,
    PopOperand,
};

auto toString(Instruction)->std::string;

struct Code {
    Instruction instruction;
    std::any operand;
};

auto operator<<(std::ostream&, Code&)->std::ostream&;

#endif // __CODE_H__