#include <map>
#include <list>
#include <tuple>
#include <vector>
#include <iomanip>
#include <cstdint>

#include "Generator.h"

static std::vector<Code> codeList;
static std::map<std::string, size_t> functionTable;
static std::list<std::map<std::string, size_t>> symbolStack;
static std::vector<size_t> offsetStack;
static size_t localSize;
static std::vector<std::vector<size_t>> continueStack;
static std::vector<std::vector<size_t>> breakStack;

auto generate(Program* program)->std::tuple<std::vector<Code>, std::map<std::string, size_t>> {
    codeList.clear();
    functionTable.clear();
    writeCode(Instruction::GetGlobal, std::string("main"));
    writeCode(Instruction::Call, static_cast<size_t>(0));
    writeCode(Instruction::Exit);
    for (auto& node: program->functions)
        node->generate();
    return {codeList, functionTable};
}

auto Function::generate()->void {
    functionTable[name] = codeList.size();
    auto temp = writeCode(Instruction::Alloca);
    initBlock();
    for (auto& name: parameters)
        setLocal(name);
    for (auto& node: block)
        node->generate();
    popBlock();
    patchOperand(temp, localSize);
    writeCode(Instruction::Return);
}

auto Variable::generate()->void {
    setLocal(name);
    expression->generate();
    writeCode(Instruction::SetLocal, getLocal(name));
    writeCode(Instruction::PopOperand);
}

auto For::generate()->void {
    breakStack.emplace_back();
    continueStack.emplace_back();
    pushBlock();
    variable->generate();
    auto jumpAddress = codeList.size();
    condition->generate();
    auto conditionJump = writeCode(Instruction::ConditionJump);
    for (auto& node: block)
        node->generate();
    auto continueAddress = codeList.size();
    expression->generate();
    writeCode(Instruction::PopOperand);
    writeCode(Instruction::Jump, jumpAddress);
    patchAddress(conditionJump);
    popBlock();
    for (auto& jump: continueStack.back())
        patchOperand(jump, continueAddress);
    continueStack.pop_back();
    for (auto& jump: breakStack.back())
        patchAddress(jump);
    breakStack.pop_back();
}

auto Continue::generate()->void {
    if (continueStack.empty()) return;
    auto jumpCode = writeCode(Instruction::Jump);
    continueStack.back().push_back(jumpCode);
}

auto Break::generate()->void {
    if (breakStack.empty()) return;
    auto jumpCode = writeCode(Instruction::Jump);
    breakStack.back().push_back(jumpCode);
}

auto If::generate()->void {
    std::vector<size_t> jumpList;
    for (size_t i = 0; i < conditions.size(); i++) {
        conditions[i]->generate();
        auto conditionJump = writeCode(Instruction::ConditionJump);
        pushBlock();
        for (auto& node: blocks[i])
        node->generate();
        popBlock();
        jumpList.push_back(writeCode(Instruction::Jump));
        patchAddress(conditionJump);
    }
    if (elseBlock.empty() == false) {
        pushBlock();
        for (auto& node: elseBlock)
        node->generate();
        popBlock();
    }
    for (auto& jump: jumpList)
        patchAddress(jump);
}

auto Print::generate()->void {
    for (auto i = arguments.size(); i > 0; i--)
        arguments[i - 1]->generate();
    writeCode(Instruction::Print, arguments.size());
    if (lineFeed)
        writeCode(Instruction::PrintLine);
}

auto Return::generate()->void {
    expression->generate();
    writeCode(Instruction::Return);
}

auto ExpressionStatement::generate()->void {
    expression->generate();
    writeCode(Instruction::PopOperand);
}

auto Or::generate()->void {
    lhs->generate();
    auto logicalOr = writeCode(Instruction::LogicalOr);
    rhs->generate();
    patchAddress(logicalOr);
}

auto And::generate()->void {
    lhs->generate();
    auto logicalAnd = writeCode(Instruction::LogicalAnd);
    rhs->generate();
    patchAddress(logicalAnd);
}

auto Relational::generate()->void {
    std::map<Kind, Instruction> instructions = {
        {Kind::Equal,          Instruction::Equal},
        {Kind::NotEqual,       Instruction::NotEqual},
        {Kind::LessThan,       Instruction::LessThan},
        {Kind::GreaterThan,    Instruction::GreaterThan},
        {Kind::LessOrEqual,    Instruction::LessOrEqual},
        {Kind::GreaterOrEqual, Instruction::GreaterOrEqual}
    };
    lhs->generate();
    rhs->generate();
    writeCode(instructions[kind]);
}

