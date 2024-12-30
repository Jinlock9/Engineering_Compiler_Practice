#include <map>
#include <any>
#include <list>
#include <vector>
#include <functional>
#include <cmath>

#include "Datatype.h"
#include "../Parser/Node.h"

struct ReturnException { std::any result; };
struct BreakException {};
struct ContinueException {};

static std::map<std::string, std::any> global;
static std::list<std::list<std::map<std::string, std::any>>> local;
static std::map<std::string, Function *> functionTable;

extern std::map<std::string, std::function<std::any(std::vector<std::any>)>> builtinFunctionTable;

auto interpret(Program *program)->void {
    functionTable.clear();
    global.clear();
    local.clear();
    for (auto& node: program->functions)
        functionTable[node->name] = node;
    if (functionTable["main"] == nullptr)
        return;
    local.emplace_back().emplace_front();
    try {
        functionTable["main"]->interpret();
    }
    catch (ReturnException) {}
    catch (BreakException) {}
    catch (ContinueException) {}
    local.pop_back();
}

auto Function::interpret()->void {
    for (auto& node: block)
        node->interpret();
}

auto Return::interpret()->void {
    throw ReturnException{expression->interpret()};
}

auto Variable::interpret()->void {
    local.back().front()[name] = expression->interpret();
}

auto For::interpret()->void {
    local.back().emplace_front();
    variable->interpret();
    while (true) {
        auto result = condition->interpret();
        if (isTrue(result) == false)
            break;
        try {
            for (auto &node: block)
                node->interpret();
        } catch (ContinueException) {
            /* empty */
        } catch (BreakException) {
            break;
        }
        expression->interpret();
    }
    local.back().pop_front();
}

auto Break::interpret()->void {
    throw BreakException();
}

auto Continue::interpret()->void {
    throw ContinueException();
}

auto If::interpret()->void {
    for (size_t i = 0; i < conditions.size(); i++) {
        auto result = conditions[i]->interpret();
        if (isTrue(result) == false)
            continue;
        local.back().emplace_front();
        for (auto& node: blocks[i])
            node->interpret();
        local.back().pop_front();
        return;
    }
    if (elseBlock.empty())
        return;
    local.back().emplace_front();
    for (auto& node: elseBlock)
        node->interpret();
    local.back().pop_front();
}

auto Print::interpret()->void {
    for (auto& node: arguments) {
        auto value = node->interpret();
        std::cout << value;
    }
    if (lineFeed) std::cout << std::endl;
}

auto ExpressionStatement::interpret()->void {
    expression->interpret();
}

auto Or::interpret()->std::any {
    return isTrue(lhs->interpret()) ? true : rhs->interpret();
}

auto And::interpret()->std::any {
    return isFalse(lhs->interpret()) ? false : rhs->interpret();
}

auto Relational::interpret()->std::any {
    auto lValue = lhs->interpret();
    auto rValue = rhs->interpret();
    if (kind == Kind::Equal && isNull(lValue) && isNull(rValue)) {
        return true;
    }
    if (kind == Kind::Equal && isBoolean(lValue) && isBoolean(rValue)) {
        return toBoolean(lValue) == toBoolean(rValue);
    }
    if (kind == Kind::Equal && isNumber(lValue) && isNumber(rValue)) {
        return toNumber(lValue) == toNumber(rValue);
    }
    if (kind == Kind::Equal && isString(lValue) && isString(rValue)) {
        return toString(lValue) == toString(rValue);
    }
    if (kind == Kind::NotEqual && isNull(lValue) && isNull(rValue)) {
        return false;
    }
    if ((kind == Kind::NotEqual && isNull(lValue)) || isNull(rValue)) {
        return true;
    }
    if (kind == Kind::NotEqual && isBoolean(lValue) && isBoolean(rValue)) {
        return toBoolean(lValue) != toBoolean(rValue);
    }
    if (kind == Kind::NotEqual && isNumber(lValue) && isNumber(rValue)) {
        return toNumber(lValue) != toNumber(rValue);
    }
    if (kind == Kind::NotEqual && isString(lValue) && isString(rValue)) {
        return toString(lValue) != toString(rValue);
    }
    if (kind == Kind::LessThan && isNumber(lValue) && isNumber(rValue)) {
        return toNumber(lValue) < toNumber(rValue);
    }
    if (kind == Kind::GreaterThan && isNumber(lValue) && isNumber(rValue)) {
        return toNumber(lValue) > toNumber(rValue);
    }
    if (kind == Kind::LessOrEqual && isNumber(lValue) && isNumber(rValue)) {
        return toNumber(lValue) <= toNumber(rValue);
    }
    if (kind == Kind::GreaterOrEqual && isNumber(lValue) && isNumber(rValue)) {
        return toNumber(lValue) >= toNumber(rValue);
    }
    return false;
}

