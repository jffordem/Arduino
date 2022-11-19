/*
MIT License

Copyright (c) 2022 jffordem

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

template <class T>
class Enumerable {
public:
    virtual T current() const = 0;
    virtual void next() = 0;
    virtual bool done() const = 0;
};

template <class T>
class IList {
public:
    virtual void add(T item) = 0;
    virtual int length() const = 0;
    virtual T item(int index) const = 0;
    virtual T operator[](int index) const { return item(index); }
};

#define USE_LINKED_LIST
#ifdef USE_LINKED_LIST

template <class T, class U>
class Pair {
	T _car;
	U _cdr;
public:
	Pair(T car, U cdr) : _car(car), _cdr(cdr) { }
	T car() const { return _car; }
	U cdr() const { return _cdr; }
};

template <class T>
class ListPair : public Pair<T, ListPair<T>*> {
public:
	ListPair(T head, ListPair<T> *tail) : Pair<T, ListPair<T>*>(head, tail) { }
};

template <class T>
class List : public IList<T> {
    ListPair<T> *_list;
public:
    List(T *items = NULL, int count = 0) : _list(NULL) {
        addAll(items, count);
    }
    void addAll(T *items, int count) {
        for (int i = 0; i < count; i++) {
            add(items[i]);
        }
    }
    void add(T item) {
        _list = new ListPair<T>(item, _list);
    }
    ListPair<T> *head() {
        return _list;
    }
    int length() const {
        int count = 0;
        ListPair<T> *temp = _list;
        while (temp) {
            count++;
            temp = temp->cdr();
        }
        return count;
    }
    T item(int index) const {
        ListPair<T> *temp = _list;
        for (int i = 0; i < index && temp; i++) {
            temp = temp->cdr();
        }
        return temp->car();
    }
};

template <class T>
int countZ(const T *itemsZ) {
    int result = 0;
    for (int i = 0; itemsZ && itemsZ[i]; i++) {
        result = i;
    }
    return result;
}

#else

#define MAX_LIST_SIZE 30

template <class T>
class List : public IList<T> {
    T _data[MAX_LIST_SIZE];
    int _length;
public:
    List() : _length(0) { }
    void add(T item) {
        _data[_length++] = item;
    }
    int length() const { return _length; }
    T item(int index) const { return _data[index]; }
};

#endif

// Just needed to test out the list thingy.
int sum(List<int> &numbers) {
    int result = 0;
    for (int i = 0; i < numbers.length(); i++) {
        result += numbers[i];
    }
    return result;
}

#endif
