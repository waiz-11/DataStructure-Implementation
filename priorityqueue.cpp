/* #pragma once */
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <ctime>
#include <cassert>
#include <cstdlib>

template <typename T, typename Comparator = std::less<T>>
class Priority_Queue {
    private:
        T *array;
        size_t capacity;
        size_t count;
        Comparator comp;

        void expand(size_t cap) {
            if (cap <= capacity) return;
            T *new_array = new T[cap];
            for (size_t i = 0; i < count; i++) {
                new_array[i] = std::move(array[i]);
            }
            
            delete[] array;
            array = new_array;
            capacity = cap;
        }

        void BubbleUp(size_t index) {
            while (index > 0) {
                size_t parent_index = (index - 1)/2;
                if (!comp(array[parent_index], array[index])) return;
                else {
                    std::swap(array[index], array[parent_index]);
                    index = parent_index;
                }
            }
        }

        void BubbleDown(size_t index) {
            while (2 * index + 1 < count) {
                size_t left_child_index = 2 * index + 1;
                size_t right_child_index = 2 * index + 2;

                size_t largest = index;
                if (left_child_index < count && comp(array[largest], array[left_child_index])) {
                    largest = left_child_index;
                }
                if (right_child_index < count && comp(array[largest], array[right_child_index])) {
                    largest = right_child_index;
                }

                if (largest == index) break;

                std::swap(array[index], array[largest]);
                index = largest;
            }
        }

    public:
        explicit Priority_Queue(size_t cap = 2, Comparator c = Comparator()) : count(0), capacity(cap), comp(c) {
            array = new T[cap];
        } //Default Explicit Constructor.

        Priority_Queue(const Priority_Queue &other) : count(other.count), capacity(other.capacity), comp(other.comp) {
            array = new T[capacity];
            for (size_t i = 0; i < count; i++) {
                array[i] = other.array[i];
            }
        } //Copy Constructor.

        Priority_Queue(Priority_Queue &&other) noexcept : count(other.count), capacity(other.capacity) {
            array = other.array;
            comp = std::move(other.comp);
            other.count = 0;
            other.capacity = 0;
            other.array = nullptr;
        } //Move Constructor.

        Priority_Queue &operator=(const Priority_Queue &other) {
            if (this == &other) {
                return *this;
            }
            count = other.count;
            capacity = other.capacity;
            comp = other.comp;

            delete[] array;
            array = new T[capacity];
            for (size_t i = 0; i < count; i++) {
                array[i] = other.array[i];
            }
            return *this;
        } //Copy Constructor.

        Priority_Queue& operator=(Priority_Queue &&other) {
            if (this == &other) {
                return *this;
            }

            delete[] array;
            count = other.count;
            capacity = other.capacity;
            array = other.array;
            comp = std::move(other.comp);

            other.array = nullptr;
            other.capacity = 0;
            other.count = 0;
            return *this;
        } //Move Assignment.

        ~Priority_Queue() {
            delete[] array;
        } //Destructor.

        bool operator==(const Priority_Queue &other) const {
            if (other.count != count) return false;
            for (size_t i = 0; i < count; i++) {
                if (other.array[i] != array[i]) {
                    return false;
                }
            }
            return true;
        } //Overloading '=='

        bool operator!=(const Priority_Queue &other) const {
            return !(other == *this);
        } //Overloading '!='

        void enqueue(T element) {
            if (capacity == count) {
                expand(capacity * 2);
            }
            array[count] = element;
            BubbleUp(count);
            count++;
        }

        template <typename ...Args>
        void emplace(Args&&... args) { //Args &&... args is an lvalue.
            if (count == capacity) {
                reserve(capacity * 2);
            }
            array[count] = T(std::forward<Args>(args)...);
            BubbleUp(count);
            count++;
        }

        void dequeue(void) {
            assert(!empty());
            array[0] = array[count - 1];
            count--;
            BubbleDown(0);
        }

        void reserve(size_t cap) {
            if (cap < capacity) return;
            expand(cap);
        }

        const T& top(void) const noexcept {
            return array[0];
        }

        void display(void) const noexcept {
            for (size_t i = 0; i < count; i++) {
                std::cout << array[i] << "  ";
            }
            std::cout << "\n";
        }

        size_t size(void) const noexcept {
            return count;
        }

        bool empty(void) const noexcept {
            return (count == 0);
        }
};

int main() {
    
}