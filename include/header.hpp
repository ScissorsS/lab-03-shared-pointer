// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <string>
#include <atomic>
#include <map>

using std::string;
using std::map;
using std::atomic_uint;

template <typename T>
class SharedPtr {
public:
    
    SharedPtr() {
        _ptr = nullptr;
    }
    
    explicit SharedPtr(T* ptr) {
        _ptr = ptr;
        SharedPtr::data_base[_ptr] = new atomic_uint(1);
    }
    
    explicit SharedPtr(const SharedPtr& r) {
        _ptr = r._ptr;
        if (_ptr != nullptr)
            (*SharedPtr::data_base[_ptr])++;
    }
    
    explicit SharedPtr(SharedPtr&& r) {
        _ptr = r._ptr;
        r._ptr = nullptr;
    }
    
    ~SharedPtr() {
        if (_ptr == nullptr)
            return;
        if (SharedPtr::data_base[_ptr] == nullptr)
            _ptr = nullptr;
        (*SharedPtr::data_base[_ptr])--;
        if ((*SharedPtr::data_base[_ptr]) == 0) {
            delete SharedPtr::data_base[_ptr];
            SharedPtr::data_base.erase(_ptr);
            delete _ptr;
        }
        _ptr = nullptr;
    }
    
    auto operator=(const SharedPtr& r) -> SharedPtr& {
        if (&r == this)
            return *this;
        this->~SharedPtr();
        _ptr = r._ptr;
        if (_ptr != nullptr)
            (*SharedPtr::data_base[_ptr])++;
        return *this;
    }
    
    auto operator=(SharedPtr&& r) -> SharedPtr& {
        if (&r == this)
            return *this;
        this->~SharedPtr();

        _ptr = r._ptr;
        r._ptr = nullptr;
        return *this;
    }

    operator bool() const {
        return (_ptr != nullptr);
    }
    
    auto operator*() const -> T& {
        return *_ptr;
    }
    
    auto operator->() const -> T* {
        return _ptr;
    }

    auto get() -> T* {
        return _ptr;
    }
    
    void reset() {
         this->~SharedPtr();
    }
    
    void reset(T* ptr) {
        if (_ptr != nullptr)
            if ((*SharedPtr::data_base[_ptr]) != 0)
                (*SharedPtr::data_base[_ptr])--;
        _ptr = nullptr;
        if (ptr == nullptr)
            return;
        _ptr = ptr;
        if ((*SharedPtr::data_base[_ptr]))
            (*SharedPtr::data_base[_ptr])++;
        SharedPtr::data_base[_ptr] = new atomic_uint(1);
    }
    
    void Swap(SharedPtr& r) {
        T* temp;
        temp = this->_ptr;
        this->_ptr = r._ptr;
        r._ptr = temp;
        temp = nullptr;
    }
     
    auto use_count() const -> size_t {
        if (_ptr == nullptr)
            return 0;
        size_t number = (*SharedPtr::data_base[_ptr]);
        return number;
    }
    
    static std::map <T*, std::atomic_uint*> data_base;
private:
    
    T* _ptr;
};

template <typename T>
std::map<T*, std::atomic_uint*> SharedPtr<T>::data_base{};

#endif // INCLUDE_HEADER_HPP_
