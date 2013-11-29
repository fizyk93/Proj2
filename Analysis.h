#ifndef ANALYSIS_H
#define ANALYSIS_H
#include <list>
#include "Process.h"
#include <iostream>
#include <vector>


class Analysis
{
public:

	long time;
	int unusedProcs;
	std::list<Process> readyTasks;
	static int succeed, unsucceed;
	static std::list<long> changeTime;

	static std::list<long> listInitialize();

    Analysis(long t, int u, std::list<Process> r);

	void print();

	bool operator<(Analysis b) const;

	bool operator>(Analysis b) const;

};

#endif // ANALYSIS_H
