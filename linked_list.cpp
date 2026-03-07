#include <iostream>
#include <vector>
#include <cassert>

class LinkedList {
    private:
        typedef struct Node {
            int val;
            Node *next;

            Node() : val(0), next(nullptr) {};
            Node(int value) : val(value), next(nullptr) {};
        } Node;

        Node *head = nullptr;
        Node *tail = nullptr;
        size_t count;

    public: 
        LinkedList() : head(nullptr), count(0), tail(nullptr) {};

        LinkedList(const LinkedList &other) : count(0), head(nullptr), tail(nullptr) {
            Node *ptr = other.head;
            while (ptr != nullptr) {
                Node *node = new Node(ptr -> val);
                if (head == nullptr) {
                    head = node;
                    tail = node;
                } else {
                    tail -> next = node;
                    tail = node;
                }

                ptr = ptr -> next;
                count++;
            }
        };
        
        LinkedList(LinkedList &&other) {
            head = other.head;
            count = other.count;
            tail = other.tail;

            other.head = nullptr;
            other.tail = nullptr;
            other.count = 0;
        }

        LinkedList& operator=(const LinkedList &other) {
            if (this == &other) {
                return *this;
            }

            clear();
            Node *ptr = other.head;
            while (ptr != nullptr) {
                Node *node = new Node(ptr -> val);

                if (head == nullptr) {
                    head = node;
                    tail = node;
                } else {
                    tail -> next = node;
                    tail = node;
                }

                ptr = ptr -> next;
                count++;
            }

            return *this;
        }

        LinkedList& operator=(LinkedList &&other) {
            if (this == &other) {
                return *this;
            }

            clear();

            head = other.head;
            tail = other.tail;
            count = other.count;

            other.head = nullptr;
            other.tail = nullptr;
            other.count = 0;

            return *this;
        }

        ~LinkedList() {
            Node *ptr = head;
            while (ptr != nullptr) {
                Node *next = ptr -> next;
                delete ptr;
                ptr = next;
            }
            head = nullptr;
            tail = nullptr;
            count = 0;
        }

        class Iterator {
            private:
                Node *current;
            public:
                Iterator(Node *start) : current(start) {};

                int& operator*(void) const noexcept {
                    return current -> val;
                }

                Iterator& operator++(void) noexcept {
                    current = current -> next;
                    return *this;
                }

                bool operator==(const Iterator& other) const noexcept {
                    return (current == other.current);
                }

                bool operator!=(const Iterator& other) const {
                    return (!(current == other.current));
                }
        };

        Iterator begin() noexcept {return Iterator(head);}
        Iterator end() noexcept {return Iterator(nullptr);}

        void push_back(int value) {
            Node *node = new Node(value);
            if (head == nullptr) {
                head = node;
                tail = node;
                count++;
                return;
            }

            tail -> next = node;
            tail = node;
            count++;
        }

        void push_front(int value) {
            Node *node = new Node(value);
            if (head == nullptr) {
                head = node;
                tail = node;
                count++;
                return;
            }
            Node *ptr = head;

            head = node;
            node -> next = ptr;
            count++;
        }

        void pop_head(void) noexcept {
            assert(!empty());
            Node *ptr = head;
            head = ptr -> next;
            if (head == nullptr) tail = nullptr;
            count--;
            delete ptr;
        }
        
        void pop_back(void) noexcept {
            assert(!empty());
            Node *ptr = head;
            if (count == 1) {
                delete head;
                head = nullptr; tail = nullptr;
                count--;
                return;
            }
            while ((ptr -> next) -> next != nullptr) {
                ptr = ptr -> next;
            }
            Node *end = ptr -> next;
            delete end;
            tail = ptr;
            tail -> next = nullptr;
            count--;
        }

        void remove(int value) {
            Node *ptr = head;
            Node *prev = nullptr;

            while (ptr != nullptr && ptr -> val != value) {
                prev = ptr;
                ptr = ptr -> next;
            }
            
            if (prev == nullptr) return;
            if (ptr == nullptr) return;
            prev -> next = ptr -> next;
            if (prev -> next == nullptr) tail = prev;
            delete ptr;
            count--;
        }

        int search(int value) const noexcept {
            Node *ptr = head; int num = 0;
            while (ptr != nullptr && ptr -> val != value) {
                ptr = ptr -> next;
                num++;
            }

            if (ptr == nullptr) return -1;
            if (ptr -> val != value) return -1;
            return num;
        }

        int access(int index) const noexcept {
            if (index >= count) return -1;
            Node *ptr = head; int num = 0;
            while (ptr != nullptr && num < index) {
                ptr = ptr -> next;
                num++;
            }
            return ptr -> val;
        }

        void clear(void) noexcept {
            Node *ptr = head;
            while (ptr != nullptr) {
                Node *next = ptr -> next;
                delete ptr;
                ptr = next;
            }
            head = nullptr;
            tail = nullptr;
            count = 0;
        }

        void displayNodes(void) const noexcept {
            Node *ptr = head;
            while (ptr != nullptr) {
                std::cout << ptr -> val << " ";
                ptr = ptr -> next;
            }
            std::cout << "\n";
        }

        size_t size(void) const noexcept {
            return count;
        }

        bool empty(void) const noexcept {
            return (count == 0);
        }

        int front(void) const noexcept {
            assert(!empty());
            return head -> val;
        }

        int back(void) const noexcept {
            assert(!empty());
            return tail -> val;
        }
};