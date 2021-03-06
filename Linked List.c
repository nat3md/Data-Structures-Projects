#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>

#define R 6371
#define TO_RAD (3.1415926536 / 180)

using namespace std;

template<typename T>
class BagInterface
{

public:

    virtual int getCurrentSize() const = 0;

    virtual bool isEmpty() const = 0;

    virtual bool add(const T& newEntry) = 0;

    virtual bool remove(const T& anEntry) = 0;

    virtual void clear() = 0;

    virtual int getFrequencyOf(const T& anEntry) const = 0;

    virtual bool contains(const T& anEntry) const = 0;

};

template <typename T>
class Node{

private:

    T item;
    shared_ptr<Node<T>> next;
    shared_ptr<Node<T>> prev;

public:
    // Class Constructors
    Node() : next(nullptr), prev(nullptr){
    }

    Node(const T& anItem) : item(anItem), next(nullptr), prev(nullptr){
    }

    Node(const T& anItem, shared_ptr<Node<T>> nextNodePtr) : item(anItem), next(nextNodePtr){
    }
    // Class Methods
    void setItem(const T& anItem){
        item = anItem;
    }

    void setNext(shared_ptr<Node<T>> nextNodePtr){
        next = nextNodePtr;
    }

    void setPrev(shared_ptr<Node<T>> prevNodePtr) {
        prev = prevNodePtr;
    }

    T getItem() const{
        return item;
    }

    shared_ptr<Node<T>> getNext() const{
        return next;
    }

    shared_ptr<Node<T>> getPrev() const{
        return prev;
    }

};

template <typename T>
class LinkedBag : public BagInterface<T>{

private:

    shared_ptr<Node<T>> headPtr;
    shared_ptr<Node<T>> tailPtr;
    int itemCount;

public:

    LinkedBag() : headPtr(new Node<T>()), tailPtr(headPtr), itemCount(0){
    }

    LinkedBag(const LinkedBag<T>& aBag){
        itemCount = aBag.itemCount;
        shared_ptr<T[]> origChainPtr = aBag.headPtr;
        if (origChainPtr == nullptr)
            headPtr = nullptr;
        else {
            headPtr = new shared_ptr<T[]>();
            headPtr->setItem(origChainPtr->getItem());
            shared_ptr<T[]> newChainPtr = headPtr;
            origChainPtr = origChainPtr->getNext();
            while (origChainPtr != nullptr){
                T nextItem = origChainPtr->getItem(); // Get next item from original chain
                shared_ptr<T[]> newNodePtr = new shared_ptr<T[]>(nextItem); // Create a new node containing the next item
                newChainPtr->setNext(newNodePtr); // Link new node to end of new chain
                newChainPtr = newChainPtr->getNext(); // Advance pointer to new last node
                origChainPtr = origChainPtr->getNext(); // Advance original-chain pointer
            }
            newChainPtr->setNext(nullptr);              // Flag end of chain
        }
    }

    ~LinkedBag(){
        shared_ptr<Node<T>> traverse(new Node<T>);
        traverse = headPtr;
        tailPtr = nullptr;
        while (headPtr){
            traverse = traverse->getNext();
            headPtr->setNext(nullptr);
            headPtr->setPrev(nullptr);
            headPtr = traverse;
        }
    }

    bool isEmpty() const{
        return itemCount == 0;
    }

    int getCurrentSize() const{
        return itemCount;
    }

    bool add(const T& newEntry){
        shared_ptr<Node<T>> newNode(new Node<T>(newEntry));
        if (itemCount == 0){
            headPtr->setNext(newNode);
            newNode->setPrev(shared_ptr<Node<T>>(headPtr));
            tailPtr = newNode;
        }
        else{
            headPtr->getNext()->setPrev(newNode);
            newNode->setNext(shared_ptr<Node<T>>(headPtr->getNext()));
            newNode->setPrev(headPtr);
            headPtr->setNext(newNode);
        }
        ++itemCount;
        return true;
    }

    T findObjectBackwards(T findObject1, bool& found){
        shared_ptr<Node<T>> currentPtr1 = tailPtr;
        T obj1 = findObject1;
        while (currentPtr1 != NULL && !found)
        {
            if ( currentPtr1->getItem() == findObject1 )
            {
                found = true;
                obj1 = currentPtr1->getItem();
            }
            currentPtr1 = currentPtr1->getPrev();
        }
        return obj1;
    }

    T findObjectForwards(T findObject2, bool& found){
        shared_ptr<Node<T>> currentPtr2 = headPtr->getNext();
        T obj2 = findObject2;
        while (currentPtr2 != NULL && !found)
        {
            if ( currentPtr2->getItem() == findObject2 )
            {
                found = true;
                obj2 = currentPtr2->getItem();
            }
            currentPtr2 = currentPtr2->getNext();
        }
        return obj2;
    }

