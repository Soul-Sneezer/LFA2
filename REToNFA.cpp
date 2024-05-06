#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stdio.h>
#include "FA.hpp"
#include "REToNFA.hpp"

    /* helper functions */
    bool Parser::isElem(char c)
    {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
    }

    bool Parser::isDigit(char c)
    {
        return c >= '0' &&  c <= '9';
    }

    bool Parser::isAtEnd()
    {
        return index >= s.length();
    }

    char Parser::currentChar()
    {
        return s[index];
    }
    char Parser::nextChar()
    {
        return s[index++];
    }

    void Parser::consume()
    {
        index++;
    }

    int Parser::parseNumber()
    {
        int nr = 0;
        while(isDigit(currentChar()))
        {
            nr = nr * 10 + (nextChar() - '0');
        }

        return nr;
    }

    NFA* Parser::makeElement(char elem)
    {
        int startValue = stateValue++;
        int endValue = stateValue++;
        NFA* result = new NFA(startValue, endValue); 
        result->addTransition(startValue, endValue, elem - 'a');
            
        return result;
    }

    NFA* Parser::makeClosure(NFA* a, int value)
    {
        int startValue = stateValue++;
        int endValue = stateValue++;
        
        NFA* result = new NFA(startValue, endValue);

        if(value == 0)
            return a;
        else if(value == -1) // +
        {
            a->addLambdaEndStart();
            result->addTransition(startValue, a->getStartValue(), LAMBDA);
            result->addTransition(startValue, endValue, a);
            result->addTransition(a->getEndValue(), endValue, LAMBDA);
        }
        else if(value == -2) // *
        {
            a->addLambdaEndStart();
            result->addTransition(startValue, a->getStartValue(), LAMBDA);
            result->addTransition(startValue, endValue, a);
            result->addTransition(startValue, endValue, LAMBDA);
            result->addTransition(a->getEndValue(), endValue, LAMBDA);
        }
        else
        {
            while(value > 0)
            {
                makeConcatenation(result, a); // this should work
                value--;
            }
        }

        return result;
    }

    NFA* Parser::makeConcatenation(NFA* a, NFA* b)
    {
        NFA* result = new NFA(a->getStartValue(), b->getEndValue());

        result->addTransitions(a);
        result->addTransition(a->getEndValue(), b->getStartValue(), LAMBDA);
        result->addTransitions(b);

        return result;
    }

    NFA* Parser::makeAlternation(NFA* a, NFA* b)
    {
        int startValue = stateValue++;
        int endValue = stateValue++;
        
        NFA* result = new NFA(startValue, endValue);
        result->addTransition(startValue, a->getStartValue(), LAMBDA);
        result->addTransition(startValue, b->getStartValue(), LAMBDA);
        result->addTransitions(a);
        result->addTransitions(b);
        result->addTransition(a->getEndValue(), endValue, LAMBDA);
        result->addTransition(b->getEndValue(), endValue, LAMBDA);

        return result;
    }

    NFA* Parser::element()
    {
        return makeElement(nextChar());
    }

    NFA* Parser::parentheses()
    {
        NFA* result = new NFA();
        
        if(currentChar() == '(')
        {
            consume();
            result = alternation();
            consume(); // trailing ')'
        }
        else
        {
            return element();
        }

        return result;
    }

    NFA* Parser::closure()
    {
        NFA* a = parentheses();
        if(currentChar() == '^')
        {
            consume();
            if(isDigit(currentChar()))
            {
                int nr = parseNumber();
                return makeClosure(a, nr);
            }
            if(currentChar() == '+')
            {
                consume();
                return makeClosure(a, -1);
            }
            if(currentChar() == '*')
            {
                consume();
                return makeClosure(a, -2);
            }
        }
        
        return a;
    }

    NFA* Parser::concatenation()
    {
        NFA* a = closure();
        if(isElem(currentChar()))
        {
            NFA* b = closure();
            return makeConcatenation(a, b);
        }

        return a;
    }

    NFA* Parser::alternation()
    {
        NFA* a = concatenation();
        if(currentChar() == '|')
        {
            consume();
            NFA * b = concatenation();
            a = makeAlternation(a, b);
            free(b);
        }

        return a;
    }

    NFA* Parser::parseRE()
    {
        NFA* result = new NFA();
        result = alternation();
        while(!isAtEnd())
        {
            NFA* a = alternation();
            result = makeConcatenation(result, a); 
        }

        return result;
    }
