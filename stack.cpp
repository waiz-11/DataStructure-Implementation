#include <iostream>
#include <stdlib.h>
#include <utility>
#include <cassert>
#include <initializer_list>

template <typename T>
class Stack {
    private:
        size_t capacity_;
        T *stack;
        size_t top_ = 0;

        void reserve(size_t new_capacity) {
            if (new_capacity <= capacity_)
                return;

            T* new_stack = new T[new_capacity];

            for (size_t i = 0; i < top_; ++i)
                new_stack[i] = std::move(stack[i]);

            delete[] stack;
            stack = new_stack;
            capacity_ = new_capacity;
        }

    public:
        explicit Stack(size_t cap = 2) : capacity_(cap), top_(0) {
            stack = new T[capacity_];
        } //Default Constructor

        Stack(std::initializer_list <T> init) : capacity_(init.size()), stack(new T[capacity_]), top_(0) {
            reserve(init.size());
            for (const T& value : init) {
                stack[top_++] = value;
            }
        } //Initializer list Constructor

        Stack(const Stack &other) : capacity_(other.capacity_), top_(other.top_) {
            stack = new T[capacity_];
            for (size_t i = 0; i < top_; i++) {
                stack[i] = other.stack[i];
            }
        } //Copy Constructor

        Stack(Stack &&other) noexcept : capacity_(other.capacity_), top_(other.top_), stack(other.stack) {
            other.stack = nullptr;
            other.capacity_ = 0;
            other.top_ = 0;
        } //Move Constructor

        Stack& operator=(const Stack &other) {
            if (this == &other) {
                return *this;
            }

            delete[] stack;
            capacity_ = other.capacity_;
            top_ = other.top_;

            stack = new T[capacity_];
            for (size_t i = 0; i < top_; i++) {
                stack[i] = other.stack[i];
            }
            return *this;
        } //Copy Assignment

        Stack& operator=(Stack &&other) noexcept {
            if (this != &other) {
                delete[] stack;

                stack = other.stack;
                top_ = other.top_;
                capacity_ = other.capacity_;

                other.capacity_ = 0;
                other.top_ = 0;
                other.stack = nullptr;
            }
            return *this;
        } //Move Assignment

        ~Stack() {
            delete[] stack;
        } //Destructor

        bool operator==(const Stack &other) const {
            if (top_ != other.top_) return false;
            for (size_t i = 0; i < top_; ++i) {
                if (stack[i] != other.stack[i]) return false;
            }
            return true;
        } //Overloading '=='

        bool operator!=(const Stack &other) const {
            return !(*this == other);
        } //Overloading '!='

        void swap(Stack &other) noexcept {
            std::swap(other.capacity_, capacity_);
            std::swap(other.top_, top_);
            std::swap(other.stack, stack);
        }

        void push(const T& value) {
            if (top_ == capacity_) {
                reserve(capacity_ ? capacity_ * 2 : 1);
            }
            stack[top_++] = value;
        }

        void push(T&& value) {
            if (top_ == capacity_)
                reserve(capacity_ ? capacity_ * 2 : 1);

            stack[top_++] = std::move(value);
        }

        template <typename... Args>
        void emplace(Args&&... args) {
            if (top_ == capacity_) {
                reserve(capacity_ ? capacity_ * 2 : 1);
            }
            stack[top_++] = T(std::forward<Args>(args)...);
        }

        void pop(void) {
            assert(!empty());
            --top_;
        }

        T& top(void) {
            assert(!empty());
            return stack[top_ - 1];
        }

        const T& top(void) const {
            assert(!empty());
            return stack[top_ - 1];
        }

        bool empty(void) const noexcept{
            return top_ == 0;
        }

        size_t size(void) const noexcept {
            return top_;
        }

        void clear() noexcept {
            top_ = 0;
        }
};