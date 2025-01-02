#include "Main.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

auto main(int argc, char** argv)->int {  
    if (argc < 2) {
        std::cout << "No source file given" << std::endl;
        return -1;
    }
    std::ifstream sourceFile(argv[1]);
    if (!sourceFile.is_open()) {  
        std::cerr << "Failed to open the source file.\n";
        return -1;
    }

    std::stringstream sourceCode;
    sourceCode << sourceFile.rdbuf(); 

    std::cout << "[1] Run Scanner" << std::endl;
    auto tokenList = scan(sourceCode.str());
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
    printObjectCode(objectCode);
    std::cout << std::endl;

    std::cout << "[5] Execute Code" << std::endl;
    execute(objectCode);
    std::cout << std::endl;

    return 0;
}