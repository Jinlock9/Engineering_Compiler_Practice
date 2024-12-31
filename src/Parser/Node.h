#ifndef __NODE_H__
#define __NODE_H__

#include "../Scanner/Token.h"

#include <any>
#include <vector>
#include <string>
#include <map>

struct Program {
    std::vector<struct Function*> functions;
};

struct Statement {
    virtual auto print(int)->void = 0;
    virtual auto interpret()->void = 0;
    virtual auto generate()->void = 0;
};

struct Expression {
    virtual auto print(int)->void = 0;
    virtual auto interpret()->std::any = 0;
    virtual auto generate()->void = 0;
};

struct Function: Statement {
    std::string name;
    std::vector<std::string> parameters;
    std::vector<Statement*> block;
    auto print(int)->void;
    auto interpret()->void;
    auto generate()->void;
};

struct Return: Statement {
    Expression* expression;
    auto print(int)->void;
    auto interpret()->void;
    auto generate()->void;
};

struct Variable: Statement {
    std::string name;
    Expression* expression;
    auto print(int)->void;
    auto interpret()->void;
    auto generate()->void;
};

struct For: Statement {
    Variable* variable;
    Expression* condition;
    Expression* expression;
    std::vector<Statement*> block;
    auto print(int)->void;
    auto interpret()->void;
    auto generate()->void;
};

struct Break: Statement {
    auto print(int)->void;
    auto interpret()->void;
    auto generate()->void;
};

struct Continue: Statement {
    auto print(int)->void;
    auto interpret()->void;
    auto generate()->void;
};

struct If: Statement {
    std::vector<Expression*> conditions;
    std::vector<std::vector<Statement*>> blocks;
    std::vector<Statement*> elseBlock;
    auto print(int)->void;
    auto interpret()->void;
    auto generate()->void;
};

struct Print: Statement {
    bool lineFeed = false;
    std::vector<Expression*> arguments;
    auto print(int)->void;
    auto interpret()->void;
    auto generate()->void;
};

struct ExpressionStatement: Statement {
    Expression* expression;
    auto print(int)->void;
    auto interpret()->void;
    auto generate()->void;
};

struct Or: Expression {
    Expression* lhs;
    Expression* rhs;
    auto print(int)->void;
    auto interpret()->std::any;
    auto generate()->void;
};

struct And: Expression {
    Expression* lhs;
    Expression* rhs;
    auto print(int)->void;
    auto interpret()->std::any;
    auto generate()->void;
};

struct Relational: Expression {
    Kind kind;
    Expression* lhs;
    Expression* rhs;
    auto print(int)->void;
    auto interpret()->std::any;
    auto generate()->void;
};

struct Arithmetic: Expression {
    Kind kind;
    Expression* lhs;
    Expression* rhs;
    auto print(int)->void;
    auto interpret()->std::any;
    auto generate()->void;
};

struct Unary: Expression {
    Kind kind;
    Expression* sub;
    auto print(int)->void;
    auto interpret()->std::any;
    auto generate()->void;
};

struct Call: Expression {
    Expression* sub;
    std::vector<Expression*> arguments;
    auto print(int)->void;
    auto interpret()->std::any;
    auto generate()->void;
};

struct GetElement: Expression {
    Expression* sub;
    Expression* index;
    auto print(int)->void;
    auto interpret()->std::any;
    auto generate()->void;
};

struct SetElement: Expression {
    Expression* sub;
    Expression* index;
    Expression* value;
    auto print(int)->void;
    auto interpret()->std::any;
    auto generate()->void;
};

struct GetVariable: Expression {
    std::string name;
    auto print(int)->void;
    auto interpret()->std::any;
    auto generate()->void;
};

struct SetVariable: Expression {
    std::string name;
    Expression* value;
    auto print(int)->void;
    auto interpret()->std::any;
    auto generate()->void;
};

struct NullLiteral: Expression {
    auto print(int)->void;
    auto interpret()->std::any;
    auto generate()->void;
};
struct BooleanLiteral: Expression {
    bool value = false;
    auto print(int)->void;
    auto interpret()->std::any;
    auto generate()->void;
};
struct NumberLiteral: Expression {
    double value = 0.0;
    auto print(int)->void;
    auto interpret()->std::any;
    auto generate()->void;
};
struct StringLiteral: Expression {
    std::string value;
    auto print(int)->void;
    auto interpret()->std::any;
    auto generate()->void;
};

struct ArrayLiteral: Expression {
    std::vector<Expression*> values;
    auto print(int)->void;
    auto interpret()->std::any;
    auto generate()->void;
};

struct MapLiteral: Expression {
    std::map<std::string, Expression*> values;
    auto print(int)->void;
    auto interpret()->std::any;
    auto generate()->void;
};

#endif // __NODE_H__