#include "Person.h"

class Sheltered : public Person {
private:
    string responsible;
    bool needResources;
    bool needHealthAssist;

public:
    Sheltered();
    ~Sheltered();
    Sheltered(string name, int age,int birthDate[DATES], int gender, string cpf, string getNationality, int bloodType);
    string getResponsible() const { return responsible; }
    void setResponsible(string r) {
        if (getAge() < 18) responsible = r;
        else responsible = "N/A";
    }

    void requestResources() { needResources = true; }
    bool isNeedingResources() const { return needResources; }
    void setNeedResources(bool value) { needResources = value; }

    void requestHealthAssist() { needHealthAssist = true; }
    bool isNeedingHealthAssist() const { return needHealthAssist; }
    void setNeedHealthAssist(bool value) { needHealthAssist = value; }
};