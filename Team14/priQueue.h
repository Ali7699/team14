
#pragma once
#include "priNode.h"


//This class impelements the priority queue as a sorted list (Linked List)
//Depending on Min or Max mode, The item with lowest or highest priority goes to the highest node
template <typename T>
class priQueue
{
protected:
    priNode<T>* head;
    bool minmax; //  0 is a Max que, 1 is a Min que, Min que places lowest priority first, max que places highest priority first. 
public:
    //default is MIN que
    priQueue(bool MINMAX=1) : head(nullptr),minmax(MINMAX) {} 

    ~priQueue() {
        T tmp;
        int p;
        while (dequeue(tmp, p));
    }

    //insert the new node in its correct position according to its priority
    void enqueue(const T& data, int priority) {
        priNode<T>* newNode = new priNode<T>(data, priority);

        if (head == nullptr || minmax&&priority < head->getPri()|| !minmax&&priority > head->getPri()) { // if minmax is false, we dont do statment. first statment checks if prio is smaller, MIN que. Second Statment checks if Prio is bigger, MAX que

            newNode->setNext(head);
            head = newNode;
            return;
        }

        priNode<T>* current = head;
        while (current->getNext() && priority <= current->getNext()->getPri()) {
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

        topEntry = head->getItem();
        pri = head->getPri();
        return true;
    }

    bool isEmpty() const {
        return head == nullptr;
    }
};
