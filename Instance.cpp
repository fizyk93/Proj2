#include "Instance.h"

list<Analysis> Instance::changeTime = listInitialize();

list<Analysis> Instance::listInitialize()
{
    list<Analysis> l;
    list<Process> p;
    p.push_back(Process(-1, -1, -1, -1));
    l.push_back(Analysis(-1, -1, p));
    return l;
}

Instance::Instance(list<Process> processList, std::ofstream *f)
{
    this->processList = processList;
    output = f;
    counter = Process::maxProcs;
    for(long i=counter; i >= 1; i--) processors.push_back(i);
    changeIterator = Analysis::changeTime.begin();
    timer = 0;
    Analysis::unsucceed = 0;
    change = false;
    analysis = myList<Analysis>(10);
    ita = a.begin();

}

void Instance::startScheduler()
{
    startTime = clock();

    while(!processList.empty() || !readyList.empty() || !execList.empty())
    {
        //Program przesuwa sie o pewna jednostke czasu, wynikajaca z dwoch list
        //Listy procesow wchodzacych oraz listy czasow zakonczenia aktualnie dzialajacych zadan
        //Program wybiera z obu list ta, na ktorej znajduje sie najblizsze zadanie i przeskakuje do niego

        if(rand()%5000 == rand()%5000)
        {
            a.push_back(timer);
        }

        while(timer >= *changeIterator && changeIterator != Analysis::changeTime.end())
            changeIterator++;

        if(!processList.empty() && !finishTimes.empty())
        {
            if(processList.front().ready < finishTimes.front())
            {
                timer = processList.front().ready;
                updateReady();
                //readyList.sort(sortReady);
            }
            else
            {
                timer = finishTimes.front();
                while(finishTimes.front() == timer) finishTimes.pop_front();
                terminateProc();
            }
        }
        else if(!processList.empty() && finishTimes.empty())
        {
            timer = processList.front().ready;
            updateReady();
            //readyList.sort(sortReady);
        }
        else if(processList.empty() && !finishTimes.empty())
        {
            timer = finishTimes.front();
            while(finishTimes.front() == timer) finishTimes.pop_front();
            terminateProc();
        }

//        if(timer == changeTime || finishTimes.front() == changeTime || processList.front().ready == changeTime)
        if(change)
        {
            //cout << "Hello! " << timer << " Unused procs: " << counter <<endl;
            readyList.sort(sortChange);
        }

        if(timer >= *changeIterator && changeIterator != Analysis::changeTime.end() && !readyList.empty())
        {
            //cout << readyList.begin()->id << "\t";
            readyList.shuffle();

            //cout << readyList.begin()->id << "\t" << readyList.size() << "\n";
        }

        runProc();


    }

    endTime = clock();

}

//Wlasna funkcja sortowania potrzebna do dzialania programu
bool Instance::sortReady(Process a, Process b)
{
    return (a.exec > b.exec);
}

bool Instance::sortChange(Process a, Process b)
{
//    cout << "hello!\n";
    return (a.nproc > b.nproc);
}

//Funkcja wypisuje wszystkie procesy na konsole
void Instance::printProcList(list<Process> p)
{
    for(list<Process>::iterator it = p.begin(); it != p.end() && !p.empty(); it++)
        cout << "\n<<Proces>>\nPole 1: " << it->id << "\nPole 2: " << it->ready << "\nPole 4: " << it->exec << "\nPole 5: " << it->nproc  << "\n\n";

}

//Funkcja przenoszaca procesy z listy "procesy" na liste procesow gotowych do wykonania jezeli dane procesy maja czas wejscia <= obecny czas
void Instance::updateReady()
{
    for(list<Process>::iterator it = processList.begin(); it != processList.end() && it->ready <= timer; )
    {
        readyList.push_front(processList.front());
        it++;
        processList.pop_front();
    }
    //readyList.sort(sortReady);

}


