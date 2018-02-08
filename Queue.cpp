#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <memory>
using namespace std;

template <typename T>
class Node{

private:

    T	speechLetter;
    shared_ptr<Node<T>> next;
    shared_ptr<Node<T>> prev;
    int letterCount;
    int sorted;

public:

    Node(){
        prev = nullptr;
        next = nullptr;
        letterCount = 0;
    }

    Node(const T & letters): speechLetter(letters), letterCount(0), next(nullptr), prev(nullptr){
    }

    void setNext(shared_ptr<Node<T>> nextPtr){
        next = nextPtr;
    }

    void setPrev(shared_ptr<Node<T>> prevPtr){
        prev = prevPtr;
    }

    shared_ptr<Node<T>> getPrev(){
        return prev;
    }

    shared_ptr<Node<T>> getNext(){
        return next;
    }

    void setLetter(T newData){
        speechLetter = newData;
    }

    void setletterCount(int count){
        letterCount = count;
    }

    T getLetter() const{
        return speechLetter;
    }

    int getletterCount(){
        return letterCount;
    }

    void incrementletterCount(){
        letterCount++;
    }

    void setSort(bool isSorted){
        sorted = isSorted;
    }

    bool isSorted(){
        return sorted;
    }

};
template <typename T>
class Queue{

private:

    shared_ptr<Node<T>> frontPtr;
    shared_ptr<Node<T>> backPtr;
    int itemCount;

public:

    Queue(): frontPtr(new Node<T>), backPtr(frontPtr), itemCount(0){
    }
    ~Queue(){
        shared_ptr<Node<T>> curentNodePtr(new Node<T>);
        curentNodePtr = frontPtr;
        backPtr = nullptr;
        while (frontPtr) {
            curentNodePtr = curentNodePtr->getNext();
            frontPtr->setNext(nullptr);
            frontPtr->setPrev(nullptr);
            frontPtr = curentNodePtr;
        }
        frontPtr = nullptr;
    }
    bool isEmpty() const ;
    bool pushCh(const T &);
    shared_ptr<Node<T>> popCh();
    T peekCharacter();
    bool queueinsertionSort(shared_ptr<Node<T>>, int);
    bool incrementCh(shared_ptr<Node<T>>, T);
    int getcurrentSize() const;
    void output();
    void callsortingFunction();

};

template <typename T>
bool Queue<T>::isEmpty() const{
    return itemCount == 0;
}

template <typename T>
bool Queue<T>::pushCh(const T & value){
    return incrementCh(frontPtr, value);
}

template <typename T>
shared_ptr<Node<T>> Queue<T>::popCh(){
    if (itemCount > 1){
        shared_ptr<Node<T>> popNode;
        popNode = frontPtr;
        frontPtr = frontPtr->getNext();
        frontPtr->setPrev(nullptr);
        popNode->setNext(nullptr);
        itemCount--;
        return popNode;
    }
    else if ((itemCount = 1)){
        itemCount--;
        return frontPtr;
    }
    else{
        return nullptr;
    }
}

template <typename T>
T Queue<T>::peekCharacter(){
    return frontPtr->getletterCount();
}

template <typename T>
bool Queue<T>::queueinsertionSort(shared_ptr<Node<T>> hdPtr, int sortCount){
    while (hdPtr->getNext()){
        int loopCount = sortCount;
        int tempLetterCount = 0;
        char tempLetterHolder = ' ';
        shared_ptr<Node<T>> chPtr = hdPtr->getNext();
        shared_ptr <Node<T>> positionPtr = hdPtr;
        for (int i = 0; i <= loopCount; i++){
            if (!hdPtr->getNext()){
                loopCount = -1;
            }
            else if (chPtr->getletterCount() > chPtr->getPrev()->getletterCount()){
                tempLetterHolder = chPtr->getLetter();
                tempLetterCount = chPtr->getletterCount();
                chPtr->setLetter(chPtr->getPrev()->getLetter());
                chPtr->setletterCount(chPtr->getPrev()->getletterCount());
                chPtr->getPrev()->setLetter(tempLetterHolder);
                chPtr->getPrev()->setletterCount(tempLetterCount);
                chPtr = chPtr->getPrev();
            }
            else{
                loopCount = -1;
            }
        }
        sortCount++;
        queueinsertionSort(positionPtr->getNext(), sortCount);
        return true;
    }
    return false;
}

template <typename T>
bool Queue<T>::incrementCh(shared_ptr<Node<T>> hdPtr, T character){
    if (hdPtr){
        if (character == hdPtr->getLetter()){
            hdPtr->incrementletterCount();
            return true;
        }
        else{
            return incrementCh(hdPtr->getNext(), character);
        }
    }
    else{
        shared_ptr<Node<T>> newchNode(new Node<T>(character));

        if (itemCount == 0){
            frontPtr = newchNode;
        }
        newchNode->setPrev(backPtr);
        backPtr->setNext(newchNode);
        backPtr = newchNode;
        newchNode->incrementletterCount();
        itemCount++;
        return true;
    }
}

template <typename T>
int Queue<T>::getcurrentSize() const{
    return itemCount;
}

template <typename T>
void Queue<T>::output(){
    shared_ptr<Node<T>> printNode(new Node<T>());
    printNode = frontPtr;
    for (int i = 0; i < itemCount; i++){
        cout << printNode->getLetter() << " : " << printNode->getletterCount() << endl;
        printNode = printNode->getNext();
    }
}

template <typename T>
void Queue<T>::callsortingFunction(){
    int sortCount = 0;
    queueinsertionSort(frontPtr, sortCount);
}

int main()
{

    Queue<char> queue;
    string line = "";
    ifstream inputFile;
    inputFile.open("Speech.txt");
    if (inputFile.is_open()){
        while (inputFile.peek() != EOF){
            getline(inputFile, line);
            for (unsigned int i = 0; i < line.length(); i++){
                unsigned char ch = tolower(line[i]);
                queue.pushCh(ch);
            }
        }
        inputFile.close();
        queue.callsortingFunction();
        queue.output();
    }
    else{
        cout << "The file could not be found." << endl;
    }
    return 0;
}
