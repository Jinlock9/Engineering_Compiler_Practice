#include "Parser.h"

#include <vector>
#include <iostream>
#include <set>

static std::vector<Token>::iterator current;

auto parse(std::vector<Token> tokens)->Program* {
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

auto parseFunction()->Function* {
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

auto parseBlock()->std::vector<Statement*> {
    std::vector<Statement*> result;
    while (current->kind != Kind::RightBrace) {
        switch (current->kind) {
            case Kind::Variable:
                result.push_back(parseVariable());
                break;
            case Kind::For:
                result.push_back(parseFor());
                break;
            case Kind::If:
                result.push_back(parseIf());
                break;
            case Kind::Print:
            case Kind::PrintLine:
                result.push_back(parsePrint());
                break;
            case Kind::Return:
                result.push_back(parseReturn());
                break;
            case Kind::Break:
                result.push_back(parseBreak());
                break;
            case Kind::Continue:
                result.push_back(parseContinue());
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

auto parseVariable()->Variable* {
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

auto parseFor()->For* {
    auto result = new For();
    skipCurrent(Kind::For);
    result->variable = new Variable();
    result->variable->name = current->string;
    skipCurrent(Kind::Identifier);
    skipCurrent(Kind::Assignment);
    result->variable->expression = parseExpression();
    if (result->variable->expression == nullptr) {
        std::cout << "No Initialization in For statement.";
        exit(1);
    }
    skipCurrent(Kind::Comma);
    result->condition = parseExpression();
    if (result->condition == nullptr) {
        std::cout << "No condition in For statement.";
        exit(1);
    }
    skipCurrent(Kind::Comma);
    result->expression = parseExpression();
    if (result->expression == nullptr) {
        std::cout << "No Inc/Dec expression in For statement.";
        exit(1);
    }
    skipCurrent(Kind::LeftBrace);
    result->block = parseBlock();
    skipCurrent(Kind::RightBrace);
    return result;
}

auto parseIf()->If* {
    auto result = new If();
    skipCurrent(Kind::If);
    do {
        auto condition = parseExpression();
        if (condition == nullptr) {
            std::cout << "No condition in If statement.";
            exit(1);
        }
        result->conditions.push_back(condition);
        skipCurrent(Kind::LeftBrace);
        result->blocks.push_back(parseBlock());
        skipCurrent(Kind::RightBrace);
    } while (skipCurrentIf(Kind::Elif));
    if (skipCurrentIf(Kind::Else)) {
        skipCurrent(Kind::LeftBrace);
        result->elseBlock = parseBlock();
        skipCurrent(Kind::RightBrace);
    }
    return result;
}

auto parsePrint()->Print* {
    auto result = new Print();
    result->lineFeed = current->kind == Kind::PrintLine;
    skipCurrent();
    if (current->kind != Kind::Semicolon) {
        do result->arguments.push_back(parseExpression());
        while (skipCurrentIf(Kind::Comma));
    }
    skipCurrent(Kind::Semicolon);
    return result;
}

auto parseReturn()->Return* {
    auto result = new Return();
    skipCurrent(Kind::Return);
    result->expression = parseExpression();
    if (result->expression == nullptr) {
        std::cout << "No expression in Return.";
        exit(1);
    }
    skipCurrent(Kind::Semicolon);
    return result;
}

auto parseBreak()->Break* {
    auto result = new Break();
    skipCurrent(Kind::Break);
    skipCurrent(Kind::Semicolon);
    return result;
}

auto parseContinue()->Continue* {
    auto result = new Continue();
    skipCurrent(Kind::Continue);
    skipCurrent(Kind::Semicolon);
    return result;
}

auto parseExpressionStatement()->ExpressionStatement* {
    auto result = new ExpressionStatement();
    result->expression = parseExpression();
    skipCurrent(Kind::Semicolon);
    return result;
}

auto parseExpression()->Expression* {
    return parseAssignment();
}

auto parseAssignment()->Expression* {
    auto result = parseOr();
    if (current->kind != Kind::Assignment) return result;
    skipCurrent(Kind::Assignment);
    if (auto getVariable = dynamic_cast<GetVariable*>(result)) {
        auto result = new SetVariable();
        result->name = getVariable->name;
        result->value = parseAssignment();
        return result;
    }
    if (auto getElement = dynamic_cast<GetElement*>(result)) {
        auto result = new SetElement();
        result->sub = getElement->sub;
        result->index = getElement->index;
        result->value = parseAssignment();
        return result;
    }
    std::cout << "Invalid Assignment.";
    exit(1);
}

auto parseOr()->Expression* {
    auto result = parseAnd();
    while (skipCurrentIf(Kind::LogicalOr)) {
        auto temp = new Or();
        temp->lhs = result;
        temp->rhs = parseAnd();
        result = temp;
    }
    return result;
}

auto parseAnd()->Expression* {
    auto result = parseRelational();
    while (skipCurrentIf(Kind::LogicalAnd)) {
        auto temp = new And();
        temp->lhs = result;
        temp->rhs = parseRelational();
        result = temp;
    }
    return result;
}

auto parseRelational()->Expression* {
    std::set<Kind> operators = {
        Kind::Equal,
        Kind::NotEqual,
        Kind::LessThan,
        Kind::GreaterThan,
        Kind::LessOrEqual,
        Kind::GreaterOrEqual,
    };
    auto result = parseArithmetic1();
    while (operators.count(current->kind)) {
        auto temp = new Relational();
        temp->kind = current->kind;
        skipCurrent();
        temp->lhs = result;
        temp->rhs = parseArithmetic1();
        result = temp;
    }
    return result;
}

auto parseArithmetic1()->Expression* {
    std::set<Kind> operators = {
        Kind::Add,
        Kind::Subtract
    };
    auto result = parseArithmetic2();
    while (operators.count(current->kind)) {
        auto temp = new Arithmetic();
        temp->kind = current->kind;
        skipCurrent();
        temp->lhs = result;
        temp->rhs = parseArithmetic2();
        result = temp;
    }
    return result;
}

auto parseArithmetic2()->Expression* {
    std::set<Kind> operators = {
        Kind::Multiply,
        Kind::Divide,
        Kind::Modulo,
    };
    auto result = parseUnary();
    while (operators.count(current->kind)) {
        auto temp = new Arithmetic();
        temp->kind = current->kind;
        skipCurrent();
        temp->lhs = result;
        temp->rhs = parseUnary();
        result = temp;
    }
    return result;
}

auto parseUnary()->Expression* {
    std::set<Kind> operators = {
        Kind::Add,
        Kind::Subtract,
    };
    while (operators.count(current->kind)) {
        auto result = new Unary();
        result->kind = current->kind;
        skipCurrent();
        result->sub = parseUnary();
        return result;
    }
    return parseOperand();
}

auto parseOperand()->Expression* {
    Expression* result = nullptr;
    switch (current->kind) {
        case Kind::NullLiteral:   
            result = parseNullLiteral();      
            break;
        case Kind::TrueLiteral:
        case Kind::FalseLiteral:  
            result = parseBooleanLiteral();   
            break;
        case Kind::NumberLiteral: 
            result = parseNumberLiteral();    
            break;
        case Kind::StringLiteral: 
            result = parseStringLiteral();    
            break;
        case Kind::LeftBracket:    
            result = parseListLiteral();      
            break;
        case Kind::LeftBrace:     
            result = parseMapLiteral();       
            break;
        case Kind::Identifier:    
            result = parseIdentifier();       
            break;
        case Kind::LeftParen:     
            result = parseInnerExpression();  
            break;
        default:                  
            std::cout << "Invalid Expression.";       
            exit(1);
    }
    return parsePostfix(result);
}

auto parseNullLiteral()->Expression* {
    skipCurrent(Kind::NullLiteral);
    auto result = new NullLiteral();
    return result;
}

auto parseBooleanLiteral()->Expression* {
    auto result = new BooleanLiteral();
    result->value = current->kind == Kind::TrueLiteral;
    skipCurrent();
    return result;
}

auto parseNumberLiteral()->Expression* {
    auto result = new NumberLiteral();
    result->value = stod(current->string);
    skipCurrent(Kind::NumberLiteral);
    return result;
}

auto parseStringLiteral()->Expression* {
    auto result = new StringLiteral();
    result->value = current->string;
    skipCurrent(Kind::StringLiteral);
    return result;
}

auto parseListLiteral()->Expression* {
    auto result = new ArrayLiteral();
    skipCurrent(Kind::LeftBracket);
    if (current->kind != Kind::RightBracket) {
        do result->values.push_back(parseExpression());
        while (skipCurrentIf(Kind::Comma));
    }
    skipCurrent(Kind::RightBracket);
    return result;
    }

auto parseMapLiteral()->Expression* {
    auto result = new MapLiteral();
    skipCurrent(Kind::LeftBrace);
    if (current->kind != Kind::RightBrace) {
        do {
            auto name = current->string;
            skipCurrent(Kind::StringLiteral);
            skipCurrent(Kind::Colon);
            auto value = parseExpression();
            result->values[name] = value;
        } while (skipCurrentIf(Kind::Comma));
    }
    skipCurrent(Kind::RightBrace);
    return result;
}

auto parseIdentifier()->Expression* {
    auto result = new GetVariable();
    result->name = current->string;
    skipCurrent(Kind::Identifier);
    return result;
}

auto parseInnerExpression()->Expression* {
    skipCurrent(Kind::LeftParen);
    auto result = parseExpression();
    skipCurrent(Kind::RightParen);
    return result;
}

auto parsePostfix(Expression* sub)->Expression* {
    while (true) {
        switch (current->kind) {
            case Kind::LeftParen:  sub = parseCall(sub);    break;
            case Kind::LeftBracket: sub = parseElement(sub); break;
            default: return sub;
        }
    }
}

auto parseCall(Expression* sub)->Expression* {
    auto result = new Call();
    result->sub = sub;
    skipCurrent(Kind::LeftParen);
    if (current->kind != Kind::RightParen) {
        do result->arguments.push_back(parseExpression());
        while (skipCurrentIf(Kind::Comma));
    }
    skipCurrent(Kind::RightParen);
    return result;
}

auto parseElement(Expression* sub)->Expression* {
    auto result = new GetElement();
    result->sub = sub;
    skipCurrent(Kind::LeftBracket);
    result->index = parseExpression();
    skipCurrent(Kind::RightBracket);
    return result;
}

auto skipCurrent()->void {
    current++;
}

auto skipCurrent(Kind kind)->void {
    if (current->kind != kind) {
        std::cout << toString(kind) + " Token Required.";
        exit(1);
    }
    current++;
}

auto skipCurrentIf(Kind token_kind)->bool {
    if (current->kind != token_kind)
        return false;
    current++;
    return true;
}