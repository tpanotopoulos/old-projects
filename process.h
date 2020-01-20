#include <cmath>
#include <vector>

class process
{
private:
    int tTime;
    int entryTime;
    int timeFinished;
    int numBursts;
    std::vector<int> burstTimes;
    int completedBursts;
    std::vector<int> ioTimes;
    //std::vector<int> tTimes;
    int nextBlock;
    int nextUnblock;
    int waitTime;
    //int qEntry;
    char name;
    
public:
    process(char n, int et, int nb);
    
    int getWaitTime() {return waitTime;}
    int getTTime() {return tTime;}
    int getTotalBurst();
    int getNextUnblock() {return nextUnblock;}
    int getNextBurst() {return burstTimes[completedBursts];}
    int burstsRem() {return numBursts - completedBursts;}
    int getEntry() {return entryTime;}
    void startio(int t);
    void startBurst(int t);
    bool checkio(int t) {return t == nextUnblock;}
    bool checkBurst(int t) {return t == nextBlock;}
    void tTimeInc() {tTime++;}
    void waitTimeInc() {waitTime++;}
    void completeBurstsInc() {completedBursts++;}
    bool isDone() {return completedBursts == numBursts;}
    char getName() {return name;}
    void addBTime(int i) {burstTimes.push_back(i);}
    void addIOTime(int i) {ioTimes.push_back(i);}
};