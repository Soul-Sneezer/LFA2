#include <fstream>
#include "FA.hpp"
#include "REToNFA.hpp"

std::ifstream in("input.in");
std::ofstream out("output.out");

int main()
{
    std::string RE;
    in>>RE;
    Parser parser(RE);
    NFA* nfa = parser.parseRE(); 
    std::cout<<*nfa;

    free(nfa);
}
