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
  auto tokenList = scan(sourceCode);
  auto syntaxTree = parse(tokenList);

  return 0;
}

auto printTokenList(std::vector<Token> tokenList)->void {
  std::cout << std::setw(12) << std::left << "KIND" << "STRING" << std::endl;
  std::cout << std::string(23, '-') << std::endl;
  for (auto& token: tokenList)
    std::cout << token << std::endl;
}