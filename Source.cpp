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
	shared_ptr<Node<T>> left; // Pointer to next node
	shared_ptr<Node<T>> right;
	shared_ptr<Node<T>> parent;
public:

	Node() : letterCount(0), left(nullptr), right(nullptr), parent(nullptr)
	{
	} // end default constructor

	Node(const T& anItem) : item(anItem), letterCount(0), left(nullptr), right(nullptr), parent(nullptr)
	{
	} // end constructor

	void setLetter(const T& anItem)
	{
		item = anItem;
	} // end setItem

	void setLeft(shared_ptr<Node<T>> a)
	{
		left = a;
	}
	void setRight(shared_ptr<Node<T>> a)
	{
		right = a;
	}
	void setParent(shared_ptr<Node<T>> a)
	{
		parent = a;
	}

	T getLetter() const
	{
		return item;
	} // end getLetter

	shared_ptr<Node<T>> getRight() const
	{
		return right;
	} // end getNext
	shared_ptr<Node<T>> getLeft() const
	{
		return left;
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
Class Tree
**********************************************************************/

template <typename T>
class Tree : public BagInterface<T>
{
private:

	shared_ptr<Node<T>> root; // Pointer to first node
	int itemCount;

public:
	Tree() :root(nullptr), itemCount(0)
	{

	}  // end default constructor

	
	bool isEmpty() const
	{
		return itemCount == 0;
	}  // end isEmpty

	int getCurrentSize() const
	{
		return itemCount;
	}  // end getCurrentSize
	
	friend ostream& operator << (ostream& stream, Tree<T>& bag)
	{
		cout << "The bag contains " << bag.getCurrentSize() << " items:" << endl;
		shared_ptr<Node<T>> rover = bag.getHead();
		while (rover != nullptr)
		{
			cout << rover->getLetter();
			rover = rover->getNext();
		}  // end for
		cout << endl << endl;
		return stream;
	}  // end displayBag

	bool letterChecker(const T& letter);
	bool insert(const T& letter);
	bool treeDelete(const T& letter);
	void output()
	{
		InOrder(root);
		cout << itemCount++;
	}
	void InOrder(shared_ptr<Node<T>> root)
	{
		if (root != NULL)
		{

			InOrder(root->getLeft());
			cout << root->getLetter()  << endl;
			InOrder(root->getRight());
		}
	}

}; // end Tree


/*********************************************************************
Insert
**********************************************************************/

template<typename T>
bool Tree<T>::letterChecker(const T& letter)
{
	shared_ptr<Node<T>> temp = nullptr;
	temp = root;

	while (temp != nullptr)
	{
		
		if (temp->getLetter() == letter)
		{
			temp->addCount();
			
			return true;
		}
		
		else if (temp->getLetter() > letter)
		{
			temp = temp->getLeft();
		}
		else
		{
			temp = temp->getRight();
		}
	}
	
	insert(letter);
	itemCount++;
	return true;
}

template<typename T>
bool Tree<T>::insert(const T& letter)
{
	
	shared_ptr<Node<T>> temp = nullptr ;
	shared_ptr<Node<T>> traverseNode=root ;
	shared_ptr<Node<T>> nuNode(new Node<T>(letter));


	while ( traverseNode != nullptr)
	{
		temp = traverseNode;

		if (nuNode->getLetter() < traverseNode->getLetter())
		{
			traverseNode = traverseNode->getLeft();
		}
		else
		{
			traverseNode = traverseNode->getRight();
		}
	}
	
	nuNode->setParent(temp);

	if (temp == nullptr)
	{
		root = nuNode;
	}

	else if (nuNode->getLetter() < temp->getLetter())
	{
		temp->setLeft(nuNode);
	}

	else
	{
		temp->setRight(nuNode);
	}
	return true;

}

/*********************************************************************
Delete
**********************************************************************/

template<typename T>
bool Tree:: deleter()
{
	shared_ptr<Node<T>> temp;
	
	if root->getLeft() == nullptr;
	{
		translplant(root, root->getRight());
	}

	else if (root->right == nullptr)
	{
		transplant(root, root->getLeft());
	}
	else
	{
		temp = TreeMinmum(root->getRight());
		if (temp.parent != root)
		{
			transplant(temp, temp->getRight());
			temp->getRight() = temp->getRight;
			temp->getRight->getParent() = temp;
		}
	}
}

template<typename T>
{

}
//template<typename T>
//bool Tree<T>::delete(shared_ptr<Node<T>> deleteNode)
//{
//	if deletNode->getLeft == nullptr;
//	{
//		Transplant(deleteNode, deleteNode->getRight());
//	}
//
//	else if deleteNode->getRight == nullptr;
//	{
//		Transplant(deletNode, deleteNode->getl)
//	}
//}



void enableDebug(bool bvalue)
{
	if (!bvalue) return;

	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

	// Turn on leak-checking bit.
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;

	// Turn off CRT block checking bit.
	tmpFlag &= ~_CRTDBG_CHECK_CRT_DF;

	// Set flag to the new value.
	_CrtSetDbgFlag(tmpFlag);
}

/*********************************************************************
Main
**********************************************************************/

int main()
{
	enableDebug(true);
	char b;
	string a;
	ifstream infile;
	infile.open("Speech.txt");
	Tree<char> bag;

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			getline(infile, a);
			for (int i = 0; i < a.length(); i++)
			{
				b = tolower(a[i]);
				bag.letterChecker(b);
			}
		}
	}
	else

	{
		cout << "No file found\n";
	}

	infile.close();
	bag.output();

	
}  // end main
 

