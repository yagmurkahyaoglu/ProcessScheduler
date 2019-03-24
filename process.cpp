//
// Created by casper on 20.11.2018.
//

#include "process.h"

Process::Process(string _name, int _priority, string _fileName, int _arrival, int _order) {
    this->name = _name;
    this->priority = _priority;
    this->fileName = _fileName;
    this->arrival = _arrival;
    this->turnaround = _arrival;
    this->wait = 0;
    this->isDone = false;
    this->where = 1;
    this->order = _order;
}

Process::Process(const Process &_other) {
    this->name = _other.name;
    this->priority = _other.priority;
    this->fileName = _other.fileName;
    this->arrival = _other.arrival;
    this->turnaround = _other.turnaround;
    this->wait = _other.wait;
    this->isDone = _other.isDone;
    this->where = _other.where;
    this->order = _other.order;
}

Process& Process::operator=(const Process &_other) {
    this->name = _other.name;
    this->priority = _other.priority;
    this->fileName = _other.fileName;
    this->arrival = _other.arrival;
    this->turnaround = _other.turnaround;
    this->wait = _other.wait;
    this->isDone = _other.isDone;
    this->where = _other.where;
    this->order = _other.order;
    return *this;
}

Process::~Process() {}