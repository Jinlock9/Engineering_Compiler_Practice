#include "Node.h"

#include <iostream>

static auto indent(int)->void;

auto printSyntaxTree(Program* program)->void {
    for (auto& node: program->functions) {
        node->print(0);
    }
}

auto Function::print(int depth)->void {
    indent(depth); std::cout << "FUNCTION " << name << ": " << std::endl;
    if (parameters.size()) {
        indent(depth + 1); std::cout << "PARAMETERS:";
        for (auto& name: parameters) std::cout << name << " ";
        std::cout << std::endl;
    }
    indent(depth + 1); std::cout << "BLOCK:" << std::endl;
    for (auto& node: block) node->print(depth + 2);
}

auto For::print(int depth)->void {
    indent(depth); std::cout << "FOR:" << std::endl;
    indent(depth + 1); std::cout << "VARIABLE:" << std::endl;
    variable->print(depth + 2);
    indent(depth + 1); std::cout << "CONDITION:" << std::endl;
    condition->print(depth + 2);
    indent(depth + 1); std::cout << "EXPRESSION:" << std::endl;
    expression->print(depth + 2);
    indent(depth + 1); std::cout << "BLOCK:" << std::endl;
    for (auto& node: block) node->print(depth + 2);
}

auto If::print(int depth)->void {
    for (size_t i = 0; i < conditions.size(); i++) {
        indent(depth); std::cout << (i == 0 ? "IF:" : "ELIF:") << std::endl;
        indent(depth + 1); std::cout << "CONDITION:" << std::endl;
        conditions[i]->print(depth + 2);
        indent(depth + 1); std::cout << "BLOCK:" << std::endl;
        for (auto& node: blocks[i])
        node->print(depth + 2);
    }
    if (elseBlock.size() == 0) return;
    indent(depth); std::cout << "ELSE:" << std::endl;
    for (auto& node: elseBlock) node->print(depth + 1);
}

auto Variable::print(int depth)->void {
    indent(depth); std::cout << "VAR " << name << ":" << std::endl;
    expression->print(depth + 1);
}

auto Print::print(int depth)->void {
    indent(depth); std::cout << (lineFeed ? "PRINT_LINE" : "PRINT:") << std::endl;
    for (auto& node: arguments) node->print(depth + 1);
}

auto Return::print(int depth)->void {
    indent(depth); std::cout << "RETURN:" << std::endl;
    expression->print(depth + 1);
}

auto Break::print(int depth)->void {
    indent(depth); std::cout << "BREAK" << std::endl;
}

auto Continue::print(int depth)->void {
    indent(depth); std::cout << "CONTINUE" << std::endl;
}

auto ExpressionStatement::print(int depth)->void {
    indent(depth); std::cout << "EXPRESSION:" << std::endl;
    expression->print(depth + 1);
}

auto Or::print(int depth)->void {
    indent(depth); std::cout << "OR:" << std::endl;
    indent(depth + 1); std::cout << "LHS:" << std::endl;
    lhs->print(depth + 2);
    indent(depth + 1); std::cout << "RHS:" << std::endl;
    rhs->print(depth + 2);
}

auto And::print(int depth)->void {
    indent(depth); std::cout << "AND:" << std::endl;
    indent(depth + 1); std::cout << "LHS:" << std::endl;
    lhs->print(depth + 2);
    indent(depth + 1); std::cout << "RHS:" << std::endl;
    rhs->print(depth + 2);
}

auto Relational::print(int depth)->void {
    indent(depth); std::cout << toString(kind) << ":" << std::endl;
    indent(depth + 1); std::cout << "LHS:" << std::endl;
    lhs->print(depth + 2);
    indent(depth + 1); std::cout << "RHS:" << std::endl;
    rhs->print(depth + 2);
}

auto Arithmetic::print(int depth)->void {
    indent(depth); std::cout << toString(kind) << ":" << std::endl;
    indent(depth + 1); std::cout << "LHS:" << std::endl;
    lhs->print(depth + 2);
    indent(depth + 1); std::cout << "RHS:" << std::endl;
    rhs->print(depth + 2);
}

auto Unary::print(int depth)->void {
    indent(depth); std::cout << toString(kind) << std::endl;
    sub->print(depth + 1);
}

auto GetElement::print(int depth)->void {
    indent(depth); std::cout << "GET_ELEMENT:" << std::endl;
    indent(depth + 1); std::cout << "SUB:" << std::endl;
    sub->print(depth + 2);
    indent(depth + 1); std::cout << "INDEX:" << std::endl;
    index->print(depth + 2);
}

auto SetElement::print(int depth)->void {
    indent(depth); std::cout << "SET_ELEMENT:" << std::endl;
    indent(depth + 1); std::cout << "SUB:" << std::endl;
    sub->print(depth + 2);
    indent(depth + 1); std::cout << "INDEX:" << std::endl;
    index->print(depth + 2);
    indent(depth + 1); std::cout << "VALUE:" << std::endl;
    value->print(depth + 2);
}

auto Call::print(int depth)->void {
    indent(depth); std::cout << "CALL:" << std::endl;
    indent(depth + 1); std::cout << "EXPRESSION:" << std::endl;
    sub->print(depth + 2);
    for (auto& node: arguments) {
        indent(depth + 1); std::cout << "ARGUMENT:" << std::endl;
        node->print(depth + 2);
    }
}

auto NullLiteral::print(int depth)->void {
    indent(depth); std::cout << "null" << std::endl;
}

auto BooleanLiteral::print(int depth)->void {
    indent(depth); std::cout << (value ? "true" : "false") << std::endl;
}

auto NumberLiteral::print(int depth)->void {
    indent(depth); std::cout << value << std::endl;
}

auto StringLiteral::print(int depth)->void {
    indent(depth); std::cout << "\"" << value << "\"" << std::endl;
}

auto ArrayLiteral::print(int depth)->void {
    indent(depth); std::cout << "[" << std::endl;
    for (auto& node: values) node->print(depth + 1);
    indent(depth); std::cout << "]" << std::endl;
}

auto MapLiteral::print(int depth)->void {
    indent(depth); std::cout << "{" << std::endl;
    for (auto& [key, value]: values) {
        std::cout << key << ": ";
        value->print(depth + 1);
    }
    indent(depth); std::cout << "}" << std::endl;
}

auto GetVariable::print(int depth)->void {
    indent(depth); std::cout << "GET_VARIABLE: " << name << std::endl;
}

auto SetVariable::print(int depth)->void {
    indent(depth); std::cout << "SET_VARIABLE: " << name << std::endl;
    value->print(depth + 1);
}

auto indent(int depth)->void {
    std::cout << std::string(static_cast<size_t>(depth * 2), ' ');
}