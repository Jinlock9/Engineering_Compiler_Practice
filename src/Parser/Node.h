#ifndef __NODE_H__
#define __NODE_H__

#include "../Scanner/Token.h"

#include <vector>
#include <string>
#include <map>

struct Program {
    std::vector<struct Function *> functions;
};

struct Statement {};
struct Expression {};

struct Function: Statement {
    std::string name;
    std::vector<std::string> parameters;
    std::vector<Statement *> block;
};

struct Return: Statement {
    Expression *expression;
};

struct Variable: Statement {
    std::string name;
    Expression *expression;
};

struct For: Statement {
    Variable *variable;
    Expression *condition;
    Expression *expression;
    std::vector<Statement *> block;
};

struct Break: Statement {};
struct Continue: Statement {};

struct If: Statement {
    std::vector<Expression *> conditions;
    std::vector<std::vector<Statement *>> blocks;
    std::vector<Statement *> elseBlock;
};

struct ExpressionStatement: Statement {
    Expression *expression;
};

struct Or: Expression {
    Expression *lhs;
    Expression *rhs;
};

struct And: Expression {
    Expression *lhs;
    Expression *rhs;
};

struct Relational: Expression {
    Kind kind;
    Expression *lhs;
    Expression *rhs;
};

struct Arithmetic: Expression {
    Kind kind;
    Expression *lhs;
    Expression *rhs;
};

struct Unary: Expression {
    Kind kind;
    Expression *expression;
};

struct Call: Expression {
    Expression *sub;
    std::vector<Expression *> arguments;
};

struct GetElement: Expression {
    Expression *sub;
    Expression *index;
};

struct SetElement: Expression {
    Expression *sub;
    Expression *index;
    Expression *value;
};

struct GetVariable: Expression {
    std::string name;
};

struct SetVariable: Expression {
    std::string name;
    Expression *value;
};

struct NullLiteral: Expression {};
struct BooleanLiteral: Expression {
    bool value = false;
};
struct NumberLiteral: Expression {
    double value = 0.0;
};
struct StringLiteral: Expression {
    std::string value;
};

struct ArrayLiteral: Expression {
    std::vector<Expression *> values;
};

struct MapLiteral: Expression {
    std::map<std::string, Expression *> values;
};

#endif // __NODE_H__