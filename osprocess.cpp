#include "process.h"

process::process(char n, int et, int nb)
{
    name = n;
    entryTime = et;
    timeFinished = -1;
    numBursts = nb;
    waitTime = 0;
    nextUnblock = -1;
    completedBursts = 0;
    nextBlock = -1;
    tTime = 0;
}

int process::getTotalBurst()
{
    int temp = 0;
    
    for (size_t i = 0; i < burstTimes.size(); i++)
    {
        temp += burstTimes[i];
    }
    
    return temp;
}

void process::startio(int t)
{
    nextUnblock = t + ioTimes[completedBursts - 1];
}

void process::startBurst(int t)
{
    nextBlock = t + burstTimes[completedBursts];
}
