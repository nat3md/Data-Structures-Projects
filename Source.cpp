#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <cstdlib>

using namespace std;


template<typename T>
class BagInterface
{
public:

	virtual int getCurrentSize() const = 0;

	virtual bool isEmpty() const = 0;

	virtual void clear() = 0;

}; // end BagInterface


/*********************************************************************
Class Node
**********************************************************************/


template <typename T>
class Node
{

private:
	int letterCount;
	T	item; // A data item
	Node<T> * next; // Pointer to next node
	Node<T> * prev;

public:

	Node() : letterCount(0), next(nullptr), prev(nullptr)
	{
	} // end default constructor

	Node(const T& anItem) : item(anItem), letterCount(0), next(nullptr), prev(nullptr)
	{
	} // end constructor

	void setLetter(const T& anItem)
	{
		item = anItem;
	} // end setItem

	void setNext(Node<T> * a)
	{
		next = a;
	}
	void setPrev(Node<T> * a)
	{
		prev = a;
	}

	T getLetter() const
	{
		return item;
	} // end getItem

	Node<T>* getNext() const
	{
		return next;
	} // end getNext
	Node<T> * getPrev() const
	{
		return prev;
	}
	int getLetterCount()
	{
		return letterCount;
	}
	void setLetterCount(int lCount)
	{
		letterCount = lCount;
	}
	void addCount()
	{
		letterCount++;
	}
}; // end Node



/*********************************************************************
Class LinkedBag
**********************************************************************/

template <typename T>
class LinkedBag : public BagInterface<T>
{
private:

	Node<T>* headPtr; // Pointer to first node
	Node<T>* rearPtr;
	int itemCount;

public:
	LinkedBag() : headPtr(nullptr), itemCount(0), rearPtr(nullptr)
	{

	}  // end default constructor


	~LinkedBag()
	{
		clear();
	}  // end destructor

	bool isEmpty() const
	{
		return itemCount == 0;
	}  // end isEmpty

	int getCurrentSize() const
	{
		return itemCount;
	}  // end getCurrentSize

	void clear()
	{
		Node<T>* nodeToDeletePtr = headPtr;
		while (headPtr != nullptr)
		{
			headPtr = headPtr->getNext();

			// Return node to the system
			nodeToDeletePtr->setNext(nullptr);
			delete nodeToDeletePtr;

			nodeToDeletePtr = headPtr;
		}  // end while

		itemCount = 0;
	}  // end clear

	friend ostream& operator << (ostream& stream, LinkedBag<T>& bag)
	{
		cout << "The bag contains " << bag.getCurrentSize() << " items:" << endl;
		Node<T>* rover = bag.getHead();
		while (rover != nullptr)
		{
			cout << rover->getItem();
			rover = rover->getNext();
		}  // end for
		cout << endl << endl;
		return stream;
	}  // end displayBag

	bool enqeue(const T& text);
	bool adder(Node<T>* ptr, T ch);

	void sort()
	{
		cout << "Sort Function." << endl;
		int sortCount = 0;
		cout << "Begin insertionSort " << endl;
		insertionSortQueue(headPtr, sortCount);
		cout << "End insertionSort" << endl;
		cout << "End Sort Function." << endl;
	}
	bool insertionSortQueue(Node<T> * ptr, int sortCount)
	{

		while (ptr->getNext())
		{

			int loopCount = sortCount;
			int tempLetterCount = 0;
			char tempLetterHolder = ' ';
			Node<T> * tempPtr = ptr->getNext();
			Node<T> * positionPtr = ptr;

			for (int i = 0; i <= loopCount; i++)
			{
				if (!ptr->getNext())
				{
					loopCount = -1;
				}
				else if (tempPtr->getLetterCount() > tempPtr->getPrev()->getLetterCount())
				{
					tempLetterHolder = tempPtr->getLetter();
					tempLetterCount = tempPtr->getLetterCount();

					tempPtr->setLetter(tempPtr->getPrev()->getLetter());
					tempPtr->setLetterCount(tempPtr->getPrev()->getLetterCount());

					tempPtr->getPrev()->setLetter(tempLetterHolder);
					tempPtr->getPrev()->setLetterCount(tempLetterCount);

					tempPtr = tempPtr->getPrev();
				}
				else
					loopCount = -1;
			}
			sortCount++;
			insertionSortQueue(positionPtr->getNext(), sortCount);

			return true;
		}
		return false;


	}
	void output()
	{
		Node<T> * traverseNodePtr = headPtr;
		while (traverseNodePtr != nullptr)
		{
			cout << traverseNodePtr->getLetter() << " : " << traverseNodePtr->getLetterCount() << endl;
			traverseNodePtr = traverseNodePtr->getNext();
		}
	}

}; // end LinkedBag



/*********************************************************************
Enqueue
**********************************************************************/

template<typename T>
bool LinkedBag<T>::enqeue(const T& letter)
{

	return adder(headPtr, letter);
}




/*********************************************************************
Adder
**********************************************************************/
template<typename T>
bool LinkedBag<T>::adder(Node<T>* ptr, T ch)
{
	if (ptr)
	{
		if (ch == ptr->getLetter())
		{
			ptr->addCount();
			return true;
		}
		else
		{
			return adder(ptr->getNext(), ch);
		}
	}

	else
	{

		Node<T> * nuNode = new Node<T>(ch);

		if (itemCount == 0)
		{
			headPtr = nuNode;
			rearPtr = nuNode;
		}

		else
		{
			nuNode->setPrev(rearPtr);
			rearPtr->setNext(nuNode);
			rearPtr = nuNode;
			nuNode->addCount();
		}
		itemCount++;

		return true;
	}
}




/*********************************************************************
Main
**********************************************************************/

int main()
{
	char b;
	string a;
	ifstream infile;
	infile.open("Speech.txt");
	LinkedBag<char> bag;

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			getline(infile, a);
			for (int i = 0; i < a.length(); i++)
			{
				b = tolower(a[i]);
				bag.enqeue(b);
			}
		}
	}
	else

	{
		cout << "No file found\n";
	}

	bag.output();
	bag.sort();
	bag.output();
}  // end main

//MAKE SURE TO REMEBER TO PUT BACK THE DEBUG FUNCTION 