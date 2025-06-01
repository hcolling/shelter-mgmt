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

class Person {
private:
    std::string name;
    int age;
    int birthDate[DATES];
    int gender;
    std::string cpf;
    std::string nationality;
    int bloodType;
    std::string diseases[MAX_DISEASES];
    std::string allergies[MAX_ALLERGIES];
    bool active;

public:
    Person();
    ~Person();
    Person(std::string name, int birthDate[DATES], int gender, std::string cpf, std::string nationality, int bloodType);
    std::string getName();
    int getAge();
    int getGender();
    std::string getCpf();
    std::string getNationality();
    int getBloodType();
    bool getActive();

    void setName(std::string n) { if (n.length() <= MAX_NAME_SZ) name = n; }
    void setBirthDate(int b[DATES]) { for (int i = 0; i < DATES; i++) birthDate[i] = b[i]; }
    void bDateToAge();
    void setGender(int g) { if (g >= 0 && g < GENDERS) gender = g; }
    void setCpf(std::string cpf) { this->cpf = cpf; }
    void setNationality(std::string n) { nationality = n; }
    void setBloodType(int b) { bloodType = b; }
    void setActive(bool a) { active = a; }
};

Person::Person(){}

Person::~Person(){}

Person::Person(std::string name, int birthDate[DATES], int gender, std::string cpf, std::string nationality, int bloodType){
    this->name = name;
    for (int i = 0; i < DATES; i++) this->birthDate[i] = birthDate[i];
    bDateToAge();
    this->gender = gender;
    this->cpf = cpf;
    this->nationality = nationality;
    this->bloodType = bloodType;
    this->active = true;
}

std::string Person::getName() { return name; }

int Person::getAge() { const_cast<Person*>(this)->bDateToAge(); return age; }

int Person::getGender() { return gender; }

std::string Person::getCpf() { return cpf; }

std::string Person::getNationality() { return nationality; }

int Person::getBloodType() { return bloodType; }

bool Person::getActive() { return active; }

void Person::setName(std::string n) { if (n.length() <= MAX_NAME_SZ) name = n; }

void Person::setBirthDate(int b[DATES]) { for (int i = 0; i < DATES; i++) birthDate[i] = b[i]; }

void Person::bDateToAge() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int year = 1900 + ltm->tm_year;
    int month = 1 + ltm->tm_mon;
    int day = ltm->tm_mday;
    age = year - birthDate[YEAR];
    if (month < birthDate[MONTH] || (month == birthDate[MONTH] && day < birthDate[DAY])) age--;
};

void Person::setGender(int g) { if (g >= 0 && g < GENDERS) gender = g; }

void Person::setCpf(std::string cpf) { this->cpf = cpf; }

void Person::setNationality(std::string n) { nationality = n; }

void Person::setBloodType(int b) { bloodType = b; }

void Person::setActive(bool a) { active = a; }

#endif // PERSON_H