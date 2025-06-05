#include <ctime>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include "Adm.h"

using namespace std;

class System{
    public:

    // === Validacões ===
    bool validarCPF(const string& cpf);
    bool validarNome(const string& nome);
    bool validarGenero(int g);
    bool validarNascimento(int b[DATES]);
    bool validarUsuario(const string& u);
    bool validarSenha(const string& s);

    // === Utilitarias ===
    int converterTipoSanguineo(const string& tipo);
    string tipoSanguineoParaTexto(int tipo);
    bool stringMatch(const string& a, const string& b);

    // === Salvar ===
    void salvarAdmCSV(Adm& adm);
    void salvarShelteredCSV(Sheltered& s);

    // === Leitura ===
    vector<Adm> carregarAdmsCSV();
    vector<Sheltered> carregarShelteredCSV();

    // === Busca ===
    void buscarAdmPorNome(const vector<Adm>& adms, const string& nome);
    void buscarAdmPorCpf(const vector<Adm>& adms, const string& cpf);
    void buscarAbrigadoPorNome(const vector<Sheltered>& abr, const string& nome);
    void buscarAbrigadoPorCpf(const vector<Sheltered>& abr, const string& cpf);
    void buscarAbrigadoPorRecurso(const vector<Sheltered>& abr);
    void buscarAbrigadoPorSaude(const vector<Sheltered>& abr);

    // === Edicao ===
    void editarCadastro(bool isAdm);

    // === Exclusao ===
    void excluirCadastro(bool isAdm);

    // === Menu ===
    void exibirCSV(const string& caminho);
    void cadastrarNovoAdm();
    void cadastrarNovoAbr();
    void exibirCadastrados();
    void menuBusca();
};

bool System::validarCPF(const string& cpf) {
    regex formato(R"(\d{3}\.\d{3}\.\d{3}-\d{2})");
    return regex_match(cpf, formato);
}

bool System::validarNome(const string& nome) {
    return !nome.empty() && nome.length() <= MAX_NAME_SZ;
}

bool System::validarGenero(int g) {
    return g >= 0 && g < GENDERS;
}

bool System::validarNascimento(int b[DATES]) {
    return (b[DAY] >= 1 && b[DAY] <= 31) &&
        (b[MONTH] >= 1 && b[MONTH] <= 12) &&
        (b[YEAR] > 1900 && b[YEAR] <= 2025);
}

bool System::validarUsuario(const string& u) {
    return !u.empty() && u.length() <= MAX_USR_SZ;
}

bool System::validarSenha(const string& s) {
    return s.length() >= 4 && s.length() <= MAX_PW_SZ;
}

