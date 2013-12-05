#include "Instance.h"

int Instance::size = 0;

list<long> Instance::changeTime = list<long>();
list<long> Instance::prevChangeTime = list<long>();

Instance::Instance(list<Process> processList, std::ofstream *f)
{
    this->processList = processList;
    output = f;
    counter = Process::maxProcs;
    for(long i=counter; i >= 1; i--) processors.push_back(i);
    changeIterator = changeTime.begin();
    timer = 0;
}

void Instance::startScheduler()
{
    startTime = (float)clock()/CLOCKS_PER_SEC;

    while(!processList.empty() || !readyList.empty() || !execList.empty())
    {
        //Program przesuwa sie o pewna jednostke czasu, wynikajaca z dwoch list
        //Listy procesow wchodzacych oraz listy czasow zakonczenia aktualnie dzialajacych zadan
        //Program wybiera z obu list ta, na ktorej znajduje sie najblizsze zadanie i przeskakuje do niego

        if(size>=20 && rand()%(size/20) == rand()%(size/20))
        {
            newChangeTime.push_back(timer);
        }

        while(timer >= *changeIterator && changeIterator != changeTime.end())
            changeIterator++;

        if(!processList.empty() && !finishTimes.empty())
        {
            if(processList.front().ready < finishTimes.front())
            {
                timer = processList.front().ready;
                updateReady();
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
        }
        else if(processList.empty() && !finishTimes.empty())
        {
            timer = finishTimes.front();
            while(finishTimes.front() == timer) finishTimes.pop_front();
            terminateProc();
        }

        if(timer >= *changeIterator && changeIterator != changeTime.end() && !readyList.empty())
        {
            readyList.shuffle();
        }

        runProc();


    }

    endTime = (float)clock()/CLOCKS_PER_SEC;

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
void Instance::runProc()
{
    for(myList<Process>::iterator it = readyList.begin(); it != readyList.end() && counter != 0 && !(readyList.empty());)
    {

        if(it->nproc <= counter)
        {


            it->start = timer; //faktyczny czas rozpoczecia zadania
            it->finish = timer + it->exec; //faktyczny czas zakonczenia zadania
            long i = it->nproc; //dodawanie procesorow do zadania

            while(i>0)
            {
                i--;
                it->processors.push_back(processors[processors.size()-1]); //procesory z proc sa przenoszone do wektora procesorow uzywanych przez zadanie
                processors.pop_back();
            }

            execList.push_back(*it); //zadanie trafia na liste procesow wykonywanych
            finishTimes.push_back(it->finish); //czas zakonczenia zadania trafia na liste zawierajaca czasy zakonczenia dzialajacych zadan

            counter -= it->nproc;
            it=readyList.erase(it);

        }
        else
        {
            it++;
        }
    }
    finishTimes.sort(); //lista czasow w ktorych koncza sie zadania jest sortowana
}

void Instance::printSummary(string filename)
{
    cout << "Czas uszeregowania na podstawie p_j: " << timer << endl;
    //Wypisanie wiadomosci koncowych oraz wyczyszczenie tablic
    cout << "Plik wynikowy " << filename << " zostal pomyslnie zapisany.\n";
    executionTime = endTime-startTime;
    cout << "Czas wykonania programu: " << executionTime << "s" << endl << endl;
}
