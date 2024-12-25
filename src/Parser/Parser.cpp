#include "Parser.h"

#include <vector>
#include <iostream>

static std::vector<Token>::iterator current;

auto parse(std::vector<Token> tokens)->Program * {
    auto result = new Program();
    current = tokens.begin();
    while (current->kind != Kind::EndOfToken) {
        switch (current->kind) {
            case Kind::Function: {
                result->functions.push_back(parseFunction());
                break;
            }
            default: {
                std::cout << *current << "Invalid Structure";
                exit(1);
            }
        }
    }
    return result;
}

auto parseFunction()->Function * {
    auto result = new Function();
    skipCurrent(Kind::Function);
    result->name = current->string;
    skipCurrent(Kind::Identifier);
    skipCurrent(Kind::LeftParen);
    if (current->kind != Kind::RightParen) {
        do {
            result->parameters.push_back(current->string);
            skipCurrent(Kind::Identifier);
        } while (skipCurrentIf(Kind::Comma));
    }
    skipCurrent(Kind::RightParen);
    skipCurrent(Kind::LeftBrace);
    result->block = parseBlock();
    skipCurrent(Kind::RightBrace);
    return result;
}

auto skipCurrent(Kind kind)->void {
    if (current->kind != kind) {
        std::cout << toString(kind) + " token required.";
        exit(1);
    }
    current++;
}

auto parseBlock()->std::vector<Statement *> {
    std::vector<Statement *> result;
    while (current->kind != Kind::RightBrace) {
        switch (current->kind) {
            case Kind::Variable:
                result.push_back(parseVariable());
                break;
            case Kind::EndOfToken:
                std::cout << *current << " Invalid Structure";
                exit(1);
            default:
                result.push_back(parseExpressionStatement());
                break;
        }
    }
    return result;
}

auto parseVariable()->Variable * {
    auto result = new Variable();
    skipCurrent(Kind::Variable);
    result->name = current->string;
    skipCurrent(Kind::Identifier);
    skipCurrent(Kind::Assignment);
    result->expression = parseExpression();
    if (result->expression == nullptr) {
        std::cout << "No initialization for the variable declared.";
        exit(1);
    }
    skipCurrent(Kind::Semicolon);
    return result;
}

auto parseExpressionStatement()->ExpressionStatement * {
    auto result = new ExpressionStatement();
    result->expression = parseExpression();
    skipCurrent(Kind::Semicolon);
    return result;
}

auto parseExpression()->Expression * {
    return parseAssignment();
}

auto parseAssignment()->Expression * {
    auto result = parseOr();
    if (current->kind != Kind::Assignment) return result;
    skipCurrent(Kind::Assignment);
    if (auto getVariable = dynamic_cast<GetVariable *>(result)) {
        auto result = new SetVariable();
        result->name = getVariable->name;
        result->value = parseAssignment();
        return result;
    }
    if (auto getElement = dynamic_cast<GetElement *>(result)) {
        auto result = new SetElement();
        result->sub = getElement->sub;
        result->index = getElement->index;
        result->value = parseAssignment();
        return result;
    }
    std::cout << "Invalid Assignment.";
    exit(1);
}

auto parseOr()->Expression * {
    auto result = parseAnd();
    while (skipCurrentIf(Kind::LogicalOr)) {
        auto temp = new Or();
        temp->lhs = result;
        temp->rhs = parseAnd();
        result = temp;
    }
    return result;
}