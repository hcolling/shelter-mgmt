#ifndef SHELTERED_H
#define SHELTERED_H
#include "Person.h"
#include <string>

using namespace std;

class Sheltered : public Person {
public:
    Sheltered();
    ~Sheltered();
    string getResponsible();
    
    void requestResources();
    bool isNeedingResources();
    void setNeedResources(bool value);
    void setResponsible(string r);

    void requestHealthAssist();
    bool isNeedingHealthAssist();
    void setNeedHealthAssist(bool value);

private:
    string responsible;
    bool needResources;
    bool needHealthAssist;
};

Sheltered::Sheltered(){
    responsible = "";
    needResources = 0;
    needHealthAssist = 0;
}

Sheltered::~Sheltered(){}

string Sheltered::getResponsible() { return responsible; }

void Sheltered::setResponsible(string r) {
    if (getAge() < 18) responsible = r;
    else responsible = "N/A";
}

void Sheltered::requestResources() { needResources = true; }

bool Sheltered::isNeedingResources(){ return needResources; }

void Sheltered::setNeedResources(bool value) { needResources = value; }

void Sheltered::requestHealthAssist() { needHealthAssist = true; }

bool Sheltered::isNeedingHealthAssist() {return needHealthAssist;}

void Sheltered::setNeedHealthAssist(bool value){ needHealthAssist = value; }

#endif // SHELTERED_H