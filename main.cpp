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
        cout << " char: " << input_char << " pop: " << pop << " push: " << push << " goto: ";
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
    vector<Transition *> getAllTrasitionsByChar(char c)
    {
        vector<Transition *> transitionsByChar;
        for (int i = 0; i < this->transitions.size(); i++)
        {
            if (transitions[i]->getInput_char() == c or transitions[i]->getInput_char() == '&')
            {
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
        int state_num, go_to;
        char input_char, pop;
        string push;

        string transition_input;
        getline(cin >> ws, transition_input);

        int spaceCount = 0;
        int lastParameter = 0;
        for (int j = 0; j < transition_input.length() + 1; j++)
        {
            if (transition_input[j] == ' ' or transition_input[j] == '\0')
            {
                spaceCount++;
                // estado em que a transicao esta
                if (spaceCount == 1)
                {
                    state_num = stoi(transition_input.substr(0, j));
                    lastParameter = i + 1;
                }
                // char consumido
                if (spaceCount == 2)
                {
                    input_char = transition_input[j - 1];
                    lastParameter = j + 1;
                }
                // pop
                if (spaceCount == 3)
                {
                    pop = transition_input[j - 1];
                    lastParameter = j + 1;
                }
                // push
                if (spaceCount == 4)
                {
                    push = transition_input.substr(lastParameter, j - lastParameter);
                    lastParameter = j + 1;
                }
                // goto
                if (spaceCount == 5)
                {
                    go_to = stoi(transition_input.substr(lastParameter, j - lastParameter));
                }
            }
        }
        states[state_num]->setNewTransition(input_char, pop, push, states[go_to]);
        // cout << "state num: " << state_num << ", char consumido: " << input_char << ", pop: " << pop << ", push: " << push << ", goto: " << go_to << endl;
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

bool isFinalStateAndEndOfWord(bool isFinalState, string remaningWord)
{
    if (isFinalState and remaningWord == "")
    {
        return true;
    }
    return false;
}

bool isStackTopAcceptedByTransition(char pop, char stackTop)
{
    if (pop == '&' or pop == stackTop)
    {
        return true;
    }
    return false;
}

bool isPopNotEmpty(char pop)
{
    if (pop != '&')
    {
        return true;
    }
    return false;
}

bool isPushNotEmpty(char push)
{
    if (push != '&')
    {
        return true;
    }
    return false;
}

string nextWord(char c, string remainingWord)
{
    if (c == '&')
    {
        return remainingWord;
    }
    return remainingWord.substr(1, remainingWord.length() - 1);
}

bool testWord(State *currentState, string remainingWord, stack<char> s)
{
    if (s.empty())
    {
        return false;
    }
    if (isFinalStateAndEndOfWord(currentState->FinalState(), remainingWord))
    {
        return true;
    }
    vector<Transition *> t = currentState->getAllTrasitionsByChar(remainingWord[0]);
    stack<char> CurrentStack = s;

    for (int i = 0; i < t.size(); i++)
    {
        s = CurrentStack;
        if (isStackTopAcceptedByTransition(t[i]->getPop(), s.top()))
        {
            if (isPopNotEmpty(t[i]->getPop()))
            {
                s.pop();
            }

            if (isPushNotEmpty(t[i]->getPush()[0]))
            {
                for (int k = t[i]->getPush().length() - 1; k >= 0; k--)
                {
                    // push de cada char da string na pilha
                    s.push(t[i]->getPush()[k]);
                }
            }

            string next_word = nextWord(t[i]->getInput_char(), remainingWord);
            if (testWord(t[i]->getGo_to(), next_word, s))
            {
                return true;
            }
        }
    }
    return false;
}

void createStates(vector<State *> *states, int Q)
{
    for (int i = 0; i < Q; i++)
    {
        states->push_back(new State(i));
    }
}

int main()
{
    int Q, T;

    vector<State *> states;

    cin >> Q;
    cin >> T;

    createStates(&states, Q);

    setAllStateTransistions(states, T);

    setFinalStates(states);

    string word = "";
    cin >> word;

    while (word != "*")
    {
        stack<char> s;
        s.push('Z');
        if (testWord(states[0], word, s))
            cout << word << ": sim" << endl;

        else
            cout << word << ": nao" << endl;
        cin >> word;
    }

    return 0;
}