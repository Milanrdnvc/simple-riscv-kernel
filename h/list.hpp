#ifndef _list_hpp_
#define _list_hpp_

#include "../lib/hw.h"
#include "memoryAllocator.hpp"

// this implementation was taken from "OS1 Vezbe 7 zadatak 6 riscv context switch 1 synchronous"
template<typename T>
class List {
private:
    struct Elem {
        T *data;
        Elem *next;

        Elem(T *data, Elem *next) : data(data), next(next) {}

        void* operator new(size_t size) {
            return MemoryAllocator::memAlloc(size);
        }

        void* operator new[](size_t size) {
            return MemoryAllocator::memAlloc(size);
        }

        void operator delete(void *ptr) {
            MemoryAllocator::memFree(ptr);
        }

        void operator delete[](void *ptr) {
            MemoryAllocator::memFree(ptr);
        }
    };

    Elem *head, *tail, *curr, *prev;
public:
    List() : head(0), tail(0), curr(0), prev(0) {}

    List(const List<T> &) = delete;

    List<T> &operator=(const List<T> &) = delete;

    void setFirst() {
        curr = head;
        prev = 0;
    }

    T* getCurrent() {
        if (curr) return curr->data;
        else return 0;
    }

    T* removeCurrent() {
        if (!curr) return 0;

        if (curr == head) {
            head = head->next;
            if (!head) tail = 0;
            T* ret = curr->data;
            delete curr;
            return ret;
        }

        if (curr == tail) {
            if (prev) prev->next = 0;
            else head = 0;
            tail = prev;
            T* ret = curr->data;
            delete curr;
            return ret;
        }

        T* ret = curr->data;
        prev->next = curr->next;
        delete curr;
        return ret;
    }

    void next() {
        if (curr) {
            prev = curr;
            curr = curr->next;
        }
    }

    void addFirst(T *data) {
        Elem *elem = new Elem(data, head);
        head = elem;
        if (!tail) { tail = head; }
    }

    void addLast(T *data) {
        Elem *elem = new Elem(data, 0);
        if (tail) {
            tail->next = elem;
            tail = elem;
        } else {
            head = tail = elem;
        }
    }

    T *removeFirst() {
        if (!head) { return 0; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T *peekFirst() {
        if (!head) { return 0; }
        return head->data;
    }

    T *removeLast() {
        if (!head) { return 0; }

        Elem *prev = 0;
        for (Elem *curr = head; curr && curr != tail; curr = curr->next) {
            prev = curr;
        }

        Elem *elem = tail;
        if (prev) { prev->next = 0; }
        else { head = 0; }
        tail = prev;

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T *peekLast() {
        if (!tail) { return 0; }
        return tail->data;
    }
};


#endif
