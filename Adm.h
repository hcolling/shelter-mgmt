#include "Sheltered.h"

using namespace std;

class Adm : public Person {
private:
    string username;
    string password;

public:
    Adm();
    ~Adm();
    string getUsername();
    string getPassword();
    void setUsername(string u);
    void setPassword(string p);

    void checkLackResources(Sheltered *s);
    void checkNeedHealthAssist(Sheltered *s);
};

Adm::Adm(){}

Adm::~Adm(){}

string Adm::getUsername(){ return username; }

string Adm::getPassword(){ return password; }

void Adm::setUsername(string u) { if (u.length() <= MAX_USR_SZ) username = u; }

void Adm::setPassword(string p) { if (p.length() <= MAX_PW_SZ) password = p; }

void Adm::checkLackResources(Sheltered *s) {
    if (s->isNeedingResources()) s->setNeedResources(false);
}

void Adm::checkNeedHealthAssist(Sheltered* s) {
    if (s->isNeedingHealthAssist()) s->setNeedHealthAssist(false);
}