auto Arithmetic::interpret()->std::any {
    auto lValue = lhs->interpret();
    auto rValue = rhs->interpret();
    if (kind == Kind::Add && isNumber(lValue) && isNumber(rValue)) {
        return toNumber(lValue) + toNumber(rValue);
    }
    if (kind == Kind::Add && isString(lValue) && isString(rValue)) {
        return toString(lValue) + toString(rValue);
    }
    if (kind == Kind::Subtract && isNumber(lValue) && isNumber(rValue)) {
        return toNumber(lValue) - toNumber(rValue);
    }
    if (kind == Kind::Multiply && isNumber(lValue) && isNumber(rValue)) {
        return toNumber(lValue) * toNumber(rValue);
    }
    if (kind == Kind::Divide && isNumber(lValue) && isNumber(rValue)) {
        return toNumber(rValue) == 0 ? 0.0 : toNumber(lValue) / toNumber(rValue);
    }
    if (kind == Kind::Modulo && isNumber(lValue) && isNumber(rValue)) {
        return toNumber(rValue) == 0 ? toNumber(lValue) : fmod(toNumber(lValue), toNumber(rValue));
    }
    return 0.0;
}

auto Unary::interpret()->std::any {
    auto value = sub->interpret();
    if (kind == Kind::Add && isNumber(value))
        return abs(toNumber(value));
    if (kind == Kind::Subtract && isNumber(value))
        return toNumber(value) * -1;
    return 0.0;
}

auto Call::interpret()->std::any {
    auto value = sub->interpret();
    if (isBuiltinFunction(value)) {
        std::vector<std::any> values;
        for (size_t i = 0; i < arguments.size(); i++)
        values.push_back(arguments[i]->interpret());
        return toBuiltinFunction(value)(values);
    }
    if (isFunction(value) == false)
        return nullptr;
    std::map<std::string, std::any> parameters;
    for (size_t i = 0; i < arguments.size(); i++) {
        auto name = toFunction(value)->parameters[i];
        parameters[name] = arguments[i]->interpret();
    }
    local.emplace_back().push_front(parameters);
    try {
        toFunction(value)->interpret();
    } catch (ReturnException exception) {
        local.pop_back();
        return exception.result;
    }
    local.pop_back();
    return nullptr;
}

auto GetElement::interpret()->std::any {
    auto object = sub->interpret();
    auto index_ = index->interpret();
    if (isArray(object) && isNumber(index_))
        return getValueOfArray(object, index_);
    if (isMap(object) && isString(index_))
        return getValueOfMap(object, index_);
    return nullptr;
}

auto SetElement::interpret()->std::any {
    auto object = sub->interpret();
    auto index_ = index->interpret();
    auto value_ = value->interpret();
    if (isArray(object) && isNumber(index_))
        return setValueOfArray(object, index_, value_);
    if (isMap(object) && isString(index_))
        return setValueOfMap(object, index_, value_);
    return nullptr;
}

auto GetVariable::interpret()->std::any {
    for (auto& variables: local.back()) {
        if (variables.count(name))
        return variables[name];
    }
    if (global.count(name))
        return global[name];
    if (functionTable.count(name))
        return functionTable[name];
    if (builtinFunctionTable.count(name))
        return builtinFunctionTable[name];
    return nullptr;
}

auto SetVariable::interpret()->std::any {
    for (auto& variables: local.back()) {
        if (variables.count(name))
        return variables[name] = value->interpret();
    }
    return global[name] = value->interpret();
}

auto NullLiteral::interpret()->std::any {
    return nullptr;
}

auto BooleanLiteral::interpret()->std::any {
    return value;
}

auto NumberLiteral::interpret()->std::any {
    return value;
}

auto StringLiteral::interpret()->std::any {
    return value;
}

auto ArrayLiteral::interpret()->std::any {
    auto result = new Array();
    for (auto& node: values)
        result->values.push_back(node->interpret());
    return result;
}

auto MapLiteral::interpret()->std::any {
    auto result = new Map();
    for (auto& [key, value]: values)
        result->values.insert_or_assign(key, value->interpret());
    return result;
}