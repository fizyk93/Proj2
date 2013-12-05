#include <iostream>
#include <list>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cmath>
#include "Instance.h"

//Maksymalny czas trwania programu w milisekundach
#define MAX_TIME 180 //3 minuty
///TESTOWY KOMENTARZ

///Dodaje kolejny kretynski komentarz

using namespace std;

bool mysort(Process a, Process b)
{
    return (a.ready < b.ready);
}

std::list<Process> readProcList(int argc, char** argv);

int main(int argc, char* argv[])
{
    srand(time(NULL));
    //Sprawdzenie czy podane zostaly wszystkie niezbedne informacje
    if(argc<=1) { cout << "PARALLEL TASKS SCHEDULING\nAby uruchomic podaj na wejsciu:\n[plik zrodlowy] [ilosc linii do wczytania|opcjonalne]\n"; return 0; }
    //vector zawierajacy wszystkie procesory dostepne w danej chwili

    long bestTime;
    float globalTime;
    double temperature, chance, probability, coolingRate;
    int maxDiscard = 0;

    ofstream output;
    string filename = argv[1];
    filename = filename.substr(0,filename.find("."));
    filename+="_s.txt";
    output.open(filename.c_str());

    list<Process> processList = readProcList(argc, argv);

    Instance::size = processList.size();

    Instance *mainInstance = new Instance(processList, &output);

    mainInstance->startScheduler();
    mainInstance->printSummary(filename);

    globalTime = mainInstance->endTime;
    bestTime = mainInstance->timer;

    Instance::changeTime = mainInstance->newChangeTime;

    output.close();

    temperature = bestTime/20000;
    coolingRate = 0.98;

    while(temperature>=1 && maxDiscard<=30 && globalTime < MAX_TIME-5)
    {
        cout << "GlobalTime: " << globalTime <<endl;
        cout << "ChangeTime list size: " << Instance::changeTime.size() << endl;
        string tmpfilename = argv[1];
        tmpfilename = tmpfilename.substr(0,tmpfilename.find("."));
        tmpfilename+="_tmp.txt";
        output.open(tmpfilename.c_str());

        Instance *newInstance = new Instance(processList, &output);
        newInstance->startScheduler();
        newInstance->printSummary(filename);

        chance = (double(rand())/RAND_MAX)*100.0;
        probability = exp((double(bestTime-newInstance->timer)/temperature))*100;

        cout << "Temperatura: " << temperature << " chance: " << chance << " probability: " <<  probability  << " " << newInstance->timer << endl;

        if( chance<=probability)
        {
            cout << "Przyjete " << endl;
            Instance::prevChangeTime = Instance::changeTime;
            if(!(newInstance->timer==bestTime))   maxDiscard = 0;
            else maxDiscard++;
            output.close();

            if(newInstance->timer<bestTime)
            {
                remove(filename.c_str());
                std::rename(tmpfilename.c_str(),filename.c_str());
            }

        }
        else
        {
            cout << "Odrzucone " << endl;
            Instance::changeTime = Instance::prevChangeTime;
            maxDiscard++;
            output.close();
            remove(tmpfilename.c_str());
        }

        if(newInstance->timer>bestTime)
            temperature*=coolingRate;
        else
            bestTime = newInstance->timer;

        globalTime += newInstance->executionTime;

        Instance::changeTime.merge(newInstance->newChangeTime);
        Instance::changeTime.unique();

        cout << "\n############################################################################################\n\n";

        delete newInstance;

    }

    delete mainInstance;

    cout << "Wynik:\n\nmainInstance: " << mainInstance->timer << " bestTime: " << bestTime << endl;

    return 0;
}

std::list<Process> readProcList(int argc, char** argv)
{
    //Licznik blednych wpisow oraz zmienne potrzebne do wczytywania procesow
    int blad = 0;
    ifstream input;
    string line = "";
    long lines;

    list<Process> processList;

    input.open(argv[1]);
    if(!input.good()) cout << "\nUWAGA: Plik zrodlowy nie istnieje!\n\n";
    //Jezeli podana zostala odpowiednia lista parametrow
    if(argc > 2)
    {
        //ustawiona jest ilosc linii do wczytania
        lines = atoi(argv[2]);
        if(lines==0) lines=-1;
    }
    else lines = -1;
    /*Tak dlugo jak mozliwe jest wczytanie linii z pliku, linia zostaje wczytana, a nastepnie wyodrebnione
    zostaja potrzebne informacje takie jak MaxJobs czy MaxProcs
    */
    while(getline(input,line) && lines!=0)
    {
        if(line[0] == ';') /// jezeli wczytana linia zaczyna sie od ;
        {
            if(line.find("MaxJobs:")!=string::npos) /// i jest w niej taki wpis
            {
                stringstream data(line);
                string tmp;
                //Zamiana znaku : na spacje w celu zagwarantowania poprawnosci wczytywania danych
                int pos = line.find(":");
                line[pos] = ' ';
                while(data>>tmp) {};
                Process::maxJobs = atoi(tmp.c_str());
                if(lines==-1 || lines>Process::maxJobs) lines = Process::maxJobs;
            }
            if(line.find("MaxProcs:")!=string::npos) // albo taki
            {
                stringstream data(line);
                string tmp;
                int pos = line.find(":");
                line[pos] = ' ';
                while(data>>tmp) {};
                Process::maxProcs = atoi(tmp.c_str());
            }
        }
        else
        {
            ///linie bez ;
            /*
            Job Number -- a counter field, starting from 1.
            Submit Time -- in seconds. The earliest time the log refers to is zero, and is usually the submittal time of the first job. The lines in the log are sorted by ascending submittal times. It makes sense for jobs to also be numbered in this order.
            Wait Time -- malo przydatne
            Run Time -- in seconds. The wall clock time the job was running (end time minus start time).
            Number of Allocated Processors -- an integer. In most cases this is also the number of processors the job uses; if the job does not use all of them, we typically don't know about it.
            Reszta zbedna
            */
            stringstream stringInput(line); /// nowy strumien, dzieki niemu rodzielony zostaje wczytany string
            string tmp; //zmienna tymczasowa, dane z tej zmiennej trafiaja do struktury Proces
            int i = 0;

            vector<int> temp;
            while (stringInput >> tmp && i < 5) //poki cos jest i to cos nalezy do 1 z 5 informacji wypisanych w komentarzu wyzej, to czytamy
            {
                i++;
                temp.push_back(atoi(tmp.c_str()));
            }
            //Sprawdzanie poprawnosci
            if(temp[0]>=1 && temp[1]>=0 && temp[3]>0 && temp[4]>0)
            {
                //Jezeli proces jest poprawny to go ladujemy na liste procesy
                Process *p = new Process(temp[0], temp[1], temp[3], temp[4]);
                processList.push_back(*p);
                delete p;
            }
            else
            {
                //W innym wypadku pomijamy taka linie i zwiekszamy zmienna blad o 1
                blad++;
            }
            lines--; ///limitowanie ilosci linii do wczytania, wg. zalozen zadania, -1 to wszystkie wpisy
        }
    }
    //Wypisanie informacji nt. pliku wczytywanego
    cout << "Max Jobs:" << Process::maxJobs << "\nMax Procs:" << Process::maxProcs << endl; ///wazne dane
    cout << "Pominiete bledne wpisy: " << blad << endl << endl;
    input.close();

    // funkcja sortuje listę wg. czasu gotowosci procesu
    processList.sort(mysort);

    return processList;
}
