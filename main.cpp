#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <string.h>

using namespace std;

class State;
class Transition
{
    State *go_to;
    char input_char;
    char pop;
    string push;

public:
    Transition(char input_char, char pop, string push, State *go_to) : go_to(go_to), input_char(input_char), pop(pop), push(push) {}

    // getters
    State *getGo_to()
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
    void print()
    {
        cout << " char: " << input_char << " pop: " << pop << " push: " << push << " goto: " << go_to->getQ();
    }
};

class State
{
    int q; // state number
    vector<Transition *> transitions;
    bool isFinalState = false;

public:
    State(int q) : q(q) {}

    // getters
    Transition *getTransition(int t)
    {
        return transitions[t];
    }
    bool FinalState()
    {
        return isFinalState;
    }
    int getQ()
    {
        return q;
    }
    // setters
    void setFinalStateTrue()
    {
        this->isFinalState = true;
    }
    void setNewTransition(char input_char, char pop, string push, State *go_to)
    {
        this->transitions.push_back(new Transition(input_char, pop, push, go_to));
    }
    void print()
    {
        cout << "q: " << q << " is final: " << isFinalState << endl;
        for (int i = 0; i < transitions.size(); i++)
        {
            transitions[i]->print();
            cout << endl;
        }
    }
    vector<Transition*> getAllTrasitionsByChar(char c)
    {
        vector<Transition*> transitionsByChar;
        for(int i = 0; i < this->transitions.size(); i++) {
            if(transitions[i]->getInput_char() == c) {
                transitionsByChar.push_back(transitions[i]);
            }
        }
        return transitionsByChar;
    }
};

void setAllStateTransistions(vector<State *> states, int T)
{
    for (int i = 0; i < T; i++)
    {
        string transition_input;
        getline(cin >> ws, transition_input);

        int state_num, go_to;
        char input_char, pop;
        string push;

        state_num = transition_input[0] - '0';
        input_char = transition_input[2];
        pop = transition_input[4];
        if (transition_input[7] == ' ')
        {
            push = transition_input[6];
        }
        else
        {
            push = transition_input.substr(6, 2);
        }
        go_to = transition_input.at(transition_input.length() - 1) - '0';

        states[state_num]->setNewTransition(input_char, pop, push, states[go_to]);
    }
}

void setFinalStates(vector<State *> states)
{
    int numFinalStates, final;
    cin >> numFinalStates;
    for (int i = 0; i < numFinalStates; i++)
    {
        cin >> final;
        states[final]->setFinalStateTrue();
    }
}

bool testWord(State* currentState, string remainingWord, stack<char> *s ) {
    if(remainingWord.compare("")){
        if(currentState->FinalState()) {
            return true;
        }
        return false;
    }
    vector<Transition*> t = currentState->getAllTrasitionsByChar(remainingWord[0]);
    for(int i = 0; i < t.size(); i++ ) {
        if(t[i]->getPop() == '&' or t[i]->getPop() == s->top()) {
            s->pop();
            for(int j = t[i]->getPush().length(); j > 0; j--) {
                s->push(t[i]->getPush()[j]);
            }
            return testWord(t[i]->getGo_to(), remainingWord.substr(1, remainingWord.length()-1), s);
        }
    }
    return false;
}


int main()
{
    int Q, T;
    stack<char> *s;
    s->push('Z');
    vector<State *> states;

    cin >> Q;
    cin >> T;

    // break into create states()
    for (int i = 0; i < Q; i++)
    {
        states.push_back(new State(i));
    }

    setAllStateTransistions(states, T);

    setFinalStates(states);

    string word;
    cin >> word;
    while (word != "*")
    {
        cin >> word;
        if(testWord(states[0],word, s)) {
            cout << "verdade" << endl;
        }
        else
            cout << "mentira" << endl;
    }

    // TODO:
    //      - states.getTransitions(char);
    //      - transições que aceitam o char sendo consumido agora E também as transições vazias

    return 0;
}