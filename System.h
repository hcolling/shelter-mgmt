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
    void salvarAdmCSV(Adm *adm);
    void salvarShelteredCSV(Sheltered *s);

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