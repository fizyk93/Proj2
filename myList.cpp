#include "myList.h"
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <vector>

using namespace std;
//
//template <class T>
//void myList<T>::push_front(const T& val)
//{
//    typename myList<T>::iterator it;
//    it = this->begin();
//    while(val < *it && it != this->end()) it++;
//    this->insert(it, val);
//}
//
//template <class T>
//void myList<T>::push_back(const T& val)
//{
//    typename myList<T>::iterator it;
//    it = this->begin();
//    while(val > *it && it != this->end()) it++;
//    this->insert(it, val);
//    if(this->size() > this->limit && this->limit > 0) this->pop_back();
//}
//
//template <class T>
//void myList<T>::shuffle()
//{
//    vector<int> v;
//    v.assign(this->begin(), this->end());
//    srand(time(NULL));
//    random_shuffle(v.begin(), v.end());
//    this->assign(v.begin(), v.end());
//}
//
//template <class T>
//void myList<T>::print()
//{
//    for(typename myList<T>::iterator it = this->begin(); it != this->end(); it++)
//        it->print();
//}

//void myList::push(Analysis x)
//{
//    std::list<Analysis>::iterator it = this->begin();
//    while(it->unusedProcs >= x.unusedProcs && it != end()) it++;
//    this->insert(it, x);
//    if(this->size() > 5)
//        this->pop_back();
//}
//
//void myList::print()
//{
//    for(std::list<Analysis>::iterator it = begin(); it != end(); it++)
//        it->print();
//
//    std::cout << "\nFailure: " << Analysis::unsucceed << "\tSuccess: " << Analysis::succeed << std::endl;
//}
