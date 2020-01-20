#include <iostream>
#include <list>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include "machine.h"

int main (int argc, char* argv[])
{
    if (argc < 8)
    {
        std::cerr << "ERROR: Insufficient arguments\n";
        
        return EXIT_FAILURE;
    }
    
    machine simulator(argc, argv);
    
    while(!simulator.isDone())
    {
        simulator.tick();
    }
    
    std::ofstream f;
    
    f.open("simout.txt");
    
    simulator.printStats(f);
    
    f.close();
    
    //FCFS
    
    //SJF
    
    //SRT
    
    //RR
    
    return 0;
}