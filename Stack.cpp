#include <vector>
#include <iostream>
#include <string>
#include <cctype>
#include <cstring>
#include <cstdlib>
using namespace std;

template <typename T>
class Node{

private:

    T data;
    Node<T> * next;

public:

    Node(){
        next = nullptr;
    }

    T getData() const{
        return data;
    }

    Node<T> * getNext(){
        return next;
    }

    void setNext(Node<T> * nextPtr){
        next = nextPtr;
    }

    void setData(T newData){
        data = newData;
    }

};

template <typename T>
class Stack
{

private:

    Node<T> * topPtr;
    int itemCount;

public:

    Stack(){
        topPtr = nullptr;
        itemCount = 0;
    }

    ~Stack(){
        Node<T> *curentNodePtr;
        Node<T> *nextNode;
        curentNodePtr = topPtr;
        while (curentNodePtr){
            nextNode = curentNodePtr->getNext();
            delete curentNodePtr;
            curentNodePtr = nextNode;
        }
    }

    bool isEmpty(){
        return itemCount == 0;
    };

    bool pushData(T){
        Node<T> * newnodePtr;
        newnodePtr = new Node<T>;
        newnodePtr->setNext(topPtr);
        newnodePtr->setData(value);
        topPtr = newnodePtr;
        itemCount++;
        return true
    };

    T popData(){
        Node<T> *tempPtr;
        char value;
        if(itemCount == 0){
            return T();
        }
        else{
            value = topPtr->getData();
            tempPtr = topPtr;
            topPtr = topPtr->getNext();
            delete tempPtr;
            itemCount--;
            return value;
        }
    }

    T getData(){
        return topPtr->getData();
    }

    string prefixtoPostFix(string expression){
        Stack<char> stack;
        string pFix = "";
        for (int i = 0; i < expression.length(); i++){
            switch (expression[i]){
            case '+':
            case '-':
            case '*':
            case '/':{
                while (!stack.isEmpty() && stack.peepData() != '(' && precidence(expression[i]) <= precidence(stack.peepData()))
                {
                    pFix += stack.peepData();
                    stack.popData();
                }
                stack.pushData(expression[i]);
            }
            break;
            case '(':
                stack.pushData(expression[i]);
                break;
            case ')':{
                while (stack.peepData() != '('){
                    pFix += stack.peepData();
                    stack.popData();

                }
                stack.popData();
            }
            break;
            default:
                pFix += expression[i];
                break;
            }
        }
        while (!stack.isEmpty()){
            pFix += stack.peepData();
            stack.popData();
        }
        return pFix;
    }
};

int precidence(char p){
    switch(p){
        case '+':
            return 1;
            break;
        case '-':
            return 1;
            break;
        case '*':
            return 2;
            break;
        case '/':
            return 2;
            break;
        case '^':
            return 3;
            break;
        default:
            return 0;
    }
}

char isOperator(char op){
    if (op == '*' || op == '/' || op == '+' || op == '-' || op == '(' || op == ')'){
        return op;
    }
    else{
        return 'o';
    }
}

int main(){
    Stack<string> bag;
    string preFix, postFix;
    cout << "Please enter in the expression." << endl;
    cin >> preFix;
    postFix = bag.prefixtoPostFix(preFix);
    cout << postFix << endl;
    return 0;
}
