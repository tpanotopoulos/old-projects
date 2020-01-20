#include "machine.h"

bool sorter(process* p1, process* p2)
{
    return p1->getName() < p2->getName();
}

machine::machine(int i, char* args[])
{
    simTime = -1;
    mode = -1;
    tRem = 0;
    rSeed = atoi(args[1]);
    lValue = strtod(args[2], NULL);
    uLimit = atoi(args[3]);
    pNum = atoi(args[4]);
    csTime = atoi(args[5]);
    aValue = strtod(args[6], NULL);
    tSlice = atoi(args[7]);
    rrType = false;
    cSwitches = 0;
    preempts = 0;
    
    if (i > 8)
    {
        std::string temp(args[8]);
        
        if (temp == "BEGINNING")
        {
            rrType = true;
        }
    }
    
    done = false;
    
    reset();
    
    char buffer[1000];
    
    snprintf(buffer, 1000, "time 0ms: Simulator started for FCFS ");
    std::cout << buffer;
    printQueue();
}

void machine::tick()
{
    bool recheck = false;
    bool skipActive = false;
    
    if (pActive.empty())
    {
        if (switching.empty())
        {
            if (pReady.empty())
            {
                recheck = true;
                if (pIO.empty())
                {
                    if (processArrival.empty())
                    {
                        done = true;
                        generateStats();
                        
                        char buffer[1000];
                        
                        snprintf(buffer, 1000, "time %dms: Simulator ended for FCFS ", simTime);
                        std::cout << buffer;
                        printQueue();
                        
                        return;
                    }
                }
            }
        }
    }
    
    simTime++;
    
    if (!switching.empty())
    {
        tRem--;
        
        switching[0]->tTimeInc();
        
        if (tRem == csTime / 2)
        {
            if (switching[0]->isDone())
            {
                pFinished.push_back(switching[0]);
                switching.pop_back();
            }
            else
            {
                pIO.push_back(switching[0]);
                switching.pop_back();
                
                std::sort(pIO.begin(), pIO.end(), sorter);
            }
            
            if (!pReady.empty())
            {
                switching.push_back(pReady[0]);
                pReady.erase(pReady.begin());
            }
            else
            {
                recheck = true;
            }
        }
        else if (tRem == 0)
        {
            if (simTime < 1000)
            {
                char buffer[1000];
                
                snprintf(buffer, 1000, "time %dms: Process %c started using the CPU for %dms burst ", simTime, switching[0]->getName(), switching[0]->getNextBurst());
                std::cout << buffer;
                printQueue();
            }
            
            switching[0]->startBurst(simTime);
            
            pActive.push_back(switching[0]);
            switching.pop_back();
            
            cSwitches++;
            skipActive = true;
        }
    }
    
    if (!skipActive)
    {
        if (!pActive.empty())
        {
            pActive[0]->tTimeInc();
        
            if (pActive[0]->checkBurst(simTime))
            {
                pActive[0]->completeBurstsInc();
                
                if (pActive[0]->isDone())
                {
                    char buffer[1000];
                
                    snprintf(buffer, 1000, "time %dms: Process %c terminated ", simTime, pActive[0]->getName());
                    std::cout << buffer;
                    printQueue();
                }
                else
                {
                    pActive[0]->startio(simTime + csTime / 2);
                    
                    if (simTime < 1000)
                    {
                        char buffer[1000];
                    
                        snprintf(buffer, 1000, "time %dms: Process %c completed a CPU burst; %d bursts to go ", simTime, pActive[0]->getName(), pActive[0]->burstsRem());
                        std::cout << buffer;
                        printQueue();
                    
                        snprintf(buffer, 1000, "time %dms: Process %c switching out of CPU; will block on I/O until time %dms ", simTime, pActive[0]->getName(), pActive[0]->getNextUnblock());
                        std::cout << buffer;
                        printQueue();
                    }
                }
                
                switching.push_back(pActive[0]);
                pActive.pop_back();
                tRem = csTime;
            }
        }
    }
    
    if (!pIO.empty())
    {
        for (size_t i = 0; i < pIO.size(); i++)
        {
            if (pIO[i]->checkio(simTime))
            {
                pReady.push_back(pIO[i]);
                
                if (simTime < 1000)
                {
                    char buffer[1000];
                    
                    snprintf(buffer, 1000, "time %dms: Process %c completed I/O; added to ready queue ", simTime, pIO[i]->getName());
                    std::cout << buffer;
                    printQueue();
                }
                
                pIO.erase(pIO.begin() + i);
                i--;
            }
        }
    }
    
    if (!processArrival.empty())
    {
        for (size_t i = 0; i < processArrival.size(); i++)
        {
            if (processArrival[i]->getEntry() == simTime)
            {
                pReady.push_back(processArrival[i]);
                
                if (simTime < 1000)
                {
                    char buffer[1000];
                    
                    snprintf(buffer, 1000, "time %dms: Process %c arrived; added to ready queue ", simTime, processArrival[i]->getName());
                    std::cout << buffer;
                    printQueue();
                }
                
                processArrival.erase(processArrival.begin() + i);
                i--;
            }
        }
    }
    
    if (!pReady.empty())
    {
        for (size_t i = 0; i < pReady.size(); i++)
        {
            pReady[i]->tTimeInc();
            pReady[i]->waitTimeInc();
        }
    }
    
    if (recheck)
    //This check is to make sure something starts context switching if the ready queue was empty at
    //the start of this tick but had something added to it partway through.
    {
        if (pActive.empty() && switching.empty())
        //This if statement should be redundant if I've done this right, but I'm putting it in anyway
        {
            if (!pReady.empty())
            {
                switching.push_back(pReady[0]);
                pReady.erase(pReady.begin());
                
                tRem = csTime / 2;
            }
        }
    }
}

