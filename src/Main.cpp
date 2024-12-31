#include "Main.h"

#include <iostream>
#include <iomanip>

auto main()->int {  
    std::string sourceCode = R""""(
      function main() {
        printLine 'Hello, World!';
        printLine 1 + 2 * 3;
      }
    )"""";

    std::cout << "[1] Run Scanner" << std::endl;
    auto tokenList = scan(sourceCode);
    printTokenList(tokenList);
    std::cout << std::endl;

    std::cout << "[2] Run Parser" << std::endl;
    auto syntaxTree = parse(tokenList);
    printSyntaxTree(syntaxTree);
    std::cout << std::endl;

    std::cout << "[3] Run Interpreter" << std::endl;
    interpret(syntaxTree);
    std::cout << std::endl;

    std::cout << "[4] Run Code Generator" << std::endl;
    auto objectCode = generate(syntaxTree);
    std::cout << std::endl;

    return 0;
}

auto printTokenList(std::vector<Token> tokenList)->void {
    std::cout << std::setw(12) << std::left << "KIND" << "STRING" << std::endl;
    std::cout << std::string(23, '-') << std::endl;
    for (auto& token: tokenList)
      std::cout << token << std::endl;
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