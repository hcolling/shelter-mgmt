#include "Sheltered.h"

class Adm : public Person {
private:
    string username;
    string password;

public:
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    void setUsername(string u) { if (u.length() <= MAX_USR_SZ) username = u; }
    void setPassword(string p) { if (p.length() <= MAX_PW_SZ) password = p; }

    void checkLackResources(Sheltered &s) {
        if (s.isNeedingResources()) s.setNeedResources(false);
    }
    void checkNeedHealthAssist(Sheltered &s) {
        if (s.isNeedingHealthAssist()) s.setNeedHealthAssist(false);
    }
};