void machine::reset()
//Note here that if everything is working properly only pFinished should ever actually have something in it
//when this function gets called.
{
    if (!processArrival.empty())
    {
        for (std::vector<process*>::iterator i = processArrival.begin(); i != processArrival.end(); ++i)
        {
            delete *i;
        }
        
        processArrival.clear();
    }
    
    if (!pReady.empty())
    {
        for (std::vector<process*>::iterator i = pReady.begin(); i != pReady.end(); ++i)
        {
            delete *i;
        }
        
        pReady.clear();
    }
    
    if (!pIO.empty())
    {
        for (std::vector<process*>::iterator i = pIO.begin(); i != pIO.end(); ++i)
        {
            delete *i;
        }
        
        pIO.clear();
    }
    
    if (!pFinished.empty())
    {
        for (std::vector<process*>::iterator i = pFinished.begin(); i != pFinished.end(); ++i)
        {
            delete *i;
        }
        
        pFinished.clear();
    }
    
    if (!pActive.empty())
    {
        for (std::vector<process*>::iterator i = pActive.begin(); i != pActive.end(); ++i)
        {
            delete *i;
        }
        
        pActive.clear();
    }
    
    if (!switching.empty())
    {
        for (std::vector<process*>::iterator i = switching.begin(); i != switching.end(); ++i)
        {
            delete *i;
        }
        
        switching.clear();
    }
    
    srand48(rSeed);
    simTime = -1;
    cSwitches = 0;
    preempts = 0;
    mode++;
    done = false;
    
    genProcesses();
}

void machine::genProcesses()
{
    char buffer[1000];
    
    for (int i = 0; i < pNum; i++)
    {
        int aTime = genTime(true);
        double d = drand48();
        d *= 100;
        int nBursts = (int) ceil(d);
        
        process* p = new process('A' + i, aTime, nBursts);
        
        for (int j = 0; j < nBursts; j++)
        {
            int bTime = genTime(false);
            
            p->addBTime(bTime);
            
            if (j < nBursts - 1)
            {
                int iTime = genTime(false);
                p->addIOTime(iTime);
            }
        }
        
        processArrival.push_back(p);
        
        snprintf(buffer, 1000, "Process %c [NEW] (arrival time %dms) %d CPU bursts\n", p->getName(), p->getEntry(), nBursts);
        std::cout << buffer;
    }
}

void machine::printStats(std::ofstream& f)
{
    char buffer[1000];
    
    f << "Algorithm FCFS\n";
    snprintf(buffer, 1000, "-- average CPU burst time: %0.3f ms\n", statsFCFS[0]);
    f << buffer;
    snprintf(buffer, 1000, "-- average wait time: %0.3f ms\n", statsFCFS[1]);
    f << buffer;
    snprintf(buffer, 1000, "-- average turnaround time: %0.3f ms\n", statsFCFS[2]);
    f << buffer;
    snprintf(buffer, 1000, "-- total number of context switches: %d\n", (int) statsFCFS[3]);
    f << buffer;
    snprintf(buffer, 1000, "-- total number of preemptions: %d\n", (int) statsFCFS[4]);
    f << buffer;
}

int machine::genTime(bool fl)
{
    double d = drand48();
    double e = -log(d) / lValue;
    
    while (e > uLimit)
    {
        d = drand48();
        e = -log(d) / lValue;
    }
    
    if (fl)
    {
        return (int) floor(e);
    }
    else
    {
        return (int) ceil(e);
    }
}

void machine::generateStats()
{
    int btime = 0;
    int ttime = 0;
    int wtime = 0;
    
    for (size_t i = 0; i < pFinished.size(); i++)
    {
        btime += pFinished[i]->getTotalBurst();
        ttime += pFinished[i]->getTTime();
        wtime += pFinished[i]->getWaitTime();
    }
    
    statsFCFS[0] = (float) btime / (float) cSwitches;
    statsFCFS[1] = (float) wtime / (float) cSwitches;
    statsFCFS[2] = (float) ttime / (float) cSwitches;
    statsFCFS[3] = (float) cSwitches;
    statsFCFS[4] = (float) preempts;
}

void machine::printQueue()
{
    std::cout << "[Q";
    
    if (pReady.empty())
    {
        std::cout << " <empty>";
    }
    else
    {
        for (size_t i = 0; i < pReady.size(); i++)
        {
            std::cout << " " << pReady[i]->getName();
        }
    }
    
    std::cout << "]\n";
}
