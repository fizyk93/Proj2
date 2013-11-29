#include "Analysis.h"
#include <cstdio>
#include <list>

using namespace std;

int Analysis::succeed = 0;
int Analysis::unsucceed = 0;

list<long> Analysis::changeTime = listInitialize();

list<long> Analysis::listInitialize()
{
    list<long> l;
    l.push_back(-1);
    return l;
}

Analysis::Analysis(long t, int u, std::list<Process> r)
{
    time = t;
    unusedProcs = u;
    readyTasks = r;

}

void Analysis::print()
{
    printf("Time: %d\tUnused processors: %d\t Ready tasks: ", time, unusedProcs);
    for(list<Process>::iterator it = readyTasks.begin(); it != readyTasks.end(); it++)
        printf("%d:%d:%d ", it->id, it->nproc, it->ready);
    printf("\n");
}

bool Analysis::operator<(Analysis b) const
{
    return (unusedProcs<=b.unusedProcs);
}

bool Analysis::operator>(Analysis b) const
{
    return (unusedProcs>=b.unusedProcs);
}
