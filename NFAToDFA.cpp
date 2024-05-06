#include <queue>
#include <deque>
#include <algorithm>
#include "NFAToDFA.hpp"
#include "FA.hpp"

template<typename T> T* NFAToDFA::reachable(T* fa)
{
   int initialValue = fa->getStartValue();
   std::priority_queue<int> pq;
   std::unordered_map<int, bool> visited;
   pq.push(initialValue);
  
   for(int i = 0; i < ALPHABET_SIZE; i++)
   {
       for(auto& elem : fa->getStates(i))
       {
           visited[elem.first] = false;
       }
   }

   while(!pq.empty())
   {
       if(visited[pq.top()] == false)
       {
            visited[pq.top()] = true;
            fa->getNodeNeighbors(pq.top(), pq);
       }
       pq.pop();
   }

   for(auto& elem : visited)
   {
       if(elem.second == false)
       {
           fa->removeNode(elem.first); 
       }
   }

   return fa;
}

NFA* NFAToDFA::DFAToNFA(DFA* dfa)
{
    NFA* nfa = new NFA(dfa->getStartValue(), dfa->getEndValue());

    for(int i = 0; i < (dfa->getEndValues()).size(); i++)
    {
        nfa->addEndValue((dfa->getEndValues())[i]);
    }

    for(int i = 0; i < ALPHABET_SIZE; i++)
    {
        for(auto& elem : dfa->getStates(i))
        {
            nfa->addTransition(elem.first, elem.second, i);
        }
    }

    return nfa;
}

template<typename T> T* NFAToDFA::reverse(T* fa)
{
    fa->setEndValue(fa->getStartValue());
    fa->setStartValue(0);

    for(int i = 0; i < fa->getEndValues().size(); i++)
        fa->addTransition(0, (fa->getEndValues())[i], LAMBDA);

    fa->reverseKeyValue();

    fa->clearEndValues();
    fa->addEndValue(fa->getEndValue());

    return fa;
}

void NFAToDFA::lambda_closure(const NFA* nfa, const int value, std::set<int>& s)
{
    std::vector<int> v = (nfa->getStates(LAMBDA))[value];

    for(int i = 0; i < v.size(); i++)
    {
        s.insert(v[i]);
    }
}

void NFAToDFA::lambda_closure(const NFA* nfa, const std::set<int>* values, std::set<int>& s)
{
    for(auto elem : *values)
    {
        lambda_closure(nfa, elem, s);
    }
}

std::set<int>* NFAToDFA::delta(const NFA* nfa, const std::set<int>& s, const int c)
{
    std::set<int>* result = new std::set<int>();
    std::unordered_map<int, std::vector<int>> new_map = nfa->getStates(c);

    for(auto elem: s)
    {
        for(int j = 0; j < new_map[elem].size(); j++)
        {
            result->insert(new_map[elem][j]);
        }
    }

    return result;
}

DFA* NFAToDFA::subset(NFA* nfa)
{
    std::set<int> q0;
    int currentState = 0;
    lambda_closure(nfa, nfa->getStartValue(), q0);
    std::deque<std::set<int>> q;
    std::deque<std::set<int>> worklist;
    std::deque<std::set<int>> prevSets;
    std::unordered_map<int, int> transitions[ALPHABET_SIZE - 1];
    q.push_back(q0);
    worklist.push_back(q0);

    while(!worklist.empty())
    {
        std::set<int> current = worklist.front();
        worklist.pop_front();

        for(int i = 0; i < ALPHABET_SIZE - 1; i++)
        {
            std::set<int> t;
            lambda_closure(nfa, delta(nfa, current, i), t);
            if(std::find(prevSets.begin(), prevSets.end(), current) == prevSets.end())
            {
                transitions[i][currentState++] = currentState++;
                prevSets.push_back(current);
            
                if(std::find(q.begin(), q.end(), t) == q.end())
                {
                    q.push_back(t);
                    worklist.push_back(t); 
                }
            }
        }
    }

    DFA* new_dfa = new DFA(0, currentState);
    for(int i = 0; i < ALPHABET_SIZE - 1; i++)
    {
        for(auto& elem : transitions[i])
        {
            new_dfa->addTransition(elem.first, elem.second, i);
        }
    }

    return new_dfa;
}

DFA* NFAToDFA::BrzozowskiAlgo()
{
    return reachable(subset(reverse(DFAToNFA(reachable(subset(reverse(this->nfa)))))));
}
