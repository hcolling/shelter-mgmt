#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>

using namespace std;

#define MAX_NAME_SZ     100
#define MAX_NATION_SZ   100
#define CPF_SZ          15
#define MAX_USR_SZ      15
#define MAX_PW_SZ       20
#define MAX_DISEASES    100
#define MAX_ALLERGIES   100
#define ADM_RGSTR_FILE  "./adm.txt"
#define RGSTR_FILE      "./shelter.txt"

enum t_gender { MALE, FEMALE, GENDERS };
enum t_bday { DAY, MONTH, YEAR, DATES };
enum t_bloodType { O_NEG, O_POS, A_NEG, A_POS, B_NEG, B_POS, AB_NEG, AB_POS, BLOOD_TYPES };

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
    string getName() const { return name; }
    int getAge() const { const_cast<Person*>(this)->bDateToAge(); return age; }
    int getGender() const { return gender; }
    string getCpf() const { return cpf; }
    string getNationality() const { return nationality; }
    int getBloodType() const { return bloodType; }
    bool getActive() const { return active; }

    void setName(string n) { if (n.length() <= MAX_NAME_SZ) name = n; }
    void setBirthDate(int b[DATES]) { for (int i = 0; i < DATES; i++) birthDate[i] = b[i]; }
    void setGender(int g) { if (g >= 0 && g < GENDERS) gender = g; }
    void setCpf(string c) { cpf = c; }
    void setNationality(string n) { nationality = n; }
    void setBloodType(int b) { bloodType = b; }
    void setActive(bool a) { active = a; }
};

