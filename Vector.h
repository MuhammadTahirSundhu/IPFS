#pragma once
#include <iostream>
#include "File.h"
template <class T>
class Vector
{
    T* arr;
    int capacity;


public:
    int size;
    Vector() : arr(nullptr), size(0), capacity(0) {}
    Vector(int s) : arr(new T[s]{ NULL }), size(0), capacity(s) {}
    Vector(const Vector& arrayToCopy) : size(arrayToCopy.size), capacity(arrayToCopy.capacity)
    {
        arr = new T[size];

        for (int i = 0; i < size; i++)
            arr[i] = arrayToCopy.arr[i];
    }

    void push_back(const T& x)
    {
        if (arr != nullptr)
        {
            if (size < capacity)
            {
                arr[size] = x;
                size++;
            }
            else
            {
                capacity++;
                T* temp = new T[capacity];
                for (int i = 0; i < size; i++)
                    temp[i] = arr[i];
                temp[size] = x;
                delete[] arr;
                arr = temp;
                size++;
            }
        }
        else
        {
            capacity++;
            arr = new T[capacity];
            arr[size] = x;
            size++;
        }
    }
    void pop(int x = 1)
    {

        size -= x;
        if (size < 0)
            size = 0;
    }
    T peek() {
        return arr[size - 1];
    }
    T& operator[](int x)
    {
        return arr[x];
    }
    T operator[](int x) const
    {
        return arr[x];
    }

    int includes(T element)
    {
        for (int i = 0; i < size; i++)
            if (element == arr[i])
                return i;
        return -1;
    }

    void addInOrder(T& value) {
        int i = size - 1;
        while (i >= 0 && arr[i] > value) {
            arr[i + 1] = arr[i];
            --i;
        }

        arr[i + 1] = value;
        ++size;
    }
    void insert(const T& value, int idx)
    {
        if (idx < 0 || idx > size) {
            std::cout << "idx out of bounds." << std::endl;
            return;
        }

        if (size < capacity) {
            for (int i = size; i > idx; --i)
                arr[i] = arr[i - 1];

            arr[idx] = value;
            size++;
        }
        else {
            capacity++;
            T* temp = new T[capacity];
            for (int i = 0; i < idx; ++i)
                temp[i] = arr[i];

            temp[idx] = value;

            for (int i = idx + 1; i <= size; ++i)
                temp[i] = arr[i - 1];

            delete[] arr;
            arr = temp;
            size++;
        }
    }

    void remove(int idx) {
        if (idx < 0 || idx >= size) {
            std::cout << "Invalid idx for removal." << std::endl;
            return;
        }

        for (int i = idx; i < size - 1; ++i) {
            arr[i] = arr[i + 1];
        }

        size--;
    }
    ~Vector() { delete arr; arr = nullptr; }
};

template <>
void Vector<File*>::addInOrder(File*& value) {
    int i = size - 1;
    while (i >= 0 && arr[i]->Hash > value->Hash) {
        arr[i + 1] = arr[i];
        --i;
    }


    arr[i + 1] = value;
    ++size;

}
template <>
int Vector<File*>::includes(File* element)
{
    for (int i = 0; i < size; i++)
        if (element->Hash == arr[i]->Hash)
            return i;
    return -1;
}