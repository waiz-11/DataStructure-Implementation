#include <iostream>
#include <queue>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <type_traits>

template <typename Key, typename Priority, typename Comparator = std::less<Priority>>
class Indexed_Priority_Queue {
    static_assert(std::is_integral<Key>::value, "Key must be integral");
    
    private:
        Key *heap; //stores indices;
        Priority *priority; //stores actual priority;
        int *pos; //look-up for the indices;
        
        size_t count;
        size_t capacity;
        Comparator comp;

        void expand(size_t cap) {
            if (cap <= capacity) return;

            size_t oldCapacity = capacity;

            Key *newheap = new Key[cap];
            Priority *newpriority = new Priority[cap];
            int *newpos = new int[cap];

            // copy heap
            for (size_t i = 0; i < count; i++)
                newheap[i] = heap[i];

            // copy priority + pos
            for (size_t i = 0; i < oldCapacity; i++) {
                newpriority[i] = priority[i];
                newpos[i] = pos[i];
            }

            // initialize NEW pos slots
            for (size_t i = oldCapacity; i < cap; i++)
                newpos[i] = -1;

            delete[] heap;
            delete[] priority;
            delete[] pos;

            heap = newheap;
            priority = newpriority;
            pos = newpos;

            capacity = cap;
        }

        void swap(size_t i, size_t j) {
            std::swap(heap[i], heap[j]);
            pos[heap[i]] = static_cast<int>(i);
            pos[heap[j]] = static_cast<int>(j);
        }

        void swim(size_t index) {
            while (index > 0) {
                size_t parent_index = (index - 1)/2;
                if (!comp(priority[heap[parent_index]], priority[heap[index]]))
                    break;
            
                swap(parent_index, index);
                index = parent_index;
            } 
        }

        void sink(size_t index) {
            while (true) {
                size_t left_index = 2 * index + 1;
                size_t right_index = 2 * index + 2;
                size_t largest_ind = index;

                if (left_index < count && comp(priority[heap[largest_ind]], priority[heap[left_index]]))
                    largest_ind = left_index;

                if (right_index < count && comp(priority[heap[largest_ind]], priority[heap[right_index]]))
                    largest_ind = right_index;

                if (largest_ind == index) {
                    break;
                }

                swap(largest_ind, index);
                index = largest_ind;
            }
        }

    public:
        explicit Indexed_Priority_Queue(size_t cap = 2) : capacity(cap), count(0) {
            heap = new Key[capacity];
            priority = new Priority[capacity];
            pos = new int[capacity];
            
            for (size_t i = 0; i < capacity; i++) {
                pos[i] = -1; //not inside heap yet.
            }
        }

        Indexed_Priority_Queue(const Indexed_Priority_Queue &other) : capacity(other.capacity), count(other.count) {
            heap = new Key[capacity];
            priority = new Priority[capacity];
            pos = new int[capacity];

            for (int i = 0; i < count; i++) {
                heap[i] = other.heap[i];
            }
            for (int i = 0; i < capacity; i++) {
                priority[i] = other.priority[i];
                pos[i] = other.pos[i];
            }
        } //Copy Constructor;
        
        Indexed_Priority_Queue(Indexed_Priority_Queue &&other) noexcept : capacity(other.capacity), count(other.count) {
            heap = other.heap;
            priority = other.priority;
            pos = other.pos;

            other.heap = nullptr;
            other.pos = nullptr;
            other.priority = nullptr;
            other.capacity = 0;
            other.count  = 0;
            
        } //Move Constructor;

        Indexed_Priority_Queue& operator=(const Indexed_Priority_Queue &other) {
            
            if (this == &other) {
                return *this;
            }

            capacity = other.capacity;
            count = other.count;

            delete[] heap;
            delete[] pos;
            delete[] priority;
            
            heap = new Key[capacity];
            priority = new Priority[capacity];
            pos = new int[capacity];

            for (int i = 0; i < count; i++) {
                heap[i] = other.heap[i];
            }
            for (int i = 0; i < capacity; i++) {
                priority[i] = other.priority[i];
                pos[i] = other.pos[i];
            }

            return *this;
        } //Copy Assigment

        Indexed_Priority_Queue& operator=(Indexed_Priority_Queue &&other) noexcept {
            if (this == &other) {
                return *this;
            }

            delete[] heap;
            delete[] priority;
            delete[] pos;

            capacity = other.capacity;
            count = other.count;
            heap = other.heap;
            priority = other.priority;
            pos = other.pos;

            other.pos = nullptr;
            other.priority = nullptr;
            other.heap = nullptr;
            other.capacity = 0;
            other.count = 0;

            return *this;
        } //Move Assignment

        ~Indexed_Priority_Queue() {
            delete[] heap;
            delete[] pos;
            delete[] priority;
        } //Destructor

        void enqueue(Key index, Priority val) noexcept {
            if (count >= capacity) {
                expand(capacity * 2);
            }
            if (index >= capacity) {
                expand(static_cast<size_t>(index) + 1);
            }
            if (contains(index)) return;

            priority[index] = val;

            size_t i = count++;
            heap[i] = index;
            pos[index] = static_cast<int>(i);

            swim(i);
        }

        void dequeue(void) {
            if (empty()) return;
            swap(0, count - 1);
            pos[heap[count - 1]] = -1;
            count--;
            sink(0);
        }

        Key topIndex(void) const {
            assert(!empty());
            return heap[0];
        }

        Priority topPriority(void) const {
            assert(!empty());
            return priority[heap[0]];
        }

        size_t size(void) const {
            return count;
        }

        bool empty(void) const {
            return (count == 0);
        }
        
        void changeKey(Key index, Priority value) {
            if (!contains(index)) return;
            priority[index] = value;

            int ind = pos[index];
            swim(ind);
            sink(ind);
        }

        bool contains(Key key) const {
            if (static_cast<size_t>(key) >= capacity)
                return false;

            return pos[key] != -1;
        }

        void removeKey(Key index) {
            if (!contains(index)) return;
            int position = pos[index];
            swap(position, count - 1);
            count--;
            if (position < count) {
                swim(position);
                sink(position);
            }
            pos[index] = -1;
        }

        void printHeap() const {
            std::cout << "Heap (pos -> index | priority)\n";
            for (size_t i = 0; i < count; i++) {
                Key idx = heap[i];
                std::cout << i << " -> " << idx << " (" << priority[idx] << ")\n";
            }
            std::cout << "\n";
        }

        bool isValidHeap() const {
            for (size_t i = 0; i < count; i++) {
                size_t left = 2 * i + 1;
                size_t right = 2 * i + 2;

                if (left < count &&
                    comp(priority[heap[i]], priority[heap[left]]))
                    return false;

                if (right < count &&
                    comp(priority[heap[i]], priority[heap[right]]))
                    return false;

                if (pos[heap[i]] != static_cast<int>(i))
                    return false;
            }
            return true;
        }
};

//Priority Queue functions
//push to heap
//pop
//top 

//Index Addons:
//contains(index)
//changekey(index, newValue)
//remove(index)
//O(logn) updates