/*Funkcja wyszukujaca wykonane procesy i usuwajaca je z listy exec.
Funkcja jednoczesnie zapisuje takie procesy do pliku wynikowego
Zwolnione procesory trafiaja ponownie na wektor dostepnych procesorow
*/

void Instance::terminateProc()
{
    for(list<Process>::iterator it = execList.begin(); it != execList.end() && !execList.empty();)
        if(it->finish == timer)
        {
            (*output) << it->id << " " << it->start << " " << it->finish << " "; //zapis danych procesu do pliku
            for(long i=0;i<(long)it->processors.size();i++)
            {
                (*output) << it->processors[i] << " "; //zapis procesorow wykorzystywanych przez zadanie do pliku
                processors.push_back(it->processors[i]); //zwracanie procesorow do listy wolnych procesorow
            }
            (*output) << "\n";
            counter += it->nproc; //zmienna zawierajaca aktualna ilosc procesorow jest zwiekszana o zwrocone procesory
            it=execList.erase(it);//usuwamy zadanie z listy wykonywanych

        }
        else it++;
}

//Funkcja przydzielajaca procesory procesom oraz przenoszaca je z listy "ready" na liste "exec"
void Instance::runProc()//long time, list<long> *finishTimes, list<Process> *x, list<Process> *r, long *counter,vector<int> *proc, myList& an)
{
    //list<Process> tmp = *r;
    for(myList<Process>::iterator it = readyList.begin(); it != readyList.end() && counter != 0 && !(readyList.empty());)
    {

        if(it->nproc <= counter) //jezeli zadanie
        {
//            if(*changeIterator == -1) cout << "Hello!\n";
//            cout << "#################\t" << timer;
//            printProcList(readyList);

            it->start = timer; //faktyczny czas rozpoczecia zadania
            it->finish = timer + it->exec; //faktyczny czas zakonczenia zadania
            long i = it->nproc; //dodawanie procesorow do zadania
           // cout << it->id << " " << i << endl; system("PAUSE");
            while(i>0)
            {
                //if((*proc).empty()) { cout << "BLAD"; system("PAUSE"); }

                i--;
                it->processors.push_back(processors[processors.size()-1]); //procesory z proc sa przenoszone do wektora procesorow uzywanych przez zadanie
                processors.pop_back();
            }
            //if(it->processors.size() != it->nproc) { cout << "BLAD " << it->id; system("PAUSE"); }
            execList.push_back(*it); //zadanie trafia na liste procesow wykonywanych
            finishTimes.push_back(it->finish); //czas zakonczenia zadania trafia na liste zawierajaca czasy zakonczenia dzialajacych zadan
//            if(timer >= *changeIterator)
//            {
//                change = false;
//                changeIterator++;
//            }
//            if(it->finish >= *changeIterator && timer <= *changeIterator) change = true;

            counter -= it->nproc;
            it=readyList.erase(it);

        }
        else
        {
            it++;
//            if(it == readyList.end())
//                if(readyList.empty() || counter == 0 || (change && counter-it->nproc < analysis.begin()->readyTasks.begin()->nproc && timer < *changeIterator))
//                {
////                    cout << "Hello!\n";
//                    Analysis::succeed++;
//                }
//                else
//                {
//                    analysis.push_front(Analysis(timer, counter, readyList));
//                    Analysis::unsucceed++;
//                }
        }
    }
    finishTimes.sort(); //lista czasow w ktorych koncza sie zadania jest sortowana
}

void Instance::printSummary(string filename)
{
    cout << "Czas uszeregowania na podstawie p_j: " << timer << endl;
    //Wypisanie wiadomosci koncowych oraz wyczyszczenie tablic
    cout << "Plik wynikowy " << filename << " zostal pomyslnie zapisany.\n";

    cout << "Czas wykonania programu: " << (float)(endTime-startTime)/CLOCKS_PER_SEC << "s" << endl;

    analysis.print();
}
