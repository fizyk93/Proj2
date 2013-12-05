#ifndef MYLIST_H
#define MYLIST_H
#include "Analysis.h"
#include <list>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

template <class T>
class myList
    : public std::list<T>
{
public:
    int limit;
    myList(int x = -1) : limit(x) {};

    void push_back(const T& val);
    void push_front(const T& val);
    void shuffle();
    void print();
};

template <class T>
void myList<T>::push_front(const T& val)
{
    typename myList<T>::iterator it;
    it = this->begin();
    while(*it > val && it != this->end())
    {
        it++;
    }
    this->insert(it, val);
    if(this->size() > this->limit && this->limit > 0) this->pop_back();
}

template <class T>
void myList<T>::push_back(const T& val)
{
    typename myList<T>::iterator it;
    it = this->begin();
    while(*it < val && it != this->end()) it++;
    this->insert(it, val);
    if(this->size() > this->limit && this->limit > 0) this->pop_back();
}

template <class T>
void myList<T>::shuffle()
{
    std::vector<T> v;
    v.assign(this->begin(), this->end());
    srand(time(NULL));
    random_shuffle(v.begin(), v.end());
    this->assign(v.begin(), v.end());
}

template <class T>
void myList<T>::print()
{
    for(typename myList<T>::iterator it = this->begin(); it != this->end(); it++)
        it->print();

    std::cout << std::endl;
}

#endif // MYLIST_H
