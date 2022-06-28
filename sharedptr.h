#ifndef SHAREDPTR_SHAREDPTR_H
#define SHAREDPTR_SHAREDPTR_H

#include <iostream>


template<typename T>
class SharedPtr
{
private:
    T *_pointer = nullptr;
    size_t *_counter = nullptr;

public:
    SharedPtr(T *ptr = nullptr) {
        _pointer = ptr;
        if (ptr) { _counter = new size_t (1); }
    }

    SharedPtr(const SharedPtr& oldPtr) {
        _counter = oldPtr._counter;
        if (_counter) {
            (*_counter)++;
            _pointer = oldPtr._pointer;
        }
    }

    SharedPtr &operator=(const T *ptr) {
        if (_pointer == ptr) { return *this; }
        decrease__counter();
        _counter = new size_t(1);
        _pointer = ptr;
        return *this;
    }

    SharedPtr &operator=(const SharedPtr<T> &ptr) {
        if (_pointer == ptr._pointer) { return *this; }
        decrease__counter();
        _pointer = ptr._pointer;
        _counter = ptr._counter;
        if (_counter) { (*_counter)++; }
        return *this;
    }

    inline ~SharedPtr() { decrease__counter(); }

    SharedPtr (SharedPtr&& ptr) noexcept {
        _counter = ptr._counter;
        _pointer = ptr._pointer;
        ptr._pointer = nullptr;
        ptr._counter = nullptr;
    }

    SharedPtr& operator=(SharedPtr &&ptr) noexcept
    {
        if (_pointer == ptr._pointer) { return *this; }
        decrease__counter();
        _counter = ptr._counter;
        _pointer = ptr._pointer;
        ptr._pointer = nullptr;
        ptr._counter = nullptr;
        return *this;
    }

    void decrease__counter() {
        if (_counter && _pointer) {
            (*_counter)--;
            if (*_counter == 0) {
                delete _pointer;
                delete _counter;
                _pointer = nullptr;
                _counter = nullptr;
            }
        }
    }

    inline T &operator* () noexcept { return *_pointer; }

    inline const T &operator* () const noexcept { return *_pointer; }

    inline const T *operator-> () const noexcept { return _pointer; }

    inline T *operator-> () noexcept { return _pointer; }

    void reset(T *ptr) {
        decrease__counter();
        _pointer = ptr;
        if (ptr) { _counter = new size_t (1); }
    }

    void swap(SharedPtr& other) noexcept {
        T *old_ptr = _pointer;
        size_t *old__counter = _counter;
        _pointer = other._pointer;
        _counter = other._counter;
        other._pointer = old_ptr;
        other._counter = old__counter;
    }

    inline T *get() const noexcept { return _pointer; }

    explicit operator bool() const noexcept { return (_pointer != nullptr); }
};

#endif //SHAREDPTR_SHAREDPTR_H
