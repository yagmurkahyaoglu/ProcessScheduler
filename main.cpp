#include <iostream>
#include "process.h"
#include "instruction.h"
#include <sstream>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <vector>
#include <functional>
#include <queue>
#include <cmath>
#include <stdio.h>



template <class Container>
void split(const string& str, Container& cont) //to split the line into words
{
    istringstream iss(str);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(cont));
}

//for priority queue to insert the processes according to their priorities or coming orders
bool comparePriority(Process* p1, Process* p2){
    if(p1->priority == p2->priority){
        return p1->order > p2->order;
    }
    return p1->priority > p2->priority;
}

void fillInstructions(Process* p){ //fills the instruction queue of given process
    string file = "C:\\Users\\casper\\Desktop\\Cmpe 322\\Project_1\\" + p->fileName + ".txt";
    ifstream infile(file);
    string line;
    while(!infile.eof()){
        getline(infile, line);
        vector<string> words;
        split(line, words);
        Instruction* i = new Instruction(words[0], stoi(words[1]));
        p->instructions.push(i);
        p->wait += i->time;
    }
}

int work(Process *p, int until, int now){ //processes the instructions and returns the time when the process stopped
    int duration = until - now;
    int where = 0;
    Instruction* i;
    while(!p->instructions.empty() && duration > 0){
        i = p->instructions.front();
        now += i->time;
        if(i->name.compare("exit") == 0){
            duration = -1;
            p->isDone = true;
        }
        else {
            duration -= i->time;
        }
        where++;
        p->instructions.pop();
    }
    p->where += where;
    return now;
}

int workNonstop(Process *p, int now){ //processes all of the left instructions of the given process, returns end time
    Instruction* i;
    while(!p->instructions.empty()){
        i = p->instructions.front();
        now += i->time;
        p->instructions.pop();
    }
    return now;
}


