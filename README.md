# Concurrent Ordered Linked List

A sorted, concurrently accessible singly linked list in C++17.

**Context.** This is the implementation part of *Project 0* from **Advanced Data Management
Systems** (TU Darmstadt, FG Data Management, 2026), an exercise on synchronization in index
structures. The course provided the `Node` class, the class declaration and the test suite. This
repository contains **only the files I implemented**, so it does not build standalone.

## Architecture

The list keeps a single `head` pointer and one `std::shared_mutex` for the whole structure:

- `find(key)` takes the mutex in **shared** mode, so any number of lookups run in parallel;
- `add` and `remove` take it in **exclusive** mode, so structural changes are serialized.

`add` walks from the head while `key` is greater, then splices a freshly allocated node in front of
the first larger key — the empty-list and new-head cases are handled separately. `remove` locates the
node through `findPrev`, unlinks it, and frees it; a missing key is a no-op. The destructor walks the
list and deletes every node, so the structure is leak-free at the end of a test.

The exercise is framed as an optimistic lock-coupling benchmark; this is deliberately the simpler
reader-writer variant. The trade-off is explicit: one lock instead of per-node latches, which is easy
to reason about and correct for the tests, but it does not scale write throughput.

## Limitations

- **One lock for the whole list.** Every mutation serializes on the same mutex and readers block
  writers, so the structure has no scaling beyond the read path — the opposite of the per-node lock
  coupling the exercise compares against.
- **`find` returns a raw pointer after unlocking.** The node can be freed by a concurrent `remove`
  once the shared lock is released, so callers must not dereference it outside a critical section.
  A safe version needs hazard pointers, epochs, or a reference-counted node.
- **O(n) operations.** Insertion and removal traverse from the head under the exclusive lock, so
  cost grows linearly with the list length.
- **Unique keys only.** Duplicates are excluded by the exercise contract; the code does not detect
  them.
- **No reclamation scheme.** There is no deferred free, so the design cannot be extended to
  lock-free traversal without adding one.

## Performance

Measured locally on an **Intel Core i5-12450H (8 cores / 12 threads), 16 GB RAM, Windows 11**,
`g++ 16.2 -O3`, single run, synthetic data. Indicative numbers, not a controlled suite.

Setup: 10,000 random keys, 200,000 lookups.

| Workload | Result |
|---|---|
| `add` (random order) | **43.8 k ops/s** |
| `find`, 1 thread | **21.7 k ops/s** |
| `find`, 8 threads | **179.4 k ops/s** (≈ 8.3× scaling) |
| `remove` | **23.6 k ops/s** |

Reads scale almost linearly because the shared lock allows concurrent traversal; writes are bound by
the exclusive lock and the O(n) walk, which is the cost this design accepts.

## Files

| Path | Contents |
|---|---|
| `src/LinkedList.cpp` | `add`, `remove`, `find`, predecessor search, destructor |
| `include/LinkedList.hpp` | Class layout, `head` pointer and the `shared_mutex` member |

## Notes

- No memory leaks: the destructor deletes every node, which the basic tests check.