int System::converterTipoSanguineo(const string& tipo) {
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

string System::tipoSanguineoParaTexto(int tipo) {
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

bool System::stringMatch(const string& a, const string& b) {
    string la = a, lb = b;
    transform(la.begin(), la.end(), la.begin(), ::tolower);
    transform(lb.begin(), lb.end(), lb.begin(), ::tolower);
    return la.find(lb) != string::npos;
}

void System::salvarAdmCSV(Adm& adm) {
    ofstream file(ADM_RGSTR_FILE, ios::app);
    file << adm.getName() << ","
         << adm.getCpf() << ","
         << adm.getAge() << ","
         << (adm.getGender() == 0 ? "M" : "F") << ","
         << adm.getNationality() << ","
         << tipoSanguineoParaTexto(adm.getBloodType()) << ","
         << adm.getActive() << ","
         << adm.getUsername() << ","
         << adm.getPassword() << endl;
    file.close();
}

void System::salvarShelteredCSV(Sheltered& s) {
    ofstream file(RGSTR_FILE, ios::app);
    file << s.getName() << ","
         << s.getCpf() << ","
         << s.getAge() << ","
         << (s.getGender() == 0 ? "M" : "F") << ","
         << s.getNationality() << ","
         << s.getResponsible() << ","
         << tipoSanguineoParaTexto(s.getBloodType()) << ","
         << s.getActive() << ","
         << s.isNeedingResources() << ","
         << s.isNeedingHealthAssist() << endl;
    file.close();
}

vector<Adm> System::carregarAdmsCSV() {
    vector<Adm> lista;
    ifstream file(ADM_RGSTR_FILE);
    string linha;

    while (getline(file, linha)) {
        stringstream ss(linha);
        string nome, cpf, idadeStr, generoStr, nacionalidade, btStr, ativoStr, usuario, senha;

        getline(ss, nome, ',');
        getline(ss, cpf, ',');
        getline(ss, idadeStr, ',');
        getline(ss, generoStr, ',');
        getline(ss, nacionalidade, ',');
        getline(ss, btStr, ',');
        getline(ss, ativoStr, ',');
        getline(ss, usuario, ',');
        getline(ss, senha, ',');

        Adm adm;
        adm.setName(nome);
        adm.setCpf(cpf);
        adm.setGender(generoStr == "M" ? 0 : 1);
        adm.setNationality(nacionalidade);
        adm.setBloodType(converterTipoSanguineo(btStr));
        adm.setActive(stoi(ativoStr) == 1);
        adm.setUsername(usuario);
        adm.setPassword(senha);

        lista.push_back(adm);
    }
    return lista;
}

vector<Sheltered> System::carregarShelteredCSV() {
    vector<Sheltered> lista;
    ifstream file(RGSTR_FILE);
    string linha;

    while (getline(file, linha)) {
        stringstream ss(linha);
        string nome, cpf, idadeStr, generoStr, nacionalidade, responsavel;
        string btStr, ativoStr, recursosStr, saudeStr;

        getline(ss, nome, ',');
        getline(ss, cpf, ',');
        getline(ss, idadeStr, ',');
        getline(ss, generoStr, ',');
        getline(ss, nacionalidade, ',');
        getline(ss, responsavel, ',');
        getline(ss, btStr, ',');
        getline(ss, ativoStr, ',');
        getline(ss, recursosStr, ',');
        getline(ss, saudeStr, ',');

        Sheltered s;
        s.setName(nome);
        s.setCpf(cpf);
        s.setGender(generoStr == "M" ? 0 : 1);
        s.setNationality(nacionalidade);
        s.setResponsible(responsavel);
        s.setBloodType(converterTipoSanguineo(btStr));
        s.setActive(stoi(ativoStr) == 1);
        s.setNeedResources(stoi(recursosStr) == 1);
        s.setNeedHealthAssist(stoi(saudeStr) == 1);

        lista.push_back(s);
    }
    return lista;
}

void System::buscarAdmPorNome(const vector<Adm>& adms, const string& nome) {
    bool encontrou = false;
    for (auto adm : adms) {
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

void System::buscarAdmPorCpf(const vector<Adm>& adms, const string& cpf) {
    for (auto adm : adms) {
        if (adm.getCpf() == cpf) {
            cout << "Nome: " << adm.getName() << "\nCPF: " << adm.getCpf()
                << "\nUsuario: " << adm.getUsername()
                << "\nAtivo: " << (adm.getActive() ? "Sim" : "Ausente") << endl;
            return;
        }
    }
    cout << "Administrador nao encontrado com esse CPF.\n";
}

void System::buscarAbrigadoPorNome(const vector<Sheltered>& abr, const string& nome) {
    bool encontrou = false;
    for (auto s : abr) {
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

void System::buscarAbrigadoPorCpf(const vector<Sheltered>& abr, const string& cpf) {
    for (auto s : abr) {
        if (s.getCpf() == cpf) {
            cout << "Nome: " << s.getName() << "\nCPF: " << s.getCpf()
                << "\nResponsavel: " << s.getResponsible()
                << "\nAtivo: " << (s.getActive() ? "Sim" : "Ausente") << endl;
            return;
        }
    }
    cout << "Abrigado nao encontrado com esse CPF.\n";
}

void System::buscarAbrigadoPorRecurso(const vector<Sheltered>& abr) {
    bool encontrou = false;
    for (auto s : abr) {
        if (s.isNeedingResources()) {
            cout << "Nome: " << s.getName() << "\nCPF: " << s.getCpf()
                << "\nPrecisa de recursos\n";
            encontrou = true;
        }
    }
    if (!encontrou) cout << "Nenhum abrigado precisa de recursos no momento.\n";
}

void System::buscarAbrigadoPorSaude(const vector<Sheltered>& abr) {
    bool encontrou = false;
    for (auto s : abr) {
        if (s.isNeedingHealthAssist()) {
            cout << "Nome: " << s.getName() << "\nCPF: " << s.getCpf()
                << "\nPrecisa de assistencia medica\n";
            encontrou = true;
        }
    }
    if (!encontrou) cout << "Nenhum abrigado precisa de assistencia medica no momento.\n";
}

void System::editarCadastro(bool isAdm) {
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
            for (auto adm : lista) {
                file << adm.getName() << "," << adm.getCpf() << "," << adm.getAge() << ","
                    << (adm.getGender() == 0 ? "M" : "F") << "," << adm.getNationality() << ","
                    << tipoSanguineoParaTexto(adm.getBloodType()) << "," << adm.getActive() << ","
                    << adm.getUsername() << "," << adm.getPassword() << endl;
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

                // Editar necessidade de recursos
                cout << "Editar necessidade de recursos (" << (s.isNeedingResources() ? "1" : "0") << ")? [s/n]: ";
                getline(cin, entrada);
                if (entrada == "s") {
                    int r;
                    while (true) {
                        cout << "Nova necessidade de recursos (1=Sim, 0=Nao): ";
                        cin >> r; cin.ignore();
                        if (r == 0 || r == 1) break;
                        cout << "Entrada invalida! Digite 1 ou 0.\n";
                    }
                    s.setNeedResources(r == 1);
                }

                // Editar necessidade de assistencia medica
                cout << "Editar necessidade de assistencia medica (" << (s.isNeedingHealthAssist() ? "1" : "0") << ")? [s/n]: ";
                getline(cin, entrada);
                if (entrada == "s") {
                    int m;
                    while (true) {
                        cout << "Nova necessidade de assistencia medica (1=Sim, 0=Nao): ";
                        cin >> m; cin.ignore();
                        if (m == 0 || m == 1) break;
                        cout << "Entrada invalida! Digite 1 ou 0.\n";
                    }
                    s.setNeedHealthAssist(m == 1);
                }

                break;
            }
        }
        if (!encontrado) cout << "Abrigado nao encontrado.\n";
        else {
            ofstream file(RGSTR_FILE);
            for (auto s : lista) {
                file << s.getName() << "," << s.getCpf() << "," << s.getAge() << ","
                    << (s.getGender() == 0 ? "M" : "F") << "," << s.getNationality() << ","
                    << s.getResponsible() << "," << tipoSanguineoParaTexto(s.getBloodType()) << ","
                    << s.getActive() << "," << s.isNeedingResources() << "," << s.isNeedingHealthAssist() << endl;
            }
            file.close();
            cout << "Cadastro atualizado com sucesso.\n";
        }
    }
}

void System::excluirCadastro(bool isAdm) {
    string cpfBusca;
    cout << "CPF do cadastro a excluir: ";
    getline(cin, cpfBusca);

    if (isAdm) {
        vector<Adm> lista = carregarAdmsCSV();
        bool removido = false;
        vector<Adm> novaLista;
        for (auto adm : lista) {
            if (adm.getCpf() == cpfBusca) {
                removido = true;
                continue;
            }
            novaLista.push_back(adm);
        }
        if (!removido) cout << "Administrador nao encontrado.\n";
        else {
            ofstream file(ADM_RGSTR_FILE);
            for (auto adm : novaLista) {
                file << adm.getName() << "," << adm.getCpf() << "," << adm.getAge() << ","
                    << (adm.getGender() == 0 ? "M" : "F") << "," << adm.getNationality() << ","
                    << tipoSanguineoParaTexto(adm.getBloodType()) << "," << adm.getActive() << ","
                    << adm.getUsername() << "," << adm.getPassword() << endl;
            }
            file.close();
            cout << "Cadastro excluido com sucesso.\n";
        }

    } else {
        vector<Sheltered> lista = carregarShelteredCSV();
        bool removido = false;
        vector<Sheltered> novaLista;
        for (auto s : lista) {
            if (s.getCpf() == cpfBusca) {
                removido = true;
                continue;
            }
            novaLista.push_back(s);
        }
        if (!removido) cout << "Abrigado nao encontrado.\n";
        else {
            ofstream file(RGSTR_FILE);
            for (auto s : novaLista) {
                file << s.getName() << "," << s.getCpf() << "," << s.getAge() << ","
                    << (s.getGender() == 0 ? "M" : "F") << "," << s.getNationality() << ","
                    << s.getResponsible() << "," << tipoSanguineoParaTexto(s.getBloodType()) << ","
                    << s.getActive() << "," << s.isNeedingResources() << "," << s.isNeedingHealthAssist() << endl;
            }
            file.close();
            cout << "Cadastro excluido com sucesso.\n";
        }
    }
}

void System::exibirCSV(const string& caminho) {
    ifstream file(caminho);
    string linha;
    if (!file) {
        cout << "Nenhum cadastro encontrado.\n";
        return;
    }
    bool temConteudo = false;
    while (getline(file, linha)) {
        // Ignora linhas vazias ou só com espaços/tabs
        string temp = linha;
        temp.erase(remove_if(temp.begin(), temp.end(), ::isspace), temp.end());
        if (!temp.empty()) {
            cout << linha << endl;
            temConteudo = true;
        }
    }
    if (!temConteudo) {
        cout << "Nenhum cadastro encontrado.\n";
    }
    file.close();
}

void System::cadastrarNovoAdm() {
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
        cout << "Senha (maior que 4 caracteres): ";
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
        transform(tipo.begin(), tipo.end(), tipo.begin(), ::toupper);
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

void System::cadastrarNovoAbr() {
    Sheltered s;
    string nome, cpf, nacionalidade, responsavel, tipo;
    int genero, bday[DATES], sangue, ativo, recursos, saude;

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

        // Responsavel (se menor)
    if (s.getAge() < 18) {
        cout << "Responsavel: ";
        getline(cin, responsavel);
        s.setResponsible(responsavel);
    } else {
        s.setResponsible("N/A");
    }

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

    // Tipo sanguineo
    while (true) {
        cout << "Tipo sanguineo (ex: O-, AB+): ";
        getline(cin, tipo);
        transform(tipo.begin(), tipo.end(), tipo.begin(), ::toupper);
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

    // Necessita recursos
    while (true) {
        cout << "Precisa de recursos? (1=Sim, 0=Não): ";
        cin >> recursos;
        if (recursos == 0 || recursos == 1) break;
        cout << "Entrada inválida.\n";
    }
    s.setNeedResources(recursos == 1);

    // Necessita assistência
    while (true) {
        cout << "Precisa de assistência médica? (1=Sim, 0=Não): ";
        cin >> saude;
        if (saude == 0 || saude == 1) break;
        cout << "Entrada inválida.\n";
    }
    s.setNeedHealthAssist(saude == 1);
    cin.ignore();

    salvarShelteredCSV(s);
    cout << "Abrigado cadastrado com sucesso!\n";
}

void System::exibirCadastrados() {
    int opcao;
    cout << "Deseja ver:\n1. Abrigados\n2. Administradores\n> ";
    cin >> opcao; cin.ignore();
    if (opcao == 1) exibirCSV(RGSTR_FILE);
    else if (opcao == 2) exibirCSV(ADM_RGSTR_FILE);
    else cout << "Opcao invalida.\n";
}

void System::menuBusca() {
    int tipo;
    cout << "\n== Buscar cadastros ==\n";
    cout << "1. Buscar Abrigado\n";
    cout << "2. Buscar Administrador\n> ";
    cin >> tipo; cin.ignore();

    if (tipo == 2) {
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
    } else if (tipo == 1) {
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