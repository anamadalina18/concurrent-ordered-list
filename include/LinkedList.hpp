#pragma once

#include <mutex>
#include <shared_mutex>
#include "Node.hpp"


// Concurrent Ordered Linked List
// Elements must be inserted in order ascending i.e. 1 , 2 , 3 .. N 
// Assume unique Keys, there will be no duplicates and you do not need to handle those
// To pass Advanced tests it must be concurrent
// NOTE: To pass even basic tests it is mandatory to not leak memory, so implement a destructor

class OrderedLinkedList
{

private:
    Node* head = nullptr;
    std::mutex listMutex; // might not be needed depending on your implementation
    // feel free to add variables
    std::shared_mutex listSharedMutex;

public:
    // -------------------------------------------------------------------------------------
    // To be implemented
    // -------------------------------------------------------------------------------------
    OrderedLinkedList();
    ~OrderedLinkedList();
    /**
     * Add a Node(@key,@value) in the linked list at the appropriate position
     */
    void add(int key, int value);
    /**
     * Remove the node with key @key if it is in the list
     */
    void remove(int key);
    /**
     * Return a ptr to the node if @key is in the list, else nullptr
     */
    Node* find(int key);

    // -------------------------------------------------------------------------------------
    // do not change
    // -------------------------------------------------------------------------------------
    Node* getHead() {
        return head;
    };
};

