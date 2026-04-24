#pragma once
#include "priNode.h"
#include <iostream>


//This class impelements the priority queue as a sorted list (Linked List)
//The item with highest priority is at the front of the queue
template <typename T>
class priQueue
{
protected:  //in case you need to inherit for the project
    priNode<T>* head;
public:
    priQueue() : head(nullptr) {}

    ~priQueue() {
        T tmp;
        int p;
        while (dequeue(tmp,p));
    }

    //insert the new node in its correct position according to its priority
    void enqueue(const T& data, int priority) {
        priNode<T>* newNode = new priNode<T>(data, priority);

        if (head == nullptr || priority > head->getPri()) {
            
            newNode->setNext(head);
            head = newNode;
            return;
        }
       
        priNode<T>* current = head;        
        while (current->getNext() && priority <= current->getNext()->getPri()) {
            current = current->getNext();
        }
        newNode->setNext( current->getNext());
        current->setNext( newNode);        
    }

    bool dequeue(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        priNode<T>* temp = head;
        head = head->getNext();
        delete temp;
        return true;
    }

    bool peek(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        return true;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

	//print the queue

    void printQueue() const {
		// check if the queue is empty before printing
        if (isEmpty()) {
            cout << "Priority queue is empty" << endl;
            return;
        }

        priNode<T>* current = head;
        while (current) {
			int pri; //created a variable to store the priority, getItem requires a parameter to store in it
			T item = current->getItem(pri); // get the item and its priority
            cout<<item <<" "; //cout << "Data: " << current->getItem(pri) << ", Priority:" << pri<<  endl;
            current = current->getNext();
        }
        cout << endl;
	}
	// get count of items in the queue
    int getCount() const {
        int count = 0;
        priNode<T>* current = head;
        while (current) {
            count++;
            current = current->getNext();
        }
        return count;
	}
};
