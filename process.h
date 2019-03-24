//
// Created by casper on 20.11.2018.
//

#ifndef PROJECT_1_PROCESS_H
#define PROJECT_1_PROCESS_H

#include <iostream>
#include <queue>
#include "instruction.h"
using namespace std;

class Process {

public:

    string name;
    int priority;
    string fileName;
    int arrival;
    queue<Instruction *> instructions;
    int turnaround;
    int wait;
    bool isDone;
    int where;
    int order;

    Process(string _name, int _priority, string _fileName, int _arrival, int _order);
    Process(const Process& _other);
    Process& operator=(const Process& _other);
    ~Process();


};

#endif //PROJECT_1_PROCESS_H
