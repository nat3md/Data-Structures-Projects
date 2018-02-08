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

    T	letter;
    shared_ptr<Node<T>> parent;
    shared_ptr<Node<T>> right;
    shared_ptr<Node<T>> left;
    int letterCount;
    int sorted;

public:

    Node(): parent(nullptr), right(nullptr), left(nullptr), letterCount(0){
    }

    Node(const T & letters) : letter(letters), letterCount(0), parent(nullptr), right(nullptr), left(nullptr){
    }

    void setRight(shared_ptr<Node<T>> rightPtr){
        right = rightPtr;
    }

    void setLeft(shared_ptr<Node<T>> leftPtr){
        left = leftPtr;
    }

    void setParent(shared_ptr<Node<T>> parentPtr){
        parent = parentPtr;
    }

    void setLetter(T newData){
        letter = newData;
    }

    void setletterCount(int count){
        letterCount = count;
    }

    shared_ptr<Node<T>> getRight(){
        return right;
    }

    shared_ptr<Node<T>> getLeft(){
        return left;
    }

    shared_ptr<Node<T>> getParent(){
        return parent;
    }

    T getLetter() const{
        return letter;
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
class Tree
{

private:

    shared_ptr<Node<T>> rootPtr;
    int heightCount;

public:

    Tree() : rootPtr(new Node<T>), heightCount(0){
    }

    ~Tree(){}

    bool isEmpty() const;
    bool insertLetters(T &);
    void deleteTree(shared_ptr<Node<T>>);
    void transplantTree(shared_ptr<Node<T>>, shared_ptr<Node<T>>);
    shared_ptr<Node<T>> minimumTree();
    void output(shared_ptr<Node<T>>);
    void calloutputFunction();

};

template <typename T>
bool Tree<T>::isEmpty() const{
    return heightCount == 0;
}

template <typename T>
bool Tree<T>::insertLetters(T & newCharacters){
    shared_ptr<Node<T>> yNode;
    shared_ptr<Node<T>> xNode = rootPtr;
    while (xNode != nullptr){
        yNode = xNode;
        if (newCharacters < xNode->getLetter()){
            xNode = xNode->getLeft();
        }
        else{
            xNode = xNode->getRight();
        }
    }
    if (yNode == nullptr){
        shared_ptr<Node<T>> newNode(new Node<T>(newCharacters));
        rootPtr = newNode;
        rootPtr->incrementletterCount();
    }
    else if (newCharacters < yNode->getLetter()){
        shared_ptr<Node<T>> newNode2(new Node<T>(newCharacters));
        yNode->setLeft(newNode2);
        newNode2->incrementletterCount();
        newNode2->setParent(yNode);
        heightCount++;
    }
    else if (newCharacters > yNode->getLetter()){
        shared_ptr<Node<T>> newNode3(new Node<T>(newCharacters));
        yNode->setRight(newNode3);
        newNode3->incrementletterCount();
        newNode3->setParent(yNode);
        heightCount++;
    }
    return true;

}

template <typename T>
void Tree<T>::deleteTree(shared_ptr<Node<T>> treePtr){
    shared_ptr<Node<T>> newTemp = nullptr;
    if (!treePtr->getLeft()){
        transplantTree(treePtr, treePtr->getRight());
    }
    else if (treePtr->getRight() == NULL){
        transplantTree(treePtr, treePtr->getLeft());
    }
    else{
        newTemp = minimumTree();
        if (newTemp->getParent() != treePtr)
        {
            transplantTree(newTemp, newTemp->getRight());
            newTemp->setRight(treePtr->getRight());
            newTemp->getRight()->setParent(newTemp);
        }
        transplantTree(treePtr, newTemp);
        newTemp->setLeft(treePtr->getLeft());
        newTemp->getLeft()->setParent(newTemp);
    }
}

template <typename T>
void Tree<T>::transplantTree(shared_ptr<Node<T>> treePtr, shared_ptr<Node<T>> transplantPtr){
    if (!treePtr->getParent() == NULL){
        rootPtr = transplantPtr;
    }
    else if (treePtr == treePtr->getParent()->getLeft()){
        treePtr->getParent()->setLeft(transplantPtr);
    }
    else{
        treePtr->getParent()->setRight(transplantPtr);
    }
    if (transplantPtr != NULL){
        transplantPtr->setParent() = treePtr->getParent();
    }
}

template <typename T>
shared_ptr<Node<T>> Tree<T>::minimumTree(){
    shared_ptr<Node<T>> newNode = rootPtr;
    while (newNode->getLeft() != NULL){
        newNode = newNode->getLeft();
    }
    return newNode;
}

template <typename T>
void Tree<T>::output(shared_ptr<Node<T>> outputPtr){
    if (outputPtr){
        output(outputPtr->getLeft());
        cout << outputPtr->getLetter() << ": " << outputPtr->getletterCount() << endl;
        output(outputPtr->getRight());
    }

}

template <typename T>
void Tree<T>::calloutputFunction(){
    output(rootPtr);
}

int main()
{
    Tree<char> Tree;
    string line = "";
    ifstream inputFile;
    inputFile.open("Speech.txt");
    if (inputFile.is_open()){
        while (inputFile.peek() != EOF)
        {
            getline(inputFile, line);

            for (unsigned int i = 0; i < line.length(); i++)
            {
                char letters = tolower(line[i]);
                Tree.insertLetters(letters);
            }

        }
        inputFile.close();
        Tree.calloutputFunction();
    }
    else{
        cout << "The file could not be found." << endl;
    }
    return 0;
}
