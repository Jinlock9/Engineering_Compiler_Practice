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