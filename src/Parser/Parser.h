#ifndef __PARSER_H__
#define __PARSER_H__

#include <vector>

#include "../Scanner/Token.h"
#include "Node.h"

static auto parseFunction()->Function *;
static auto parseBlock()->std::vector<Statement *>;
static auto parseVariable()->Variable *;
static auto parseFor()->For *;
static auto parseIf()->If *;
static auto parsePrint()->Print *;
static auto parseReturn()->Return *;
static auto parseBreak()->Break *;
static auto parseContinue()->Continue *;
static auto parseExpressionStatement()->ExpressionStatement *;
static auto parseExpression()->Expression *;
static auto parseAssignment()->Expression *;
static auto parseOr()->Expression *;
static auto parseAnd()->Expression *;
static auto parseRelational()->Expression *;
static auto parseArithmetic1()->Expression *;
static auto parseArithmetic2()->Expression *;
static auto parseUnary()->Expression *;
static auto parseOperand()->Expression *;
static auto parseNullLiteral()->Expression *;
static auto parseBooleanLiteral()->Expression *;
static auto parseNumberLiteral()->Expression *;
static auto parseStringLiteral()->Expression *;
static auto parseListLiteral()->Expression *;
static auto parseMapLiteral()->Expression *;
static auto parseIdentifier()->Expression *;
static auto parseInnerExpression()->Expression *;
static auto parsePostfix(Expression *)->Expression *;
static auto parseCall(Expression *)->Expression *;
static auto parseElement(Expression *)->Expression *;
static auto skipCurrent()->void;
static auto skipCurrent(Kind)->void;
static auto skipCurrentIf(Kind)->bool;

#endif // __PARSER_H__