class Sheltered : public Person {
private:
    string responsible;
    bool needResources;
    bool needHealthAssist;

public:
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

// === Validacões ===
bool validarCPF(const string& cpf) {
    regex formato(R"(\d{3}\.\d{3}\.\d{3}-\d{2})");
    return regex_match(cpf, formato);
}
bool validarNome(const string& nome) {
    return !nome.empty() && nome.length() <= MAX_NAME_SZ;
}
bool validarGenero(int g) {
    return g >= 0 && g < GENDERS;
}
bool validarNascimento(int b[DATES]) {
    return (b[DAY] >= 1 && b[DAY] <= 31) &&
           (b[MONTH] >= 1 && b[MONTH] <= 12) &&
           (b[YEAR] > 1900 && b[YEAR] <= 2025);
}
bool validarUsuario(const string& u) {
    return !u.empty() && u.length() <= MAX_USR_SZ;
}
bool validarSenha(const string& s) {
    return s.length() >= 4 && s.length() <= MAX_PW_SZ;
}

// === Utilitarias ===
int converterTipoSanguineo(const string& tipo) {
    if (tipo == "O-") return O_NEG;
    if (tipo == "O+") return O_POS;
    if (tipo == "A-") return A_NEG;
    if (tipo == "A+") return A_POS;
    if (tipo == "B-") return B_NEG;
    if (tipo == "B+") return B_POS;
    if (tipo == "AB-") return AB_NEG;
    if (tipo == "AB+") return AB_POS;
    return -1;
}

string tipoSanguineoParaTexto(int tipo) {
    switch (tipo) {
        case O_NEG: return "O-";
        case O_POS: return "O+";
        case A_NEG: return "A-";
        case A_POS: return "A+";
        case B_NEG: return "B-";
        case B_POS: return "B+";
        case AB_NEG: return "AB-";
        case AB_POS: return "AB+";
        default: return "Desconhecido";
    }
}

bool stringMatch(const string& a, const string& b) {
    string la = a, lb = b;
    transform(la.begin(), la.end(), la.begin(), ::tolower);
    transform(lb.begin(), lb.end(), lb.begin(), ::tolower);
    return la.find(lb) != string::npos;
}

// === Salvar ===
void salvarAdmCSV(const Adm& adm) {
    ofstream file(ADM_RGSTR_FILE, ios::app);
    file << adm.getName() << ","
         << adm.getCpf() << ","
         << adm.getAge() << ","
         << (adm.getGender() == 0 ? "M" : "F") << ","
         << adm.getNationality() << ","
         << tipoSanguineoParaTexto(adm.getBloodType()) << ","
         << adm.getUsername() << ","
         << adm.getPassword() << ","
         << adm.getActive() << endl;
    file.close();
}

void salvarShelteredCSV(const Sheltered& s) {
    ofstream file(RGSTR_FILE, ios::app);
    file << s.getName() << ","
         << s.getCpf() << ","
         << s.getAge() << ","
         << s.getResponsible() << ","
         << (s.getGender() == 0 ? "M" : "F") << ","
         << s.getNationality() << ","
         << tipoSanguineoParaTexto(s.getBloodType()) << ","
         << s.getActive() << endl;
    file.close();
}

// === Leitura ===
vector<Adm> carregarAdmsCSV() {
    vector<Adm> lista;
    ifstream file(ADM_RGSTR_FILE);
    string linha;
    while (getline(file, linha)) {
        stringstream ss(linha);
        string nome, cpf, usuario, senha, generoStr, nacionalidade, btStr, activeStr, idadeStr;
        getline(ss, nome, ','); getline(ss, cpf, ','); getline(ss, idadeStr, ',');
        getline(ss, generoStr, ','); getline(ss, nacionalidade, ','); getline(ss, btStr, ',');
        getline(ss, usuario, ','); getline(ss, senha, ','); getline(ss, activeStr, ','); 

        Adm adm;
        adm.setName(nome); adm.setCpf(cpf);
        adm.setGender(stoi(generoStr));
        adm.setNationality(nacionalidade);
        adm.setBloodType(converterTipoSanguineo(btStr));
        adm.setUsername(usuario); adm.setPassword(senha);
        adm.setActive(stoi(activeStr) == 1);
        lista.push_back(adm);
    }
    return lista;
}

vector<Sheltered> carregarShelteredCSV() {
    vector<Sheltered> lista;
    ifstream file(RGSTR_FILE);
    string linha;
    while (getline(file, linha)) {
        stringstream ss(linha);
        string nome, cpf, idadeStr, generoStr, nacionalidade, responsavel, btStr, activeStr;

        getline(ss, nome, ','); getline(ss, cpf, ','); getline(ss, idadeStr, ',');
        getline(ss, responsavel, ','); getline(ss, generoStr, ','); getline(ss, nacionalidade, ',');
        getline(ss, btStr, ','); getline(ss, activeStr, ',');

        Sheltered s;
        s.setName(nome); s.setCpf(cpf);
        s.setResponsible(responsavel);
        s.setGender(generoStr == "M" ? 0 : 1);
        s.setNationality(nacionalidade);
        s.setBloodType(converterTipoSanguineo(btStr));
        s.setActive(stoi(activeStr) == 1);
        lista.push_back(s);
    }
    return lista;
}

// === Busca ===
void buscarAdmPorNome(const vector<Adm>& adms, const string& nome) {
    bool encontrou = false;
    for (const auto& adm : adms) {
        string nomeAdm = adm.getName();
        string nomeBusca = nome;

        // converter para minúsculas
        transform(nomeAdm.begin(), nomeAdm.end(), nomeAdm.begin(), ::tolower);
        transform(nomeBusca.begin(), nomeBusca.end(), nomeBusca.begin(), ::tolower);

        if (nomeAdm.find(nomeBusca) != string::npos) {
            cout << "Nome: " << adm.getName() << "\nCPF: " << adm.getCpf()
                 << "\nUsuario: " << adm.getUsername()
                 << "\nAtivo: " << (adm.getActive() ? "Sim" : "Ausente") << endl;
            encontrou = true;
        }
    }
    if (!encontrou) cout << "Nenhum administrador encontrado com esse nome.\n";
}

void buscarAdmPorCpf(const vector<Adm>& adms, const string& cpf) {
    for (const auto& adm : adms) {
        if (adm.getCpf() == cpf) {
            cout << "Nome: " << adm.getName() << "\nCPF: " << adm.getCpf()
                 << "\nUsuario: " << adm.getUsername()
                 << "\nAtivo: " << (adm.getActive() ? "Sim" : "Ausente") << endl;
            return;
        }
    }
    cout << "Administrador nao encontrado com esse CPF.\n";
}

void buscarAbrigadoPorNome(const vector<Sheltered>& abr, const string& nome) {
    bool encontrou = false;
    for (const auto& s : abr) {
        string nomeShel = s.getName();
        string nomeBusca = nome;

        // converter para minúsculas
        transform(nomeShel.begin(), nomeShel.end(), nomeShel.begin(), ::tolower);
        transform(nomeBusca.begin(), nomeBusca.end(), nomeBusca.begin(), ::tolower);

        if (nomeShel.find(nomeBusca) != string::npos) {
            cout << "Nome: " << s.getName() << "\nCPF: " << s.getCpf()
                 << "\nResponsavel: " << s.getResponsible()
                 << "\nAtivo: " << (s.getActive() ? "Sim" : "Ausente") << endl;
            encontrou = true;
        }
    }
    if (!encontrou) cout << "Nenhum abrigado encontrado com esse nome.\n";
}

void buscarAbrigadoPorCpf(const vector<Sheltered>& abr, const string& cpf) {
    for (const auto& s : abr) {
        if (s.getCpf() == cpf) {
            cout << "Nome: " << s.getName() << "\nCPF: " << s.getCpf()
                 << "\nResponsavel: " << s.getResponsible()
                 << "\nAtivo: " << (s.getActive() ? "Sim" : "Ausente") << endl;
            return;
        }
    }
    cout << "Abrigado nao encontrado com esse CPF.\n";
}

void buscarAbrigadoPorRecurso(const vector<Sheltered>& abr) {
    bool encontrou = false;
    for (const auto& s : abr) {
        if (s.isNeedingResources()) {
            cout << "Nome: " << s.getName() << "\nCPF: " << s.getCpf()
                 << "\nPrecisa de recursos\n";
            encontrou = true;
        }
    }
    if (!encontrou) cout << "Nenhum abrigado precisa de recursos no momento.\n";
}

void buscarAbrigadoPorSaude(const vector<Sheltered>& abr) {
    bool encontrou = false;
    for (const auto& s : abr) {
        if (s.isNeedingHealthAssist()) {
            cout << "Nome: " << s.getName() << "\nCPF: " << s.getCpf()
                 << "\nPrecisa de assistencia medica\n";
            encontrou = true;
        }
    }
    if (!encontrou) cout << "Nenhum abrigado precisa de assistencia medica no momento.\n";
}

// === Edicao ===
void editarCadastro(bool isAdm) {
    string cpfBusca;
    cout << "CPF do cadastro a editar: ";
    getline(cin, cpfBusca);

    if (isAdm) {
        vector<Adm> lista = carregarAdmsCSV();
        bool encontrado = false;
        for (auto& adm : lista) {
            if (adm.getCpf() == cpfBusca) {
                encontrado = true;
                cout << "\nCadastro encontrado: " << adm.getName() << endl;

                string entrada;
                cout << "Editar nome atual (" << adm.getName() << ")? [s/n]: "; getline(cin, entrada);
                if (entrada == "s") {
                    cout << "Novo nome: "; getline(cin, entrada);
                    adm.setName(entrada);
                }

                cout << "Editar usuario atual (" << adm.getUsername() << ")? [s/n]: "; getline(cin, entrada);
                if (entrada == "s") {
                    cout << "Novo usuario: "; getline(cin, entrada);
                    adm.setUsername(entrada);
                }

                cout << "Editar nacionalidade atual (" << adm.getNationality() << ")? [s/n]: "; getline(cin, entrada);
                if (entrada == "s") {
                    cout << "Nova nacionalidade: "; getline(cin, entrada);
                    adm.setNationality(entrada);
                }

                cout << "Editar ativo (" << (adm.getActive() ? "Sim" : "Nao") << ")? [s/n]: "; getline(cin, entrada);
                if (entrada == "s") {
                    int ativo;
                    while (true) {
                        cout << "Novo status ativo (1=Sim, 0=Ausente): ";
                        cin >> ativo;
                        cin.ignore();
                        if (ativo == 0 || ativo == 1) break;
                        cout << "Entrada invalida! Digite 1 ou 0.\n";
                    }
                    adm.setActive(ativo == 1);
                }

                break;
            }
        }
        if (!encontrado) cout << "Administrador nao encontrado.\n";
        else {
            ofstream file(ADM_RGSTR_FILE);
            for (const auto& adm : lista) {
                file << adm.getName() << "," << adm.getCpf() << ","
                     << adm.getUsername() << "," << adm.getPassword() << ","
                     << adm.getGender() << "," << adm.getNationality() << ","
                     << tipoSanguineoParaTexto(adm.getBloodType()) << ","
                     << adm.getActive() << "," << adm.getAge() << endl;
            }
            file.close();
            cout << "Cadastro atualizado com sucesso.\n";
        }

    } else {
        vector<Sheltered> lista = carregarShelteredCSV();
        bool encontrado = false;
        for (auto& s : lista) {
            if (s.getCpf() == cpfBusca) {
                encontrado = true;
                cout << "\nCadastro encontrado: " << s.getName() << endl;

                string entrada;
                cout << "Editar nome atual (" << s.getName() << ")? [s/n]: "; getline(cin, entrada);
                if (entrada == "s") {
                    cout << "Novo nome: "; getline(cin, entrada);
                    s.setName(entrada);
                }

                cout << "Editar nacionalidade atual (" << s.getNationality() << ")? [s/n]: "; getline(cin, entrada);
                if (entrada == "s") {
                    cout << "Nova nacionalidade: "; getline(cin, entrada);
                    s.setNationality(entrada);
                }

                cout << "Editar ativo (" << (s.getActive() ? "Sim" : "Nao") << ")? [s/n]: "; getline(cin, entrada);
                if (entrada == "s") {
                    int ativo;
                    while (true) {
                        cout << "Novo status ativo (1=Sim, 0=Ausente): ";
                        cin >> ativo;
                        cin.ignore();
                        if (ativo == 0 || ativo == 1) break;
                        cout << "Entrada invalida! Digite 1 ou 0.\n";
                    }
                    s.setActive(ativo == 1);
                }
                break;
            }
        }
        if (!encontrado) cout << "Abrigado nao encontrado.\n";
        else {
            ofstream file(RGSTR_FILE);
            for (const auto& s : lista) {
                file << s.getName() << "," << s.getCpf() << ","
                     << s.getAge() << "," << (s.getGender() == 0 ? "M" : "F") << ","
                     << s.getNationality() << "," << s.getResponsible() << ","
                     << tipoSanguineoParaTexto(s.getBloodType()) << ","
                     << s.getActive() << endl;
            }
            file.close();
            cout << "Cadastro atualizado com sucesso.\n";
        }
    }
}

// === Exclusao ===
void excluirCadastro(bool isAdm) {
    string cpfBusca;
    cout << "CPF do cadastro a excluir: ";
    getline(cin, cpfBusca);

    if (isAdm) {
        vector<Adm> lista = carregarAdmsCSV();
        bool removido = false;
        vector<Adm> novaLista;
        for (const auto& adm : lista) {
            if (adm.getCpf() == cpfBusca) {
                removido = true;
                continue;
            }
            novaLista.push_back(adm);
        }
        if (!removido) cout << "Administrador nao encontrado.\n";
        else {
            ofstream file(ADM_RGSTR_FILE);
            for (const auto& adm : novaLista) {
                file << adm.getName() << "," << adm.getCpf() << ","
                     << adm.getUsername() << "," << adm.getPassword() << ","
                     << adm.getGender() << "," << adm.getNationality() << ","
                     << tipoSanguineoParaTexto(adm.getBloodType()) << ","
                     << adm.getActive() << "," << adm.getAge() << endl;
            }
            file.close();
            cout << "Cadastro excluido com sucesso.\n";
        }

    } else {
        vector<Sheltered> lista = carregarShelteredCSV();
        bool removido = false;
        vector<Sheltered> novaLista;
        for (const auto& s : lista) {
            if (s.getCpf() == cpfBusca) {
                removido = true;
                continue;
            }
            novaLista.push_back(s);
        }
        if (!removido) cout << "Abrigado nao encontrado.\n";
        else {
            ofstream file(RGSTR_FILE);
            for (const auto& s : novaLista) {
                file << s.getName() << "," << s.getCpf() << ","
                     << s.getAge() << "," << (s.getGender() == 0 ? "M" : "F") << ","
                     << s.getNationality() << "," << s.getResponsible() << ","
                     << tipoSanguineoParaTexto(s.getBloodType()) << ","
                     << s.getActive() << endl;
            }
            file.close();
            cout << "Cadastro excluido com sucesso.\n";
        }
    }
}

// === Menu ===
void exibirCSV(const string& caminho) {
    ifstream file(caminho);
    string linha;
    if (!file) {
        cout << "Arquivo nao encontrado.\n";
        return;
    }
    while (getline(file, linha)) cout << linha << endl;
    file.close();
}

void cadastrarNovoAdm() {
    Adm adm;
    string nome, cpf, usuario, senha, nacionalidade, tipo;
    int genero, bday[DATES], sangue, ativo;

    // Nome
    while (true) {
        cout << "Nome: ";
        getline(cin, nome);
        if (validarNome(nome)) break;
        cout << "Nome invalido! Tente novamente.\n";
    }
    adm.setName(nome);

    // CPF
    while (true) {
        cout << "CPF (###.###.###-##): ";
        getline(cin, cpf);
        if (validarCPF(cpf)) break;
        cout << "CPF invalido! Tente novamente.\n";
    }
    adm.setCpf(cpf);

    // Usuario
    while (true) {
        cout << "Usuario: ";
        getline(cin, usuario);
        if (validarUsuario(usuario)) break;
        cout << "Usuario invalido! Tente novamente.\n";
    }
    adm.setUsername(usuario);

    // Senha
    while (true) {
        cout << "Senha: ";
        getline(cin, senha);
        if (validarSenha(senha)) break;
        cout << "Senha invalida! Tente novamente.\n";
    }
    adm.setPassword(senha);

    // Data de nascimento
    while (true) {
        cout << "Data de nascimento (D M A): ";
        for (int i = 0; i < DATES; i++) cin >> bday[i];
        cin.ignore();
        if (validarNascimento(bday)) break;
        cout << "Data invalida! Tente novamente.\n";
    }
    adm.setBirthDate(bday);

    // Genero
    while (true) {
        cout << "Genero (0=Masculino, 1=Feminino): ";
        cin >> genero;
        cin.ignore();
        if (validarGenero(genero)) break;
        cout << "Genero invalido! Tente novamente.\n";
    }
    adm.setGender(genero);

    // Nacionalidade
    cout << "Nacionalidade: ";
    getline(cin, nacionalidade);
    adm.setNationality(nacionalidade);

    // Tipo sanguineo
    while (true) {
        cout << "Tipo sanguineo (ex: O-, AB+): ";
        getline(cin, tipo);
        sangue = converterTipoSanguineo(tipo);
        if (sangue != -1) break;
        cout << "Tipo sanguineo invalido! Tente novamente.\n";
    }
    adm.setBloodType(sangue);

    // Ativo
    while (true) {
        cout << "Ativo no abrigo? (1=Sim, 0=Ausente): ";
        cin >> ativo;
        cin.ignore();
        if (ativo == 0 || ativo == 1) break;
        cout << "Entrada invalida! Digite 1 ou 0.\n";
    }
    adm.setActive(ativo == 1);

    salvarAdmCSV(adm);
    cout << "Administrador cadastrado com sucesso!\n";
}

void cadastrarNovoAbr() {
    Sheltered s;
    string nome, cpf, nacionalidade, responsavel, tipo;
    int genero, bday[DATES], sangue, ativo;

    // Nome
    while (true) {
        cout << "Nome: ";
        getline(cin, nome);
        if (validarNome(nome)) break;
        cout << "Nome invalido! Tente novamente.\n";
    }
    s.setName(nome);

    // CPF
    while (true) {
        cout << "CPF (###.###.###-##): ";
        getline(cin, cpf);
        if (validarCPF(cpf)) break;
        cout << "CPF invalido! Tente novamente.\n";
    }
    s.setCpf(cpf);

    // Data de nascimento
    while (true) {
        cout << "Data de nascimento (D M A): ";
        for (int i = 0; i < DATES; i++) cin >> bday[i];
        cin.ignore();
        if (validarNascimento(bday)) break;
        cout << "Data invalida! Tente novamente.\n";
    }
    s.setBirthDate(bday);

    // Genero
    while (true) {
        cout << "Genero (0=Masculino, 1=Feminino): ";
        cin >> genero;
        cin.ignore();
        if (validarGenero(genero)) break;
        cout << "Genero invalido! Tente novamente.\n";
    }
    s.setGender(genero);

    // Nacionalidade
    cout << "Nacionalidade: ";
    getline(cin, nacionalidade);
    s.setNationality(nacionalidade);

    // Responsavel (se menor)
    if (s.getAge() < 18) {
        cout << "Responsavel: ";
        getline(cin, responsavel);
        s.setResponsible(responsavel);
    } else {
        s.setResponsible("N/A");
    }

    // Tipo sanguineo
    while (true) {
        cout << "Tipo sanguineo (ex: O-, AB+): ";
        getline(cin, tipo);
        sangue = converterTipoSanguineo(tipo);
        if (sangue != -1) break;
        cout << "Tipo sanguineo invalido! Tente novamente.\n";
    }
    s.setBloodType(sangue);

    // Ativo
    while (true) {
        cout << "Ativo no abrigo? (1=Sim, 0=Ausente): ";
        cin >> ativo;
        cin.ignore();
        if (ativo == 0 || ativo == 1) break;
        cout << "Entrada invalida! Digite 1 ou 0.\n";
    }
    s.setActive(ativo == 1);

    salvarShelteredCSV(s);
    cout << "Abrigado cadastrado com sucesso!\n";
}

void exibirCadastrados() {
    int opcao;
    cout << "Deseja ver:\n1. Abrigados\n2. Administradores\n> ";
    cin >> opcao; cin.ignore();
    if (opcao == 1) exibirCSV(RGSTR_FILE);
    else if (opcao == 2) exibirCSV(ADM_RGSTR_FILE);
    else cout << "Opcao invalida.\n";
}

void menuBusca() {
    int tipo;
    cout << "\n== Buscar cadastros ==\n";
    cout << "1. Buscar Administrador\n";
    cout << "2. Buscar Abrigado\n> ";
    cin >> tipo; cin.ignore();

    if (tipo == 1) {
        vector<Adm> adms = carregarAdmsCSV();
        int op;
        cout << "Buscar ADM por:\n1. Nome\n2. CPF\n> ";
        cin >> op; cin.ignore();
        string entrada;
        if (op == 1) {
            cout << "Nome: "; getline(cin, entrada);
            buscarAdmPorNome(adms, entrada);
        } else if (op == 2) {
            cout << "CPF: "; getline(cin, entrada);
            buscarAdmPorCpf(adms, entrada);
        }
    } else if (tipo == 2) {
        vector<Sheltered> abr = carregarShelteredCSV();
        int op;
        cout << "Buscar Abrigado por:\n1. Nome\n2. CPF\n3. Necessidade de recurso\n4. Necessidade de assistencia medica\n> ";
        cin >> op; cin.ignore();
        string entrada;
        switch (op) {
            case 1:
                cout << "Nome: "; getline(cin, entrada);
                buscarAbrigadoPorNome(abr, entrada); break;
            case 2:
                cout << "CPF: "; getline(cin, entrada);
                buscarAbrigadoPorCpf(abr, entrada); break;
            case 3:
                buscarAbrigadoPorRecurso(abr); break;
            case 4:
                buscarAbrigadoPorSaude(abr); break;
            default:
                cout << "Opcao invalida!\n";
        }
    } else {
        cout << "Tipo invalido!\n";
    }
}

int main() {
    int opcao;
    do {
        cout << "\n==== MENU PRINCIPAL ====\n";
        cout << "1. Cadastrar novo abrigado\n";
        cout << "2. Cadastrar novo administrador\n";
        cout << "3. Exibir cadastrados\n";
        cout << "4. Buscar cadastro\n";
        cout << "5. Editar cadastro\n";
        cout << "6. Excluir cadastro\n";
        cout << "7. Sair\n> ";
        cin >> opcao;
        cin.ignore();

        switch (opcao) {
            case 1: cadastrarNovoAbr(); break;
            case 2: cadastrarNovoAdm(); break;
            case 3: exibirCadastrados(); break;
            case 4: menuBusca(); break;
            case 5: {
                int tipo;
                cout << "Editar cadastro de:\n1. Administrador\n2. Abrigado\n> ";
                cin >> tipo; cin.ignore();
                editarCadastro(tipo == 1);
                break;
            }
            case 6: {
                int tipo;
                string confirm;
                cout << "Excluir cadastro de:\n1. Administrador\n2. Abrigado\n> ";
                cin >> tipo; cin.ignore();
                cout << "Tem certeza que deseja excluir? (s para confirmar): ";
                getline(cin, confirm);
                if (confirm == "s") excluirCadastro(tipo == 1);
                else cout << "Operacao cancelada.\n";
                break;
            }
            case 7: cout << "Saindo...\n"; break;
            default: cout << "Opcao invalida!\n";
        }
    } while (opcao != 7);

    return 0;
}