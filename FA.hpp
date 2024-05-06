#ifndef LFA_FA
#define LFA_FA

#include <ostream>
#include <unordered_map>
#include <vector>
#include <queue>

const int LAMBDA = 26;
const int ALPHABET_SIZE = 27;

class FA
{
protected:
    int startValue;
    int endValue;

    std::vector<int> endValues;
public:
    int getStartValue() const
    {
        return this->startValue;
    }

    int getEndValue() const
    {
        return this->endValue;
    }

    std::vector<int> getEndValues() const
    {
        return this->endValues;
    }
   
    void setStartValue(int value)
    {
        this->startValue = value;
    }

    void setEndValue(int value)
    {
        this->endValue = value;
    }

    void clearEndValues();
    void addEndValue(int value);

    virtual void removeNode(int node) = 0;
    virtual void getNodeNeighbors(int node, std::priority_queue<int>& neighbors) = 0;
    virtual void reverseKeyValue() = 0;
    virtual void addTransition(int from, int to, int c) = 0;
    virtual void addTransitions(FA* c) = 0;
    virtual void addTransition(int from, int to, FA* c) = 0;
    
    FA& operator=(const FA& fa);

    friend std::ostream& operator<<(std::ostream& os, const FA& fa)
    {
        os<<"FA Start: "<<fa.getStartValue()<<"\n";
        os<<"FA End: "<<fa.getEndValue()<<"\n\n";

        return os;
    }

    FA()
    {

    }

    FA(int startValue, int endValue)
    {
        this->startValue = startValue;
        this->endValue = endValue;
    }

    FA(const FA& fa)
    {
        this->startValue = fa.getStartValue();
        this->endValue = fa.getEndValue();
    }
};

class DFA : public FA
{
private:
    std::unordered_map<int, int> states[ALPHABET_SIZE];
public:
    std::unordered_map<int, int> getStates(int index) const 
    {
        if(index >= ALPHABET_SIZE)
            return this->states[0];
        return this->states[index];
    }

    void removeNode(int node);
    void getNodeNeighbors(int node, std::priority_queue<int>& neighbors) override; 
    void reverseKeyValue() override;
    void addTransition(int from, int to, int c) override;
    void addTransitions(FA* c) override;
    void addTransition(int from, int to, FA* c) override;

    DFA& operator=(const DFA& dfa);

    friend std::ostream& operator<<(std::ostream& os, const DFA& dfa)
    {
        os<<"DFA Start: "<<dfa.getStartValue()<<"\n";
        os<<"DFA End: "<<dfa.getEndValue()<<"\n\n";
        for(int i = 0; i < ALPHABET_SIZE; i++)
        {
            for(auto const& elem: dfa.getStates(i))
            {
                os<<elem.first<<" "<<(char)(i + 'a')<<"->"<<elem.second<<"\n";
            }
        }

        return os;
    }

    DFA()
    {

    }

    DFA(int startValue)
    {
        this->startValue = startValue;
    }

    DFA(int startValue, int endValue)
    {
        this->startValue = startValue;
        this->endValue = endValue;
    }

    DFA(const DFA& dfa)
    {
        this->startValue = dfa.getStartValue();
        this->endValue = dfa.getEndValue();
        for(int i = 0; i < ALPHABET_SIZE; i++)
        {
            this->states[i] = dfa.getStates(i);
        }
    }
};

class NFA : public FA
{
private:
    std::unordered_map<int, std::vector<int>> states[ALPHABET_SIZE];
public:
    std::unordered_map<int, std::vector<int>> getStates(int index) const
    {
        if(index > ALPHABET_SIZE)
            return this->states[0];
        return this->states[index];
    }

    void removeNode(int node);
    void getNodeNeighbors(int node, std::priority_queue<int>& neighbors) override; 
    void reverseKeyValue() override;
    void addTransition(int from, int to, int c) override;
    void addTransitions(FA* c) override;
    void addTransition(int from, int to, FA* c) override;

    void addLambdaEndStart();
    NFA& operator=(const NFA& nfa);

    friend std::ostream& operator<<(std::ostream& os, const NFA& nfa)
    {
        os<<"NFA Start: "<<nfa.getStartValue()<<"\n";
        os<<"NFA End: "<<nfa.getEndValue()<<"\n\n";
        for(int i = 0; i < ALPHABET_SIZE; i++)
        {
            for(auto const& elem: nfa.getStates(i))
            {
                for(int j = 0; j < elem.second.size(); j++)
                {
                    os<<elem.first<<" "<<(char)(i + 'a')<<"->"<<elem.second[j]<<"\n";
                }
            }
        }

        return os;
    }

    NFA()
    {

    }

    NFA(int startValue, int endValue)
    {
        this->startValue = startValue;
        this->endValue = endValue;
    }

    NFA(const NFA& nfa)
    {
        this->startValue = nfa.getStartValue();
        this->endValue = nfa.getEndValue();
    
        for(int i = 0; i < ALPHABET_SIZE; i++)
        {
            this->states[i] = nfa.getStates(i);
        }
    }

};

#endif
