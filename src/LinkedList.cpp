#include "LinkedList.hpp"

OrderedLinkedList::OrderedLinkedList() {
}
OrderedLinkedList::~OrderedLinkedList() {
    Node* head = getHead();
    while (head != nullptr) {
        Node* node = head;
        head = head->nextNode;

        delete(node);
    }
}

void OrderedLinkedList::add(int key, int value) {
    listSharedMutex.lock();

    Node* new_node = new Node(key, value);
    Node* current_node = getHead();

    if (current_node == nullptr) {
        head = new_node;
        listSharedMutex.unlock();
        return;
    }

    if (current_node->key > key) {
        new_node->nextNode = current_node;
        head = new_node;
        listSharedMutex.unlock();
        return;
    }

    while (current_node->nextNode != nullptr) {
        Node* prev = current_node;
        current_node = current_node->nextNode;

        if (prev->key < key && current_node->key > key) {
            new_node->nextNode = current_node;
            prev->nextNode = new_node;
            listSharedMutex.unlock();
            return;
        }
    }

    current_node->nextNode = new_node;
    listSharedMutex.unlock();
};

Node* findPrev(Node* head, int key) {
    if (head == nullptr || head->key == key) {
        return nullptr;
    }

    while (head->nextNode != nullptr) {
        Node* prev = head;
        head = head->nextNode;

        if (head->key == key) {
            return prev;
        }
    }

    return head;
}

void OrderedLinkedList::remove(int key) {
    if (getHead() == nullptr) {
        return;
    }

    Node* node = find(key);

    listSharedMutex.lock();
    if (node == nullptr) {
        listSharedMutex.unlock();
        return;
    }

    Node* prev = findPrev(getHead(), key);

    if (prev == nullptr) {
        head = node->nextNode;
    }
    else {
        prev->nextNode = node->nextNode;
    }

    delete(node);
    listSharedMutex.unlock();
};

Node* OrderedLinkedList::find(int key) {
    listSharedMutex.lock_shared();

    Node* current_node = getHead();

    while (current_node != nullptr) {
        if (current_node->key == key) {
            listSharedMutex.unlock_shared();
            return current_node;
        }

        current_node = current_node->nextNode;
    }

    listSharedMutex.unlock_shared();
    return nullptr;
};

