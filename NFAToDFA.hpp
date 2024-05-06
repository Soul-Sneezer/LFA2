#ifndef LFA_NFATODFA
#define LFA_NFATODFA

#include <set>
#include "FA.hpp"

class NFAToDFA
{
    private:
        NFA* nfa;
       
        std::set<int>* delta(const NFA* nfa, const std::set<int>& s, int c);
        void lambda_closure(const NFA* nfa, const std::set<int>* values, std::set<int>& s);
        void lambda_closure(const NFA* nfa, const int value, std::set<int>& s);
        NFA* DFAToNFA(DFA* dfa);
        template<typename T> T* reverse(T* fa);
        template<typename T> T* reachable(T* fa);
    public:
        NFAToDFA()
        {
            nfa = NULL;
        }

        NFAToDFA(NFA* nfa)
        {
            this->nfa = nfa;
        }
        
        DFA* subset(NFA* nfa);
        DFA* BrzozowskiAlgo();
};

#endif
