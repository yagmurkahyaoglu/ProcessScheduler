//
// Created by casper on 20.11.2018.
//

#include "instruction.h"

Instruction::Instruction(string _name, int _time) {
    this->name = _name;
    this->time = _time;
}

Instruction::Instruction(const Instruction &_other) {
    this->name = _other.name;
    this->time = _other.time;
}

Instruction& Instruction::operator=(const Instruction &_other) {
    this->name = _other.name;
    this->time = _other.time;
    return *this;
}

Instruction::~Instruction() {}