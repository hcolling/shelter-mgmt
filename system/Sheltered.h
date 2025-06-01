#ifndef SHELTERED_H
#define SHELTERED_H
#include "Person.h"
#include <string>

class Sheltered : public Person {
private:
    std::string responsible;
    bool needResources;
    bool needHealthAssist;

    Sheltered();
    ~Sheltered();
    Sheltered(std::string name, int birthDate[DATES], int gender, std::string cpf, std::string nationality, int bloodType);
    std::string getResponsible();
    void setResponsible(std::string r);

    void requestResources();
    bool isNeedingResources();
    void setNeedResources(bool value);

    void requestHealthAssist();
    bool isNeedingHealthAssist();
    void setNeedHealthAssist(bool value);
};
Sheltered::Sheltered(){}

Sheltered::~Sheltered(){}
Sheltered::Sheltered(std::string name, int birthDate[DATES], int gender, std::string cpf, std::string nationality, int bloodType):Person(name, birthDate, gender, cpf, nationality, bloodType){}

std::string Sheltered::getResponsible() { return responsible; }
void Sheltered::setResponsible(std::string r) {
    if (getAge() < 18) responsible = r;
    else responsible = "N/A";
}

void Sheltered::requestResources() { needResources = true; }
bool Sheltered::isNeedingResources(){ return needResources; }
void Sheltered::setNeedResources(bool value) { needResources = value; }

#endif // SHELTERED_H