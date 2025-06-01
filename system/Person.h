#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>

#define MAX_NAME_SZ     100
#define MAX_NATION_SZ   100
#define CPF_SZ          15
#define MAX_USR_SZ      15
#define MAX_PW_SZ       20
#define MAX_DISEASES    100
#define MAX_ALLERGIES   100
#define ADM_RGSTR_FILE  "./register/adm.txt"
#define RGSTR_FILE      "./register/shelter.txt"

enum t_gender { MALE, FEMALE, GENDERS };
enum t_bday { DAY, MONTH, YEAR, DATES };
enum t_bloodType { O_NEG, O_POS, A_NEG, A_POS, B_NEG, B_POS, AB_NEG, AB_POS, BLOOD_TYPES };

using namespace std;

class Person {
private:
    string name;
    int age;
    int birthDate[DATES];
    int gender;
    string cpf;
    string nationality;
    int bloodType;
    string diseases[MAX_DISEASES];
    string allergies[MAX_ALLERGIES];
    bool active;

    void bDateToAge() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        int year = 1900 + ltm->tm_year;
        int month = 1 + ltm->tm_mon;
        int day = ltm->tm_mday;
        age = year - birthDate[YEAR];
        if (month < birthDate[MONTH] || (month == birthDate[MONTH] && day < birthDate[DAY])) age--;
    };

public:
    Person();
    ~Person();
    Person(string name, int age,int birthDate[DATES], int gender, string cpf, string nationality, int bloodType);
    string getName();
    int getAge();
    int getGender();
    string getCpf();
    string getNationality();
    int getBloodType();
    bool getActive();

    void setName(string n) { if (n.length() <= MAX_NAME_SZ) name = n; }
    void setBirthDate(int b[DATES]) { for (int i = 0; i < DATES; i++) birthDate[i] = b[i]; }
    void setGender(int g) { if (g >= 0 && g < GENDERS) gender = g; }
    void setCpf(string cpf) { this->cpf = cpf; }
    void setNationality(string n) { nationality = n; }
    void setBloodType(int b) { bloodType = b; }
    void setActive(bool a) { active = a; }
};

Person::Person(){}

Person::~Person(){}

Person(string name, int age,int birthDate[DATES], int gender, string cpf, string nationality, int bloodType){
    this->name = name;
    this->age = age;
    for (int i = 0; i < DATES; i++) this->birthDate[i] = birthDate[i];
    this->gender = gender;
    this->cpf = cpf;
    this->nationality = nationality;
    this->bloodType = bloodType;
    this->active = true;
}
    

string Person::getName() { return name; }

int Person::getAge() { const_cast<Person*>(this)->bDateToAge(); return age; }

int Person::getGender() { return gender; }

string Person::getCpf() { return cpf; }

string Person::getNationality() { return nationality; }

int Person::getBloodType() { return bloodType; }

bool Person::getActive() { return active; }

void Person::setName(string n) { if (n.length() <= MAX_NAME_SZ) name = n; }

void Person::setBirthDate(int b[DATES]) { for (int i = 0; i < DATES; i++) birthDate[i] = b[i]; }

void Person::setGender(int g) { if (g >= 0 && g < GENDERS) gender = g; }

void Person::setCpf(string cpf) { this->cpf = cpf; }

void Person::setNationality(string n) { nationality = n; }

void Person::setBloodType(int b) { bloodType = b; }

void Person::setActive(bool a) { active = a; }

#endif // PERSON_H