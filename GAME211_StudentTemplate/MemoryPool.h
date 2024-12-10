#pragma once
#include <vector>
#include <cassert>
#include <iostream>

template<typename T>
class MemoryPool {
public:
    MemoryPool(size_t poolSize) {
        // Pre-allocate a block of objects
        objects.reserve(poolSize);
        for (size_t i = 0; i < poolSize; i++) {
            objects.push_back(new T());
        }
    }

    ~MemoryPool() {
        for (auto obj : objects) {
            delete obj;
        }
    }

    T* getObject() {
        if (objects.empty()) {
            std::cout << "Pool exhausted! No objects available.\n";
            return nullptr; // Indicate that the pool is out of objects
        }
        T* obj = objects.back();
        objects.pop_back();
        return obj;
    }


    void ReturnObject(T* obj) {
 
        objects.push_back(obj);

    }

private:
    std::vector<T*> objects;
};
