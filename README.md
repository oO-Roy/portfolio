# C / C++ Systems Programming Portfolio

Projects I wrote during the **Infinity Labs R&D** Software Development Program (2024–2025): an intensive program in C, C++ and Linux systems programming.

Each module has its own public header, implementation and unit test, and is built with **Make** under strict flags (`-ansi -pedantic-errors -Wall -Wextra`). I debugged them with **GDB** on Linux.

## Featured projects

| Project | Language | Highlights |
|---|---|---|
| [**Watchdog**](projects/watchdog) | C | A process-pair watchdog that detects when the client program has crashed and restarts it. Built on `fork`/`execv`, POSIX signals (`SIGUSR1`/`SIGUSR2`), named and unnamed semaphores, and a dedicated monitoring thread. |
| [**Thread Pool**](projects/thread_pool) | C++11/20 | A priority-based thread pool on top of a thread-safe waitable priority queue. Supports futures, resizing at runtime, and pause/resume/stop. Uses RAII, `std::condition_variable` and `std::atomic`. |
| [**TCP/UDP Server**](system_programming/src/server.c) | C | A single-threaded server that handles TCP, UDP and stdin together through `select()`, with matching TCP and UDP clients. |
| [**ext2 File-System Parser**](system_programming/src/file_system_parsing.c) | C | Reads an ext2 RAM-disk image directly, walking the superblock, group descriptors and inodes to locate files and print their contents. |
| [**Memory Allocators**](system_programming/include) | C | A fixed-size block allocator (FSA) and a variable-size allocator (VSA) that manage a pre-allocated memory pool, with alignment handled. |
| [**Expression Calculator**](projects/calculator) | C | Parses and evaluates arithmetic expressions with a finite-state machine and two stacks, and reports syntax and math errors. |

## Linux systems programming — [`system_programming/`](system_programming)
- **Networking:** TCP and UDP client/server libraries, plus UDP broadcast.
- **IPC:** ping-pong between processes over pipes, named pipes, shared memory, message queues and semaphores ([`others/ipc_ping_pong`](system_programming/others/ipc_ping_pong)).
- **Concurrency:** producer-consumer variants (mutex, semaphore, condition variable), multithreaded counting sort and lexicographic sort, and a test that creates 100k threads.
- **Utilities:** a logger and a simple shell (`fork`/`exec`/`wait`).

## Data structures — [`ds/`](ds) (ANSI C)
Singly and doubly linked lists, sorted list, stack, queue, vector, **circular buffer**, **bit array**, BST, **AVL tree**, **hash table**, heap, priority queue (list-based and heap-based), **task scheduler**, UID generator, and a **DHCP IP allocator** built on a binary trie.

## Algorithms — [`algorithm/`](algorithm)
Sorting (comparison and non-comparison), searching, recursion, a **finite-state machine**, and Knight's Tour (backtracking with Warnsdorff's heuristic).

## C++ — [`cpp/`](cpp)
A reference-counted string with copy-on-write (proxy class), a polymorphic shapes hierarchy, and **cpp2c**: hand-translating C++ classes, inheritance and virtual dispatch (vtables) into plain C.

## Build & run
```bash
cd ds            # or algorithm / system_programming / projects/<name>
make             # builds the modules (data structures as shared libraries) and their unit tests
```
Then run the generated `run_<module>` test executable.

---
**Roy Steiner** · Embedded Software Engineer · C · C++ · Linux · STM32