    bool remove(const T& anEntry){
       shared_ptr<Node<T>> entryNodePtr = getPointerTo(anEntry);
       bool canRemoveItem = !isEmpty() && (entryNodePtr != nullptr);
       if (canRemoveItem){
           entryNodePtr->setItem(headPtr->getItem());
           shared_ptr<T[]> nodeToDeletePtr = headPtr;
           headPtr = headPtr->getNext();
           nodeToDeletePtr->setNext(nullptr);
           delete nodeToDeletePtr;
           nodeToDeletePtr = nullptr;
           itemCount--;
       }
        return true;
    }

    void clear(){
        shared_ptr<Node<T>> currPtr = headPtr;
        shared_ptr<Node<T>> temp;
        while (currPtr != nullptr){
            temp = currPtr->getNext();
            currPtr.reset();
            currPtr = temp;
        }
        itemCount = 0;
    }

    int getFrequencyOf(const T& anEntry) const{
        int frequency = 0;
        int counter = 0;
        shared_ptr<T[]> curPtr = headPtr;
        while ((curPtr != nullptr) && (counter < itemCount)){
           if (anEntry == curPtr->getItem()){
               frequency++;
           }
           counter++;
           curPtr = curPtr->getNext();
        }
        return frequency;
    }

    bool contains(const T& anEntry) const{
        return (getPointerTo(anEntry) != nullptr);
    }

    shared_ptr<Node<T>> getPointerTo(const T& anEntry){
       bool found = false;
       shared_ptr<T[]> curPtr = headPtr;
       while (!found && (curPtr != nullptr){
           if (anEntry == curPtr->getItem()){
               found = true;
           }
           else{
               curPtr = curPtr->getNext();
           }
        return headPtr;
        }
    }



    shared_ptr<Node<T>> getHead(){
        return headPtr;
    }

    friend ostream& operator << (ostream& stream, LinkedBag<T>& bag){
        cout << "The bag contains " << bag.getCurrentSize() << " items:" << endl;
        shared_ptr<T[]> rover = bag.getHead();
        while (rover != nullptr){
            cout << rover->getItem();
            rover = rover->getNext();
        }
        cout << endl << endl;
        return stream;
    }

};

class Airports{

private:

    string airportName;
    string airportLatitude;
    string airportLongitude;
    string cityName;
    string stateName;

public:

    Airports(){
        airportName = "";
        airportLatitude = "";
        airportLongitude = "";
        cityName = "";
        stateName = "";
    }

    Airports(string an, string al, string along, string cn, string sn){
        airportName = an;
        airportLatitude = al;
        airportLongitude = along;
        cityName = cn;
        stateName = sn;
    }

    void setName(string name){
        airportName = name;
    }

    double getLatitude(){
        return stod(airportLatitude);
    }

    double getLongitude() {
        return stod(airportLongitude);
    }

    bool operator == (const Airports &an) const{
        return (an.airportName == airportName);
    }

    friend ostream & operator << (ostream & out, const Airports & obj){
        out << obj.airportName << " " << obj.airportLatitude << " " << obj.airportLongitude << " ";
        out << obj.cityName << " " << obj.stateName << endl;
        return out;
    }

};


void enableDebug(bool bvalue){
   if (!bvalue) return;
   int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
   tmpFlag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit.
   tmpFlag &= ~_CRTDBG_CHECK_CRT_DF; // Turn off CRT block checking bit.
   _CrtSetDbgFlag(tmpFlag); // Set flag to the new value.
}

void f(const Airports& item){
    cout << item << endl;
}

bool airportsEqual(const Airports& obj1, const Airports& obj2){
    return (obj1 == obj2);
}

double dist(double th1, double ph1, double th2, double ph2){
    double dx, dy, dz;
    ph1 -= ph2;
    ph1 *= TO_RAD, th1 *= TO_RAD, th2 *= TO_RAD;
    dz = sin(th1) - sin(th2);
    dx = cos(ph1) * cos(th1) - cos(th2);
    dy = sin(ph1) * cos(th1);
    return asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * R;
}

int main(){
    enableDebug(true);
    LinkedBag<Airports> bag;
    ifstream inputFile;
    inputFile.open("Airports.csv");
    if (inputFile.is_open()){
        while (inputFile.peek() != EOF){
            string airName;
            string airLat;
            string airLong;
            string cityN;
            string stateN;
            getline(inputFile, airName, ',');
            getline(inputFile, airLat, ',');
            getline(inputFile, airLong, ',');
            getline(inputFile, cityN, ',');
            getline(inputFile, stateN, '\n');
            Airports air(airName, airLat, airLong, cityN, stateN);
            bag.add(air);
        }
        inputFile.close();
        Airports airport1;
        string airportName1, airportName2;
        cout << "Please enter your first airport name. Example: [ABE]\n";
        cin >> airportName1;
        airport1.setName(airportName1);
        bool found = false;
        airport1 = bag.findObjectForwards(airport1, found);
        Airports airport2;
        cout << "Please enter your second airport name. Example: [ZZV]\n";
        cin >> airportName2;
        airport2.setName(airportName2);
        found = false;
        airport2 = bag.findObjectForwards(airport2, found);
        cout << "Distance = " << dist(airport1.getLatitude(), airport1.getLongitude(), airport2.getLatitude(), airport2.getLongitude()) << endl;
    }
    else{
        cout << "Sorry, file not found." << endl;
    }
    return 0;
}