int main() {

    string file = "C:\\Users\\casper\\Desktop\\Cmpe 322\\Project_1\\definition8.txt";
    ifstream infile(file);
    string line;
    queue<Process*> jobQueue;
    priority_queue<Process*, vector<Process*>, function<bool(Process*, Process*)>> priorityQueue(comparePriority);
    vector<Process*> temp;
    vector<Process*> forData;

    freopen("C:\\Users\\casper\\Desktop\\Cmpe 322\\Project_1\\output.txt", "w", stdout); // opens output.txt to write

    int lastPrinted = 0;

    int order = 0;
    while(!infile.eof()){ //reads from file to store processes
        getline(infile, line);
        vector<string> words;
        split(line, words);
        order++;
        if(!line.empty()) {
            Process *n = new Process(words[0], stoi(words[1]), words[2], stoi(words[3]), order);
            jobQueue.push(n);
            forData.push_back(n);
        }
    }

    Process* p,* next;
    int now;
    int until;
    string old ="";
    int whereOld = 0;

    cout<< "0:HEAD--TAIL"<<endl;
   // printf("0:HEAD--TAIL\n");

    if(!jobQueue.empty()){ //put first process to ready queue
        p = jobQueue.front();
        jobQueue.pop();
        priorityQueue.push(p);
        now = p->arrival;
    }

    while(!priorityQueue.empty() || !jobQueue.empty()){ //continue if ready queue or wait queue is not empty

        while(!jobQueue.empty() && jobQueue.front()->arrival == now){ //if new processes arrived at now
            next = jobQueue.front();
            priorityQueue.push(next);
            jobQueue.pop();
        }

        p = priorityQueue.top(); //takes the process on the top of the queue to run
        now = fmax(now, p->arrival);

        // prints the ready queue if there is a change after it was printed the last time
        if(lastPrinted != now && ((!jobQueue.empty() && now != jobQueue.front()->arrival) || jobQueue.empty())){
            if((p->name.compare(old) == 0 && p->where != whereOld) || p->where == 1) {
                while(!priorityQueue.empty()){
                    temp.push_back(priorityQueue.top());
                    priorityQueue.pop();
                }
                cout<<now<<":HEAD";
              //  printf("%d:HEAD", now);
                for(int k=0; k<temp.size(); k++){
                    cout<<"-"<<temp[k]->name<<"["<<temp[k]->where<<"]";
                   // printf("-%s[%d]", temp[k]->name, temp[k]->where);
                }
               // printf("-TAIL\n");
                cout<<"-TAIL"<<endl;
                for(int k=0; k<temp.size(); k++){
                    priorityQueue.push(temp[k]);
                }
                temp.clear();
            }
            lastPrinted = now;
        }
        priorityQueue.pop();

        // if the process wil be run the first run it calls the method to fill its instructions
        if(p->instructions.size() == 0){
            fillInstructions(p);
        }

        // if there is a process that may has a priority over the current one
        if(!jobQueue.empty()){
            next = jobQueue.front();
            jobQueue.pop();
            until = next->arrival;
            p->arrival = now;

            now = work(p, until, now); // runs the current process until the new one comes
            if(!p->isDone){
                priorityQueue.push(p); // if the current process is not done, we push it to the queue
            }
            // the arrival time is first held in turnaround so extracting it from "now" will give us the turnaround time
            else {
                p->turnaround = now - p->turnaround;
            }
            // until the next process enters, if there is time it runs the processes currently in the queue
            while(now < until){
                while(!priorityQueue.empty()){
                    temp.push_back(priorityQueue.top());
                    priorityQueue.pop();
                }
                if(lastPrinted != now) { // prints the queue if necessary
                    cout << now << ":HEAD";
                    //printf("%d:HEAD", now);
                    if (temp.size() == 0) {
                        cout << "-";
                       // printf("-");
                    }
                    for (int k = 0; k < temp.size(); k++) {
                        cout << "-" << temp[k]->name << "[" << temp[k]->where << "]";
                       // printf("-%s[%d]", temp[k]->name, temp[k]->where);
                    }
                    cout << "-TAIL" << endl;
                   // printf("-TAIL\n");
                    for (int k = 0; k < temp.size(); k++) {
                        priorityQueue.push(temp[k]);
                    }
                    temp.clear();
                    lastPrinted = now;
                }
                if(priorityQueue.empty()) {
                    break;
                }
                p = priorityQueue.top();
                if(p->instructions.size() == 0){
                    fillInstructions(p);
                }
                p->arrival = now;
                now = work(p, until, now);
                if(p->isDone){
                    priorityQueue.pop();
                    p->turnaround = now - p->turnaround;
                }


            }
            priorityQueue.push(next); // pushes the next process from the wait queue to the ready queue

            while(!jobQueue.empty() && jobQueue.front()->arrival == until){
                next = jobQueue.front();
                priorityQueue.push(next);
                jobQueue.pop();
            }

            // if there are processes that comes at the same time with the "next" process
            if(next->priority >= p->priority || (next->priority == p->priority && now > next->arrival)){
                while(!priorityQueue.empty()){
                    temp.push_back(priorityQueue.top());
                    priorityQueue.pop();
                }
                cout<<now<<":HEAD";
               // printf("%d:HEAD", now);
                for(int k=0; k<temp.size(); k++){
                    cout<<"-"<<temp[k]->name<<"["<<temp[k]->where<<"]";
                   // printf("-%s[%d]", temp[k]->name, temp[k]->where);
                }
                cout<<"-TAIL"<<endl;
              //  printf("-TAIL\n");
                for(int k=0; k<temp.size(); k++){
                    priorityQueue.push(temp[k]);
                }
                temp.clear();
                lastPrinted = now;
            }
        }
        // if the wait queue is empty and there is no process to stop the current process it runs until it finishes
        else {
            p->arrival = now;
            now = workNonstop(p, now);
            p->turnaround = now - p->turnaround;

            if(lastPrinted != now) {
              //  printf("%d:HEAD", now);
                cout << now << ":HEAD";
                if (priorityQueue.empty()) {
                    cout << "-";
                   // printf("-");
                }
                while (!priorityQueue.empty()) {
                    temp.push_back(priorityQueue.top());
                    priorityQueue.pop();
                }

                for (int k = 0; k < temp.size(); k++) {
                    cout << "-" << temp[k]->name << "[" << temp[k]->where << "]";
                   // printf("-%s[%d]", temp[k]->name, temp[k]->where);
                }
                cout << "-TAIL" << endl;
               // printf("-TAIL\n");
                for (int k = 0; k < temp.size(); k++) {
                    priorityQueue.push(temp[k]);
                }
                temp.clear();
                lastPrinted = now;
            }
        }
        old = p->name;
        whereOld = p->where;

    }

    cout<<endl;
 //   printf("\n");

    // total wait time equals to turnaround time - run time
    // the total run time values are held in wait at first so extracting it from turnaround gives us the waiting time
    for(int j=0; j<forData.size(); j++){
        Process* x = forData[j];
        cout<<"Turnaround time for "<<x->name<<"= "<<x->turnaround<<" ms"<<endl;
        //printf("Turnaround time for %s= %d ms", x->name, x->turnaround);
        cout<<"Waiting time for "<<x->name<<"= "<<x->turnaround - x->wait<<" ms"<<endl;
        //printf("Waiting time for %s= %d ms", x->name, (x->turnaround - x->wait));
    }

    fclose(stdout);

    return 0;
}