#include "Inc\System.h"

using namespace std;

int main() {
    int opcao;
    System execute;
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
            case 1: execute.cadastrarNovoAbr(); break;
            case 2: execute.cadastrarNovoAdm(); break;
            case 3: execute.exibirCadastrados(); break;
            case 4: execute.menuBusca(); break;
            case 5: {
                int tipo;
                cout << "Editar cadastro de:\n1. Administrador\n2. Abrigado\n> ";
                cin >> tipo; cin.ignore();
                execute.editarCadastro(tipo == 1);
                break;
            }
            case 6: {
                int tipo;
                string confirm;
                cout << "Excluir cadastro de:\n1. Administrador\n2. Abrigado\n> ";
                cin >> tipo; cin.ignore();
                cout << "Tem certeza que deseja excluir? (s para confirmar): ";
                getline(cin, confirm);
                if (confirm == "s") execute.excluirCadastro(tipo == 1);
                else cout << "Operacao cancelada.\n";
                break;
            }
            case 7: cout << "Saindo...\n"; break;
            default: cout << "Opcao invalida!\n";
        }
    } while (opcao != 7);

    return 0;
}