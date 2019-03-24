//
// Created by casper on 20.11.2018.
//

#ifndef PROJECT_1_INSTRUCTION_H
#define PROJECT_1_INSTRUCTION_H

#include <iostream>
using namespace std;

class Instruction {

public:

    string name;
    int time;

    Instruction(string _name, int _time);
    Instruction(const Instruction& _other);
    Instruction& operator=(const Instruction& _other);
    ~Instruction();

};

#endif //PROJECT_1_INSTRUCTION_H
