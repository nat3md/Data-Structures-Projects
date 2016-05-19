#include <stdexcept>
#include <string>
#include <iostream>
#include <unordered_map>
#include <fstream>

using namespace std;

static const int DEFAULT_SIZE = 101;


template <typename Key, typename Value>
class Entry
{
private:
	Value item;
	Key searchKey;
public:
	Entry() { }
	Entry(Value newEntry, Key itemKey) :item(newEntry), searchKey(itemKey) { }
	Value getItem() const
	{
		return item;
	}
	Key getKey() const
	{
		return searchKey;
	}
	void setItem(const Value& newEntry)
	{
		item = newEntry;
	}
	void setKey(const Key& itemKey)
	{
		searchKey = itemKey;
	}
	bool operator==(const Entry<Key, Value>& rightHandItem) const
	{
		return (searchKey == rightHandItem.getKey());
	}  // end operator==
	bool operator>(const Entry<Key, Value>& rightHandItem) const
	{
		return (searchKey > rightHandItem.getKey());
	}

}; // end Entry

template<typename Key, typename Value>
class HashedEntry : public Entry<Key, Value>
{
private:
	HashedEntry<Key, Value>* nextPtr;
public:
	HashedEntry()
	{
		Entry<Key, Value>();
		nextPtr = nullptr;
	}  // end 
	HashedEntry(Value newEntry, Key itemKey)
	{
		Entry<Key, Value>::setItem(newEntry);
		Entry<Key, Value>::setKey(itemKey);
		nextPtr = nullptr;
	}  // end 
	HashedEntry(Value newEntry, Key itemKey, HashedEntry<Key, Value>* nextEntryPtr)
	{
		Entry<Key, Value>::setItem(newEntry);
		Entry<Key, Value>::setKey(itemKey);
		nextPtr = nextEntryPtr;
	}  // end 

	void setNext(HashedEntry<Key, Value>* nextEntryPtr)
	{
		nextPtr = nextEntryPtr;
	}  // end 
	HashedEntry<Key, Value>* HashedEntry<Key, Value>::getNext() const
	{
		return nextPtr;
	}  // end 
}; // end HashedEntry

template<typename Key, typename Value>
class DictionaryInterface
{
public:
	/** Sees whether this dictionary is empty.
	@return True if the dictionary is empty;
	otherwise returns false. */
	virtual bool isEmpty() const = 0;

	/** Gets the number of items in this dictionary.
	@return The number of items in the dictionary. */
	virtual int getNumberOfItems() const = 0;

	/** Inserts an item into this dictionary according to the item�s search key.
	@pre  The search key of the new item differs from all search
	keys presently in the dictionary.
	@post  If the insertion is successful, newItem is in its
	proper position within the dictionary.
	@param searchKey  The search key associated with the item to be inserted.
	@param newItem  The item to add to the dictionary.
	@return  True if item was successfully added, or false if not. */
	virtual bool add(const Key& searchKey, const Value& newItem) = 0;

	/** Removes an item with the given search key from this dictionary.
	@post  If the item whose search key equals searchKey existed in the dictionary,
	the item was removed.
	@param searchKey  The search key of the item to be removed.
	@return  True if the item was successfully removed, or false if not. */
	virtual bool remove(const Key& searchKey) = 0;

	/** Removes all entries from this dictionary. */
	virtual void clear() = 0;

	/** Retrieves an item with a given search key from a dictionary.
	@post  If the retrieval is successful, the item is returned.
	@param searchKey  The search key of the item to be retrieved.
	@return  The item associated with the search key.
	@throw  NotFoundException if the item does not exist. */
	virtual Value getItem(const Key& searchKey) ;

	/** Sees whether this dictionary contains an item with a given
	search key.
	@post  The dictionary is unchanged.
	@param searchKey  The search key of the item to be retrieved.
	@return  True if an item with the given search key exists in the dictionary. */
	virtual bool contains(const Key& searchKey) const = 0;

	/** Traverses this dictionary and calls a given client function once for each item.
	@post  The given function�s action occurs once for each item in the
	dictionary and possibly alters the item.
	@param visit A client function. */
	virtual void traverse(void visit(Value&)) const = 0;
}; // end DictionaryInterface

