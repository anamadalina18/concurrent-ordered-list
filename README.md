# Concurrent Ordered Linked List

A sorted, concurrently accessible singly linked list in C++17.

**Context.** This is the implementation part of *Project 0* from **Advanced Data Management
Systems** (TU Darmstadt, FG Data Management, 2026), an exercise on synchronization in index
structures. The course provided the `Node` class, the class declaration and the test suite. This
repository contains **only the files I implemented**, so it does not build standalone.

## What I implemented

**`OrderedLinkedList`** — keys are unique and inserted in ascending order, as the exercise specifies:

- `add(key, value)` — allocates a node and splices it in after the last node with a smaller key;
  handles the empty list and the "new head" case.
- `remove(key)` — locates the node, unlinks it by finding its predecessor (`findPrev`), frees it,
  and leaves the list untouched when the key is absent.
- `find(key)` — returns the node pointer or `nullptr`.
- Destructor walks the list and frees every node, so the tests run leak-free.

**Concurrency.** The list is guarded by a `std::shared_mutex`: readers (`find`) take the lock in
shared mode, mutations (`add`, `remove`) take it exclusively. This keeps concurrent lookups
parallel while serializing structural changes, without the ABA/reclamation problems of a lock-free
splice.

## Files

| Path | Contents |
|---|---|
| `src/LinkedList.cpp` | `add`, `remove`, `find`, predecessor search, destructor |
| `include/LinkedList.hpp` | Class layout, `head` pointer and the `shared_mutex` member |

## Notes

- The exercise is framed as an optimistic lock-coupling benchmark; my version is intentionally the
  reader-writer-lock variant — simpler to reason about, and it passes the concurrent tests while
  keeping lookup scalability.
- No memory leaks: the destructor deletes every node, which the basic tests check.
