#include "Main.h"

#include <iostream>
#include <iomanip>

auto main()->int {  
  std::string sourceCode = R""""(
      function main() {
        print 'Hello, World!';
      }
    )"""";
    
    auto tokenList = scan(sourceCode);
    printTokenList(tokenList);

    return 0;
}

auto printTokenList(std::vector<Token> tokenList)->void {
  std::cout << std::setw(12) << std::left << "KIND" << "STRING" << std::endl;
  std::cout << std::string(23, '-') << std::endl;
  for (auto& token: tokenList)
    std::cout << token << std::endl;
}