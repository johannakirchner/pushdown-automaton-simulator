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

    for (int i = 0; i < T; i++)
    {
        int state_num, go_to;
        char input_char, pop;
        string push;

        string transition_input;
        getline(cin >> ws, transition_input);

        int spaceCount = 0;
        int lastParameter = 0;
        for (int j = 0; j < transition_input.length(); j++)
        {
            if (transition_input[j] == ' ' or transition_input[j] == '\0')
            {
                spaceCount++;
                // estado em que a transicao esta
                if (spaceCount == 1)
                {
                    state_num = stoi(transition_input.substr(0, i-1));
                    lastParameter = i+1;
                }
                // char consumido
                if (spaceCount == 2)
                {
                    
                }
                // pop
                if (spaceCount == 3)
                {
                }
                // push
                if (spaceCount == 4)
                {
                }
                // goto
                if (spaceCount == 5)
                {
                }
            }
        }
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
    /*cout << endl
         << "estado atual: " << currentState->getQ()
         << " - topo pilha: " << s.top() << " - palavra restante: " << remainingWord << endl
         << endl;*/
    if (isFinalStateAndEndOfWord(currentState->FinalState(), remainingWord))
    {
        return true;
    }

    vector<Transition *> t = currentState->getAllTrasitionsByChar(remainingWord[0]);
    // for (int i = 0; i < t.size(); i++)
    // {
    //     cout << "estado para ir: " << t[i]->getGo_to()->getQ() << ", input char das transicoes: " << t[i]->getInput_char() << ", pop: " << t[i]->getPop() << endl;
    // }
    for (int i = 0; i < t.size(); i++)
    {
        if (isStackTopAcceptedByTransition(t[i]->getPop(), s.top()))
        {
            if (isPopNotEmpty(t[i]->getPop()))
            {
                s.pop();
            }

            //  alterar quando for usar push grande
            if (isPushNotEmpty(t[i]->getPush()[0]))
            {
                s.push(t[i]->getPush()[0]);
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

int main()
{
    int Q, T;

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

    string word = "";
    while (word != "*")
    {
        stack<char> s;
        s.push('Z');
        cin >> word;
        if (testWord(states[0], word, s))
        {
            cout << word << ": sim" << endl;
        }
        else
            cout << word << ": nao" << endl;
    }

    // TODO:
    //      - ver porque a recursao nao volta no for pra continuar (era pq eu tava retornando o resultado sem dar chance de checar outras possibildades)
    //      - parsear o parse para funcionar com pushs de dois chars e
    //      - 100 estados, 100 trasicoes e 100 empilhamentos por transicao, palavras entre 1 e 100 chars

    return 0;
}