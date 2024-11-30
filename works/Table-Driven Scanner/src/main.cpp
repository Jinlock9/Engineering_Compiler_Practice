#include <iostream>
#include <cctype>
#include <string>

#define DEBUG 0

static int i = 0;

enum Category {
    Register, Digit, Other
};

enum State {
    s0, s1, s2, se
};

State delta[4][3] = {
    {s1, se, se},
    {se, s2, se},
    {se, s2, se},
    {se, se, se}
};

char NextChar(std::string str) {
    return str[i++];
}

Category CharCat(char ch) {
    unsigned char ascii = static_cast<unsigned char> (ch);
    if (ascii > 47 && ascii < 58) {
        return Digit;
    } else if (ascii == 114) {
        return Register;
    } else {
        return Other;
    }
}

#if DEBUG
void printState(State st) {
    if (st == s0) std::cout << "s0" << std::endl;
    else if (st == s1) std::cout << "s1" << std::endl;
    else if (st == s2) std::cout << "s2" << std::endl;
    else std::cout << "se" << std::endl;
}
#endif

int main(int argc, char **argv) {
    if (argc != 2) return -1;
    
    State state = s0;
    std::string lexeme = "";
    std::string raw = argv[1];

    while (state != se && i != (int)raw.length()) {
        char ch = NextChar(raw);
        lexeme = lexeme + ch;
        Category cat = CharCat(ch);
        state = delta[state][cat];
    } 

    if (state == s2) std::cout << lexeme << std::endl;

    return 0;
}