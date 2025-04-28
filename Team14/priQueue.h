
#pragma once
#include "priNode.h"


//This class impelements the priority queue as a sorted list (Linked List)
//This is a MIn pri que, lowest priority first
template <typename T>
class priQueue
{
protected:
    priNode<T>* head;
public:
    //default is MIN que
    priQueue() : head(nullptr) {} 

    ~priQueue() {
        T tmp;
        int p;
        while (dequeue(tmp, p));
    }

    // Insert the new node in its correct position (sorted by priority in ascending order) 
    void enqueue(const T& data, int priority) {
        priNode<T>* newNode = new priNode<T>(data, priority);

        if (head == nullptr || priority < head->getPri()) {
            newNode->setNext(head);
            head = newNode;
            return;
        }

        
        priNode<T>* current = head;
        while (current->getNext() && current->getNext()->getPri() <= priority) {
            current = current->getNext();
        }

        
        newNode->setNext(current->getNext());
        current->setNext(newNode);
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
        pri = head->getPri();
        return true;
    }

    bool isEmpty() const {
        return head == nullptr;
    }
    //returns item count
    int count() const
    {
        int counter = 0;
        priNode<T>* current = head;

        while (current != nullptr)
        {
            counter++;
            current = current->getNext();
        }

        return counter;
    }
};
