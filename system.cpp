/******************
 * INCLUDED FILES *
 ******************/

/* Dependencies: */
#include <iostream>
#include <string>
#include <ctime>

using namespace std;



/********************
 * PRIVATE DEFINES  *
 ********************/

#define MAX_NAME_SZ     100
#define MAX_NATION_SZ   100
#define CPF_SZ          15

#define MAX_USR_SZ  15
#define MAX_PW_SZ   20

#define MAX_DISEASES    100
#define MAX_ALLERGIES   100   

#define ADM_RGSTR_FILE  "./register/adm.txt"
#define RGSTR_FILE      "./register/shelter.txt"



/*****************
 * PRIVATE TYPES *
 *****************/

enum t_gender
{
    MALE,
    FEMALE,
    GENDERS
};

enum t_bday
{
    DAY,
    MONTH,
    YEAR,
    DATES
};

enum t_bloodType
{
    O_NEG,
    O_POS,
    A_NEG,
    A_POS,
    B_NEG,
    B_POS,
    AB_NEG,
    AB_POS,
    BLOOD_TYPES
};



/**********************
 * CLASSES DEFINITION *
 **********************/

/** CLASS Person **/
class Person
{
private:
    /* Personal info */
    string name;
    int age;
    int birthDate[DATES];
    int gender;

    /* Legal info */
    string cpf;
    string nationality;

    /* Health Info */
    int bloodType;
    string diseases[MAX_DISEASES];
    string allergies[MAX_ALLERGIES];

    /* Shelter Info */
    bool active;

    /* Private Methods */
    void bDateToAge()
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
    
        int year = 1900 + ltm->tm_year;
        int month = 1 + ltm->tm_mon;
        int day = ltm->tm_mday;
    
        age = year - birthDate[YEAR];
    
        if (month < birthDate[MONTH] || (month == birthDate[MONTH] && day < birthDate[DAY]))
            age--;
    };

public:
    /* Methods Get */
    string getName();
    int getAge();
    int getGender();
    string getCpf();
    string getNationality();
    int getBloodType();
    string* getDiseases();
    string* getAllergies();
    bool getActive();

    /* Methods Set */
    void setName(string n);
    void setBirthDate(int b[DATES]);
    void setGender(int g);
    void setCpf(string c);
    void setNationality(string n);
    void setBloodType(int b);
    void setDiseases(string d[MAX_DISEASES]);
    void setAllergies(string a[MAX_ALLERGIES]);
    void setActive(bool a);
};

string Person::getName()
{
    return name;
}
int Person::getAge()
{
    bDateToAge();
    return age;
}
int Person::getGender()
{
    return gender;
}
string Person::getCpf()
{
    return cpf;
}
string Person::getNationality()
{
    return nationality;
}
int Person::getBloodType()
{
    return bloodType;
}
string* Person::getDiseases()
{
    return diseases;
}
string* Person::getAllergies()
{
    return allergies;
}
bool Person::getActive()
{
    return active;
}

