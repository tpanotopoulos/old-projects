#include "process.h"
#include <list>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>

class machine
{
private:
    std::vector<process*> processArrival;
    std::vector<process*> pReady;
    std::vector<process*> pIO;
    std::vector<process*> pFinished;
    std::vector<process*> pActive; //active process, in a vector so it can be empty without breaking
    std::vector<process*> switching; //process in context switch, ''
    int simTime;
    int mode; //scheduling algorithm
    int tRem; //remaining time in a context switch
    float statsFCFS[5];
    float statsSJF[5];
    float statsSRT[5];
    float statsRR[5];
    int rSeed; //seed
    double lValue; //lambda
    int uLimit; //RNG limit
    int pNum; //number of processes
    int csTime; //time to perform context switch
    double aValue; //alpha
    int tSlice; //time slice size
    bool rrType;
    bool done;
    int cSwitches;
    int preempts;
    void sortRList();
    
public:
    machine(int i, char* args[]);
    void tick();
    bool isDone() {return done;}
    void reset();
    void genProcesses();
    void printStats(std::ofstream& f);
    int genTime(bool fl);
    void generateStats();
    void printQueue();
};