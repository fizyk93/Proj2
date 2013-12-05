#ifndef INSTANCE_H_INCLUDED
#define INSTANCE_H_INCLUDED

#include <vector>
#include <list>
#include <fstream>
#include <ctime>
#include <iostream>
#define TIME 180000

#include "myList.h"
#include "Process.h"

using namespace std;

class Instance
{
public:
    vector<int> processors;
    list<Process> processList, execList, tmp;
    myList<Process> readyList;
    list<long> finishTimes;
    std::ofstream *output;
    int counter;
    long timer;

    float startTime, endTime, executionTime;

    list<long> newChangeTime;
    static std::list<long> changeTime, prevChangeTime;

    list<long>::iterator changeIterator;

    static int size;

    Instance(list<Process> processList, std::ofstream *f);

    void startScheduler();

    //Funckja wypisujaca procesy na ekran konsoli
    void printProcList(list<Process> p);
    //Funkcja przenoszaca procesy z listy "Procesy" na "ready"
    void updateReady();
    //Funkcja usuwajaca wykonane procesy z programu
    void terminateProc();
    //Funkcja przydzielajaca procesory oraz przenoszaca proces z "ready" na "exec"
    void runProc();

    void printSummary(string filename);


};

#endif // INSTANCE_H_INCLUDED