void Person::setName(string n)
{
    if (n.length() > MAX_NAME_SZ)
        cout << "Name too long!" << endl;
    else
        name = n;
}
void Person::setBirthDate(int b[DATES])
{
    for (int i = 0; i < DATES; i++)
        birthDate[i] = b[i];
}
void Person::setGender(int g)
{
    for(int i = 0; i < GENDERS; i++)
    {
        if(g == i)
        {
            gender = g;
            break;
        }
        else
            cout << "Invalid" << endl;
    }
}
void Person::setCpf(string c)
{
    if(c.length() > CPF_SZ)
        cout << "CPF too long!" << endl;
    else
    {
        for(int i = 0; i < CPF_SZ; i++)
        {
            if((i == 3 || i == 7) && c[i] != '.')
            {
                cout << "Invalid CPF!" << endl;
                return;
            }
            else if(i == 11 && c[i] != '-')
            {
                cout << "Invalid CPF!" << endl;
                return;
            }
            else if(i == 14 && c[i] != '\0')
            {
                cout << "Invalid CPF!" << endl;
                return;
            }
            else if(c[i] < '0' || c[i] > '9')
            {
                cout << "Invalid CPF!" << endl;
                return;
            }
        }
        cpf = c;
    }
}
void Person::setNationality(string n)
{
    if(n.length() > MAX_NATION_SZ)
        cout << "Nationality too long!" << endl;
    else
        nationality = n;
}
void Person::setBloodType(int b)
{
    for(int i = 0; i < BLOOD_TYPES; i++)
    {
        if(b == i)
        {
            bloodType = b;
            break;
        }
        else
            cout << "Invalid" << endl;
    }
}
void Person::setDiseases(string d[MAX_DISEASES])
{
    for(int i = 0; i < MAX_DISEASES; i++)
    {
        if(d[i].length() > 0)
            diseases[i] = d[i];
        else
            break;
    }
}
void Person::setAllergies(string a[MAX_ALLERGIES])
{
    for(int i = 0; i < MAX_ALLERGIES; i++)
    {
        if(a[i].length() > 0)
            allergies[i] = a[i];
        else
            break;
    }
}
void Person::setActive(bool a)
{
    active = a;
}


/** CLASS Sheltered **/
class Sheltered : public Person
{
private:
    bool minor;
    string responsible;
    bool needResources;
    bool needHealthAssist;

    void checkMinor()
    {
        if(getAge() < 18)
            minor = true;
        else
            minor = false;
    };

public:
    /* Methods Get */
    string getResponsible();
    
    /* Methods Set */
    void setResponsible(string r);

    /* Interclasses Methods */
    void requestResources();
    bool isNeedingResources() const;
    void setNeedResources(bool value);
    void requestHealthAssist();
    bool isNeedingHealthAssist() const;
    void setNeedHealthAssist(bool value);
};

string Sheltered::getResponsible()
{
    return responsible;
}
void Sheltered::setResponsible(string r)
{
    if(minor == true)
    {
        if(r.length() > MAX_NAME_SZ)
            cout << "Responsible name too long!" << endl;
        else
            responsible = r;
    }
    else
    {
        responsible = "N/A";
    }
}

void Sheltered::requestResources()
{
    needResources = true;
}
bool Sheltered::isNeedingResources() const
{
    return Sheltered::needResources;
}
void Sheltered::setNeedResources(bool value)
{
    needResources = value;
}
void Sheltered::requestHealthAssist()
{
    needHealthAssist = true;
}
bool Sheltered::isNeedingHealthAssist() const
{
    return Sheltered::needHealthAssist;
}
void Sheltered::setNeedHealthAssist(bool value)
{
    needHealthAssist = value;
}


/** CLASS Adm **/
class Adm : public Person
{
private:
    string username;
    string password;

public:
    /* Methods Get */
    string getUsername();
    string getPassword();
    
    /* Methods Set */
    void setUsername(string u);
    void setPassword(string p);

    /* Interclasses Methods */
    void checkLackResources(Sheltered &s);
    void checkNeedHealthAssist(Sheltered &s);
};

string Adm::getUsername()
{
    return username;
}
string Adm::getPassword()
{
    return password;
}

void Adm::setUsername(string u)
{
    if(u.length() > MAX_USR_SZ)
        cout << "Username too long!" << endl;
    else
        username = u;
}
void Adm::setPassword(string p)
{
    if(p.length() > MAX_PW_SZ)
        cout << "Password too long!" << endl;
    else
        password = p;
}

void Adm::checkLackResources(Sheltered &s)
{
    if (s.isNeedingResources()) {
        // Buy and deliver resources

        s.setNeedResources(false);
    }
}
void Adm::checkNeedHealthAssist(Sheltered &s)
{
    if (s.isNeedingHealthAssist()) {
        // Buy and deliver resources

        s.setNeedHealthAssist(false);
    }
}



/***********************************
 * PROTOTYPES OF PRIVATE FUNCTIONS *
 ***********************************/
int main() {
    

    return 0;
}