template <typename Key, typename Value>
class HashedDictionary : public DictionaryInterface<Key, Value>
{
private:
	HashedEntry<Key, Value>** hashTable;  // Array of pointers to entries
	int itemCount;                               // Count of dictionary entries
	int hashTableSize;                           // Table size must be prime
	void destroyDictionary()
	{
		for (int i = 0; i < hashTableSize; i++)
		{
			// If there are hashed entries at this location,
			// we need to delete them
			while (hashTable[i] != nullptr)
			{
				HashedEntry<Key, Value>* entryToRemovePtr = hashTable[i];
				hashTable[i] = hashTable[i]->getNext();
				delete entryToRemovePtr;
				entryToRemovePtr = nullptr;  // For safety
			}  // end while
		}  // end for

		itemCount = 0;
	}  // end destroyDictionary
	int getHashIndex(const Key& key) const
	{
		// We are creating a hash function type called hashFunction that hashes "Key."
		// First we create an an unordered_map object for our Key and Value
		unordered_map<Key, Value> mapper;

		// Then we call the hash_function method to return the hash function
		// for the Key and assign it to 'hashFunction'
		unordered_map<Key, Value>::hasher hashFunction = mapper.hash_function();

		// static_cast needed since hashFunction returns an unsigned long
		return static_cast<int>(hashFunction(key) % hashTableSize);

	} // end getHashIndex
	int getNextPrime(int number) const
	{
		bool foundPrime = false;
		if ((number % 2) == 0)
			number++;

		while (!foundPrime)
		{
			if ((number % 5 == 0) || !isPrime(number))
			{
				number = number + 2;
			}
			else
				foundPrime = true;
		}  // end

		return number;
	}  // end getNextPrime
	bool isPrime(int number) const
	{
		bool isPrimeNumber = true;                 // Assume prime, prove otherwise

		if ((number == 2) || (number == 3))        // Smallest primes
			isPrimeNumber = true;
		else
		{
			if ((number % 2 == 0) || (number < 2))  // Even number or ineligible
				isPrimeNumber = false;
			else
			{
				int root = sqrt(number);             // Limit of search
				int i = 3;
				do
				{
					if (number % i == 0)               // Not prime
						isPrimeNumber = false;
					else
						i = i + 2;

				} while ((isPrimeNumber == true) && (i <= root));
			}  // end if
		}  // endif

		return isPrimeNumber;
	}  // end isPrime
public:
	HashedDictionary() : itemCount(0), hashTableSize(DEFAULT_SIZE)
	{
		hashTable = new HashedEntry<Key, Value>*[DEFAULT_SIZE];
		for (int i = 0; i < DEFAULT_SIZE; i++)
			hashTable[i] = nullptr;
	}  // end 
	HashedDictionary(int maxNumberOfEntries) : itemCount(0)
	{
		hashTableSize = getNextPrime(maxNumberOfEntries);
		hashTable = new HashedEntry<Key, Value>*[hashTableSize];
		for (int i = 0; i < maxNumberOfEntries; i++)
			hashTable[i] = nullptr;
	}  // end 
	HashedDictionary(const HashedDictionary<Key, Value>& dict) : itemCount(dict.itemCount), hashTableSize(dict.hashTableSize)
	{
		hashTable = new HashedDictionary<Key, Value>*[dict.maxItems];
		for (int index = 0; index < dict.itemCount; index++)
		{
			hashTable[index] = dict.hashTable[index];
		}  // end for
	}  // end 
	~HashedDictionary()
	{
		destroyDictionary();
	} // end destructor
	bool isEmpty() const
	{
		return (itemCount == 0);
	} // end isEmpty
	int getNumberOfItems() const
	{
		return itemCount;
	} // end getNumberOfItems
	void clear()
	{
		destroyDictionary();
	} // end clear
	bool add(const Key& searchKey, const Value& newItem)
	{
		// Create entry to add to dictionary
		HashedEntry<Key, Value>* entryToAddPtr = new HashedEntry<Key, Value>(newItem, searchKey);

		// Compute the hashed index into the array
		int itemHashIndex = getHashIndex(searchKey);

		// Add the entry to the chain at itemHashIndex
		if (hashTable[itemHashIndex] == nullptr)
		{
			hashTable[itemHashIndex] = entryToAddPtr;
		}
		else
		{
			entryToAddPtr->setNext(hashTable[itemHashIndex]);
			hashTable[itemHashIndex] = entryToAddPtr;
		} // end if

		return true;
	} // end add
	bool remove(const Key& searchKey)
	{
		bool itemFound = false;

		// Compute the hashed index into the array
		int itemHashIndex = getHashIndex(searchKey);
		if (hashTable[itemHashIndex] != nullptr)
		{
			// Special case - first node has target
			if (searchKey == hashTable[itemHashIndex]->getKey())
			{
				HashedEntry<Key, Value>* entryToRemovePtr =
					hashTable[itemHashIndex];
				hashTable[itemHashIndex] = hashTable[itemHashIndex]->getNext();
				delete entryToRemovePtr;
				entryToRemovePtr = nullptr; // For safety
				itemFound = true;
			}
			else // Search the rest of the chain
			{
				HashedEntry<Key, Value>* prevPtr = hashTable[itemHashIndex];
				HashedEntry<Key, Value>* curPtr = prevPtr->getNext();
				while ((curPtr != nullptr) && !itemFound)
				{
					// Found item in chain so remove that node
					if (searchKey == curPtr->getKey())
					{
						prevPtr->setNext(curPtr->getNext());
						delete curPtr;
						curPtr = nullptr; // For safety
						itemFound = true;
					}
					else // Look at next entry in chain
					{
						prevPtr = curPtr;
						curPtr = curPtr->getNext();
					} // end if
				} // end while
			} // end if
		} // end if

		return itemFound;
	} // end remove
	Value getItem(const Key& searchKey)
	{
		// Compute the hashed index into the array
		int itemHashIndex = getHashIndex(searchKey);
		HashedEntry<Key, Value>* chainPtr = hashTable[itemHashIndex];

		// Short circuit evaluation is important here
		while ((chainPtr != nullptr) && (searchKey != chainPtr->getKey()))
		{
			chainPtr = chainPtr->getNext();
		} // end while 

		if (chainPtr == nullptr)
			throw NotFoundException("\n\nItem not found in Dictionary!\n\n");

		return chainPtr->getItem();
	} // end getItem
	bool contains(const Key& searchKey) const
	{
		// Compute the hashed index into the array
		int itemHashIndex = getHashIndex(searchKey);
		HashedEntry<Key, Value>* chainPtr = hashTable[itemHashIndex];

		// Short circuit evaluation is important here
		while ((chainPtr != nullptr) && (searchKey != chainPtr->getKey()))
		{
			chainPtr = chainPtr->getNext();
		} // end while 

		return (chainPtr != nullptr);
	} // end contains
	void traverse(void visit(Value&)) const
	{
		// Simple because the array is sorted.
		for (int index = 0; index<hashTableSize; index++)
		{
			// cout<<"\n Index: " << index << "  :"; // for testing
			HashedEntry<Key, Value>* chainPtr = hashTable[index];
			while (chainPtr != nullptr)
			{
				Value currentItem = chainPtr->getItem();
				visit(currentItem);
				chainPtr = chainPtr->getNext();
			} // end while 
		}  // end for 
	}  //end traverse
	Value operator[](Key key)
	{
		return getItem(key);
	}
}; // end HashedDictionary

void displayKey(string& anItem)
{
	cout << "Displaying item - " << anItem << endl;
}  // end display1

void displayValue(int& anItem)
{
	cout << "Displaying item - " << anItem << endl;
}  // end display2

void main()
{
	ifstream inFile;
	string product="";
	string price = "";
	HashedDictionary<string, string> bag;
	
	inFile.open("UProducts.csv");
	
	if (inFile.is_open())
	{
		while (!inFile.eof())
		{
		getline(inFile, product,',');
		getline(inFile, price);
		
		bag.add(product, price);
			
		}

	}
	else
	{
		cerr << "File Not found";
	}

	getchar();
}  // end main
