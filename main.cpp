#include <iostream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

class Transition
{
    int go_to;
    char input_char;
    char pop;
    string push;

public:
    Transition(int go_to, char input_char, char pop, string push) : go_to(go_to), input_char(input_char), pop(pop), push(push) {}

    // getters
    int getGo_to()
    {
        return this->go_to;
    }
    char getInput_char()
    {
        return this->input_char;
    }
    char getPop()
    {
        return this->pop;
    }
    string getPush()
    {
        return this->push;
    }
};

class State
{
    int q; // state number
    vector<Transition> transitions;
    bool isFinalState = false;

public:
    State(int q, int numOfTransisions) : q(q) {}
    
    // setters
    void setFinalStateTrue() {
        this->isFinalState = true;
    }
    void setNewTransition(int go_to, char input_char, char pop, string push) {
        Transition *t = new Transition(go_to, input_char, pop, push);
        this->transitions.push_back(*t);
    }
};

int main()
{
    int Q, T;
    stack<char> s;
    vector<State> states;

    cin >> Q;
    cin >> T;

    

    return 0;
}