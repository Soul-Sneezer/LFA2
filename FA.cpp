#include "FA.hpp"
#include <ostream>
#include <vector>
#include <unordered_map>
    void FA::clearEndValues()
    {
        endValues.clear();
    }

    void FA::addEndValue(int value)
    {
        endValues.push_back(value);
    }

    void DFA::removeNode(int node)
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            this->states[i].erase(node);
        }
    }

    void DFA::getNodeNeighbors(int node, std::priority_queue<int>& neighbors)
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            neighbors.push(this->states[i][node]);
        }
    }

    void DFA::reverseKeyValue()
    {
        for(int i = 0; i < ALPHABET_SIZE; i++)
        {
            std::unordered_map<int, int> new_map;

            for(auto const& elem : this->states[i])
            {
                new_map[elem.second] = elem.first;
            }

            this->states[i] = new_map;
        }
    }

    void DFA::addTransition(int from, int to, int c)
    {
        this->states[c][from] = to;
    }

    void DFA::addTransitions(FA* c)
    {
        for(int i = 0; i < ALPHABET_SIZE; i++)
        {
            for (auto const& elem : ((DFA*)c)->getStates(i))
            {
                this->addTransition(elem.first, elem.second, i);
            }
        }
    }

    void DFA::addTransition(int from, int to, FA* c)
    {
       this->addTransitions(c); // add transition from C to current NFA
    }

    void NFA::removeNode(int node)
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            this->states[i].erase(node);
        }
    }

    void NFA::getNodeNeighbors(int node, std::priority_queue<int>& neighbors)
    {
        for(int i = 0; i < ALPHABET_SIZE; i++)
        {
            for(int j = 0; j < this->states[i][node].size(); j++)
            {
                neighbors.push(this->states[i][node][j]);
            }
        }
    }

    void NFA::reverseKeyValue()
    {
        for(int i = 0; i < ALPHABET_SIZE; i++)
        {
            std::unordered_map<int, std::vector<int>> new_map;
            for(auto const& elem : this->states[i])
            {
                for(int j = 0; j < elem.second.size(); j++)
                {
                    new_map[elem.second[j]].push_back(elem.first);
                }
            }
            this->states[i] = new_map;
        }
    }

    void NFA::addTransition(int from, int to, int c)
    {
        this->states[c][from].push_back(to);
    }

    void NFA::addTransitions(FA* c)
    {
        for(int i = 0; i < ALPHABET_SIZE; i++)
        {
            for (auto const& elem : ((NFA*)c)->getStates(i))
            {
                for(int j = 0; j < elem.second.size(); j++)
                    this->addTransition(elem.first, elem.second[j], i);
            }
        }
    }

    void NFA::addTransition(int from, int to, FA* c)
    {
       this->addTransitions(c); // add transition from C to current NFA
    }

    void NFA::addLambdaEndStart()
    {
        this->addTransition(endValue, startValue, LAMBDA);
    }

    FA& FA::operator=(const FA& fa)
    {
        this->startValue = fa.getStartValue();
        this->endValue = fa.getEndValue();
        this->endValues = fa.getEndValues();

        return *this;
    }

    NFA& NFA::operator=(const NFA& nfa)
    {
        this->startValue = nfa.getStartValue();
        this->endValue = nfa.getEndValue();
        this->endValues = nfa.getEndValues();

        for(int i = 0; i <= ALPHABET_SIZE; i++)
        {
            this->states[i] = nfa.getStates(i);
        }

        return *this;
    }

    DFA& DFA::operator=(const DFA& dfa)
    {
        this->startValue = dfa.getStartValue();
        this->endValue = dfa.getEndValue();
        this->endValues = dfa.getEndValues();
        
        for(int i = 0; i <= ALPHABET_SIZE; i++)
        {
            this->states[i] = dfa.getStates(i);
        }

        return *this;
    }
