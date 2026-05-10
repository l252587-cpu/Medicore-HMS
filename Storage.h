#ifndef STORAGE_H
#define STORAGE_H

#include <iostream>
using namespace std;

template <class T>
class Storage {
private:
    T data[100];
    int count;

public:
    // constructor
    Storage() {
        count = 0;
    }

    // add object
    void add(T obj) {
        if (count < 100) {
            data[count] = obj;
            count++;
        }
        else {
            cout << "Storage is full." << endl;
        }
    }

    // remove by ID
    void removeById(int id) {
        for (int i = 0; i < count; i++) {
            if (data[i].getid() == id) {
                // shift everything after it one position back
                for (int j = i; j < count - 1; j++) {
                    data[j] = data[j + 1];
                }
                count--;
                return;
            }
        }
        cout << "Record not found." << endl;
    }

    // find by ID
    T* findById(int id) {
        for (int i = 0; i < count; i++) {
            if (data[i].getid() == id) {
                return &data[i];
            }
        }
        return nullptr;
    }

    // get all objects
    T* getAll() {
        return data;
    }

    // get count
    int size() {
        return count;
    }

    // clear all
    void clear() {
        count = 0;
    }
};

#endif
