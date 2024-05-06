#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stdio.h>
#include "FA.hpp"

class Parser
{
private:
    std::string s;
    int index;
    int stateValue;

    /* helper functions */

    bool isElem(char c);
    bool isDigit(char c);

    bool isAtEnd();

    char currentChar();
    char nextChar();
    void consume();

    int parseNumber();

    NFA* makeElement(char elem);
    NFA* makeClosure(NFA* a, int value);
    NFA* makeConcatenation(NFA* a, NFA* b);
    NFA* makeAlternation(NFA* a, NFA* b);
    
    /* actual parsing */

    NFA* element();
    NFA* parentheses();
    NFA* closure();
    NFA* concatenation();
    NFA* alternation();
public:
    Parser()
    {
        this->stateValue = 0;
    }

    Parser(std::string& s)
    {
        this->s = s;
        this->stateValue = 0;
    }

    NFA* parseRE();
};
