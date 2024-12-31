#ifndef __PARSER_H__
#define __PARSER_H__

#include <vector>

#include "../Scanner/Token.h"
#include "Node.h"

auto parse(std::vector<Token>)->Program*;

auto parseFunction()->Function*;
auto parseBlock()->std::vector<Statement*>;
auto parseVariable()->Variable*;
auto parseFor()->For*;
auto parseIf()->If*;
auto parsePrint()->Print*;
auto parseReturn()->Return*;
auto parseBreak()->Break*;
auto parseContinue()->Continue*;
auto parseExpressionStatement()->ExpressionStatement*;
auto parseExpression()->Expression*;
auto parseAssignment()->Expression*;
auto parseOr()->Expression*;
auto parseAnd()->Expression*;
auto parseRelational()->Expression*;
auto parseArithmetic1()->Expression*;
auto parseArithmetic2()->Expression*;
auto parseUnary()->Expression*;
auto parseOperand()->Expression*;
auto parseNullLiteral()->Expression*;
auto parseBooleanLiteral()->Expression*;
auto parseNumberLiteral()->Expression*;
auto parseStringLiteral()->Expression*;
auto parseListLiteral()->Expression*;
auto parseMapLiteral()->Expression*;
auto parseIdentifier()->Expression*;
auto parseInnerExpression()->Expression*;
auto parsePostfix(Expression*)->Expression*;
auto parseCall(Expression*)->Expression*;
auto parseElement(Expression*)->Expression*;
auto skipCurrent()->void;
auto skipCurrent(Kind)->void;
auto skipCurrentIf(Kind)->bool;

auto printSyntaxTree(Program*)->void;

#endif // __PARSER_H__