auto Arithmetic::generate()->void {
    std::map<Kind, Instruction> instructions = {
        {Kind::Add,      Instruction::Add},
        {Kind::Subtract, Instruction::Subtract},
        {Kind::Multiply, Instruction::Multiply},
        {Kind::Divide,   Instruction::Divide},
        {Kind::Modulo,   Instruction::Modulo},
    };
    lhs->generate();
    rhs->generate();
    writeCode(instructions[kind]);
}

auto Unary::generate()->void {
    std::map<Kind, Instruction> instructions = {
        {Kind::Add,      Instruction::Absolute},
        {Kind::Subtract, Instruction::ReverseSign}
    };
    sub->generate();
    writeCode(instructions[kind]);
}

auto Call::generate()->void {
    for (auto i = arguments.size(); i > 0; i--)
        arguments[i - 1]->generate();
    sub->generate();
    writeCode(Instruction::Call, arguments.size());
}

auto GetElement::generate()->void {
    sub->generate();
    index->generate();
    writeCode(Instruction::GetElement);
}

auto SetElement::generate()->void {
    value->generate();
    sub->generate();
    index->generate();
    writeCode(Instruction::SetElement);
}

auto NullLiteral::generate()->void {
    writeCode(Instruction::PushNull);
}

auto BooleanLiteral::generate()->void {
    writeCode(Instruction::PushBoolean, value);
}

auto NumberLiteral::generate()->void {
    writeCode(Instruction::PushNumber, value);
}

auto StringLiteral::generate()->void {
    writeCode(Instruction::PushString, value);
}

auto ArrayLiteral::generate()->void {
    for (auto i = values.size(); i > 0; i--)
        values[i - 1]->generate();
    writeCode(Instruction::PushArray, values.size());
}

auto MapLiteral::generate()->void {
    for (auto& [key, value]: values) {
        writeCode(Instruction::PushString, key);
        value->generate();
    }
    writeCode(Instruction::PushMap, values.size());
}

auto GetVariable::generate()->void {
    if (getLocal(name) == SIZE_MAX)
        writeCode(Instruction::GetGlobal, name);
    else
        writeCode(Instruction::GetLocal, getLocal(name));
}

auto SetVariable::generate()->void {
    value->generate();
    if (getLocal(name) == SIZE_MAX)
        writeCode(Instruction::SetGlobal, name);
    else
        writeCode(Instruction::SetLocal, getLocal(name));
}

auto getLocal(std::string name)->size_t {
    for (auto& symbolTable: symbolStack) {
        if (symbolTable.count(name))
        return symbolTable[name];
    }
    return SIZE_MAX;
}

auto setLocal(std::string name)->void {
    symbolStack.front()[name] = offsetStack.back();
    offsetStack.back() += 1;
    localSize = std::max(localSize, offsetStack.back());
}

auto initBlock()->void {
    localSize = 0;
    offsetStack.push_back(0);
    symbolStack.emplace_front();
}

auto pushBlock()->void {
    symbolStack.emplace_front();
    offsetStack.push_back(offsetStack.back());
}

auto popBlock()->void {
    offsetStack.pop_back();
    symbolStack.pop_front();
}

auto writeCode(Instruction instruction)->size_t {
    codeList.push_back({instruction, nullptr});
    return codeList.size() - 1;
}

auto writeCode(Instruction instruction, std::any operand)->size_t {
    codeList.push_back({instruction, operand});
    return codeList.size() - 1;
}

auto patchAddress(size_t codeIndex)->void {
    codeList[codeIndex].operand = codeList.size();
}

auto patchOperand(size_t codeIndex, size_t operand)->void {
    codeList[codeIndex].operand = operand;
}

auto printObjectCode(std::tuple<std::vector<Code>, std::map<std::string, size_t>> objectCode)->void {
    auto codeList = std::get<0>(objectCode);
    auto functionTable = std::get<1>(objectCode);
    std::cout << std::setw(11) << std::left << "FUNCTION" << "ADDRESS" << std::endl;
    std::cout << std::string(18, '-') << std::endl;
    for (auto& [key, value] : functionTable)
        std::cout << std::setw(11) << key << value << std::endl;
    std::cout << std::endl;
    std::cout << "ADDR" << " " << std::setw(15) << "INSTRUCTION" << "OPERAND" << std::endl;
    std::cout << std::string(36, '-') << std::endl;
    for (size_t i = 0; i < codeList.size(); i++)
        std::cout << std::setw(4) << std::right << i << " " << codeList[i] << std::endl;
}