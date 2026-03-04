// ===================== BIBLIOTECAS =====================
#include <stdio.h>    // Biblioteca padrão de entrada e saída — usada para funções como printf(), scanf(), entra outras.
#include <string.h>   // Biblioteca de manipulação de strings — fornece funções como strlen(), strcpy(), strcmp(), strcat().
#include <stdlib.h>   // Biblioteca padrão de utilidades — inclui funções de alocação de memória (malloc, free).
#include <locale.h>   // Biblioteca de configuração regional — usada para definir localizações (idioma, acentuação, formatação de números) com setlocale().
#include <windows.h>  // Biblioteca específica do Windows — usada para acessar funções da API do Windows, como Sleep(), system(), manipulação de janelas, e outros recursos do sistema operacional.


// ===================== CONSTANTES =====================

#define MAX_CONTAS 100     // Define o número máximo de contas que podem ser cadastradas no sistema (limite de 100 contas)
#define TAM_NOME 100       // Define o tamanho máximo (número de caracteres) permitido para armazenar o nome de uma pessoa
#define TAM_CPF 15         // Define o tamanho máximo do campo de CPF (inclui pontos e traço, ex: "123.456.789-10" = 14 caracteres + '\0')
#define TAM_AGENCIA 10     // Define o tamanho máximo para o código ou número da agência bancária
#define TAM_TELEFONE 20    // Define o tamanho máximo para armazenar números de telefone (incluindo DDD, +55, espaços, traços, etc.)
#define ATIVA 1            // Define o valor que representa uma conta ativa (status = ativa)
#define ENCERRADA 0        // Define o valor que representa uma conta encerrada (status = encerrada)

// ===================== STRUCT =====================
typedef struct {
    int numero;                 
    char nome[TAM_NOME];
    char cpf[TAM_CPF];
    char agencia[TAM_AGENCIA];
    char telefone[TAM_TELEFONE];
    double saldo;
    int status;                 
} Conta;

// ===================== VARIÁVEIS GLOBAIS =====================
static Conta contas[MAX_CONTAS];
static int totalContas = 0;

// ===================== PROTÓTIPOS =====================
void menu();
void abrirConta();
void depositar();
void sacar();
void transferir();
void consultarSaldoDados();
void atualizarDadosCliente();
void listarContas();
void encerrarConta();

int buscarContaPorNumero(int numero);
int buscarContaPorCPF(char cpf[]);

// ===================== FUNÇÃO PRINCIPAL =====================
int main() {
    setlocale(LC_ALL, "Portuguese");
    menu();
    return 0;
}

// ===================== MENU PRINCIPAL =====================
void menu() {
    int opcao;

    do {
        system("cls");
        printf("===================================\n");
        printf("===============ATM=================\n");
        printf("===========Banco Malvezzi==========\n");
        printf("===================================\n");
        printf("1 - Abrir conta\n");
        printf("2 - Depositar\n");
        printf("3 - Sacar\n");
        printf("4 - Transferir\n");
        printf("5 - Consultar saldo e dados\n");
        printf("6 - Atualizar dados do cliente\n");
        printf("7 - Listar contas\n");
        printf("8 - Encerrar conta\n");
        printf("9 - Sair\n");
        printf("====================================\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: abrirConta(); break;
            case 2: depositar(); break;
            case 3: sacar(); break;
            case 4: transferir(); break;
            case 5: consultarSaldoDados(); break;
            case 6: atualizarDadosCliente(); break;
            case 7: listarContas(); break;
            case 8: encerrarConta(); break;
            case 9: printf("Encerrando o programa...\n"); Sleep(2000); exit(0);
            default: printf("Opção inválida.\n"); Sleep(1500);
        }
    } while (1);
}

// ===================== FUNÇÕES =====================
void abrirConta() { //Função responsável por criar a conta bancária
    if (totalContas >= MAX_CONTAS) {
        printf("Limite máximo de contas atingido.\n");
        Sleep(2000);
        return;
    }

    Conta nova;
    nova.numero = totalContas + 1;
    nova.saldo = 0.0;
    nova.status = ATIVA;

    printf("\n=== ABRIR CONTA ===\n");
    printf("Nome: ");
    fgets(nova.nome, TAM_NOME, stdin);
    nova.nome[strcspn(nova.nome, "\n")] = '\0';

    printf("CPF: ");
    fgets(nova.cpf, TAM_CPF, stdin);
    nova.cpf[strcspn(nova.cpf, "\n")] = '\0';

    if (buscarContaPorCPF(nova.cpf) != -1) {
        printf("Erro: Já existe uma conta ativa com esse CPF.\n");
        Sleep(2000);
        return;
    }

    // Limpa o buffer antes de ler a agência
    fflush(stdin);

    printf("Agência: ");
    fgets(nova.agencia, TAM_AGENCIA, stdin);
    nova.agencia[strcspn(nova.agencia, "\n")] = '\0';

    printf("Telefone: ");
    fgets(nova.telefone, TAM_TELEFONE, stdin);
    nova.telefone[strcspn(nova.telefone, "\n")] = '\0';

    contas[totalContas++] = nova;

    printf("\nConta criada com sucesso!\nNúmero da conta: %d\n", nova.numero);
    Sleep(2000);
}

void depositar() {//Função responsável efetuar o depósito para a conta recem criada
    int numero;
    double valor;

    printf("\n=== DEPÓSITO ===\n");
    printf("Informe o número da conta: ");
    scanf("%d", &numero);

    int idx = buscarContaPorNumero(numero);
    if (idx == -1) {
        printf("Conta não encontrada.\n");
        Sleep(2000);
        return;
    }

    if (contas[idx].status == ENCERRADA) {
        printf("Operação não permitida: conta encerrada.\n");
        Sleep(2000);
        return;
    }

    printf("Informe o valor a depositar: ");
    scanf("%lf", &valor);

    if (valor <= 0) {
        printf("Valor inválido.\n");
        Sleep(2000);
        return;
    }

    contas[idx].saldo += valor;
    printf("Depósito realizado com sucesso!\n");
    Sleep(2000);
}

void sacar() {//Função para sacar os valores das contas criadas
    int numero;
    double valor;

    printf("\n=== SAQUE ===\n");
    printf("Informe o número da conta: ");
    scanf("%d", &numero);

    int idx = buscarContaPorNumero(numero);
    if (idx == -1) {
        printf("Conta não encontrada.\n");
        Sleep(2000);
        return;
    }

    if (contas[idx].status == ENCERRADA) {
        printf("Operação não permitida: conta encerrada.\n");
        Sleep(2000);
        return;
    }

    printf("Informe o valor do saque: ");
    scanf("%lf", &valor);

    if (valor <= 0 || valor > contas[idx].saldo) {
        printf("Valor inválido ou saldo insuficiente.\n");
        Sleep(2000);
        return;
    }

    contas[idx].saldo -= valor;
    printf("Saque realizado com sucesso!\n");
    Sleep(2000);
}

void transferir() {//Função para transferir da conta de origem para a conta destino
    int origem, destino;
    double valor;

    printf("\n=== TRANSFERÊNCIA ===\n");
    printf("Conta origem: ");
    scanf("%d", &origem);
    printf("Conta destino: ");
    scanf("%d", &destino);

    int idxO = buscarContaPorNumero(origem);
    int idxD = buscarContaPorNumero(destino);

    if (idxO == -1 || idxD == -1) {
        printf("Conta origem ou destino não encontrada.\n");
        Sleep(2000);
        return;
    }

    if (contas[idxO].status == ENCERRADA || contas[idxD].status == ENCERRADA) {
        printf("Transferência não permitida: uma das contas está encerrada.\n");
        Sleep(2000);
        return;
    }

    printf("Valor da transferência: ");
    scanf("%lf", &valor);

    if (valor <= 0 || valor > contas[idxO].saldo) {
        printf("Valor inválido ou saldo insuficiente.\n");
        Sleep(2000);
        return;
    }

    contas[idxO].saldo -= valor;
    contas[idxD].saldo += valor;
    printf("Transferência realizada com sucesso!\n");
    Sleep(2000);
}

void consultarSaldoDados() {//Função para consultar Saldo e Dados pessoais do cliente
    int opcao;
    printf("\n=== CONSULTAR SALDO E DADOS ===\n");
    printf("1 - Buscar por número da conta\n");
    printf("2 - Buscar por CPF\n");
    scanf("%d", &opcao);
    getchar();

    int idx = -1;
    if (opcao == 1) {
        int numero;
        printf("Informe o número da conta: ");
        scanf("%d", &numero);
        idx = buscarContaPorNumero(numero);
    } else if (opcao == 2) {
    char cpf[TAM_CPF];
    printf("Informe o CPF: ");
    fflush(stdin); // garante que o buffer esteja limpo
    fgets(cpf, TAM_CPF, stdin);
    cpf[strcspn(cpf, "\n")] = '\0'; // remove o \n
    idx = buscarContaPorCPF(cpf);
}


    if (idx == -1) {
        printf("Conta não encontrada.\n");
        Sleep(2000);
        return;
    }

    Conta c = contas[idx];
    printf("\nNúmero: %d\nNome: %s\nCPF: %s\nAgência: %s\nTelefone: %s\nStatus: %s\nSaldo: R$ %.2lf\n",
           c.numero, c.nome, c.cpf, c.agencia, c.telefone,
           (c.status == ATIVA ? "Ativa" : "Encerrada"), c.saldo);
    system("pause");
}

void atualizarDadosCliente() {//Função para atualizar dados do cliente
    int numero;
    printf("\n=== ATUALIZAR DADOS ===\n");
    printf("Informe o número da conta: ");
    scanf("%d", &numero);
    getchar();

    int idx = buscarContaPorNumero(numero);
    if (idx == -1) {
        printf("Conta não encontrada.\n");
        Sleep(2000);
        return;
    }

    if (contas[idx].status == ENCERRADA) {
        printf("Conta encerrada. Não é possível atualizar.\n");
        Sleep(2000);
        return;
    }

    printf("Novo telefone: ");
    fgets(contas[idx].telefone, TAM_TELEFONE, stdin);
    contas[idx].telefone[strcspn(contas[idx].telefone, "\n")] = '\0';

    printf("Nova agência: ");
    fgets(contas[idx].agencia, TAM_AGENCIA, stdin);
    contas[idx].agencia[strcspn(contas[idx].agencia, "\n")] = '\0';

    printf("Dados atualizados com sucesso!\n");
    Sleep(2000);
}

void listarContas() {//Função para listar contas ativas, inativas ou todas
    int filtro;
    printf("\n=== LISTAR CONTAS ===\n");
    printf("1 - Somente ativas\n2 - Somente encerradas\n3 - Todas\n");
    scanf("%d", &filtro);

    for (int i = 0; i < totalContas; i++) {
        if ((filtro == 1 && contas[i].status == ATIVA) ||
            (filtro == 2 && contas[i].status == ENCERRADA) ||
            (filtro == 3)) {
            printf("\nConta: %d | Nome: %s | Saldo: R$ %.2lf | Status: %s\n",
                   contas[i].numero, contas[i].nome, contas[i].saldo,
                   (contas[i].status == ATIVA ? "Ativa" : "Encerrada"));
        }
    }
    system("pause");
}

void encerrarConta() {//Função para excluir contas cadastradas se estiverem zeradas
    int numero;
    printf("\n=== ENCERRAR CONTA ===\n");
    printf("Informe o número da conta: ");
    scanf("%d", &numero);

    int idx = buscarContaPorNumero(numero);
    if (idx == -1) {
        printf("Conta não encontrada.\n");
        Sleep(2000);
        return;
    }

    if (contas[idx].saldo != 0) {
        printf("Não é possível encerrar: saldo diferente de zero.\n");
        Sleep(2000);
        return;
    }

    contas[idx].status = ENCERRADA;
    printf("Conta encerrada com sucesso!\n");
    Sleep(2000);
}

// ===================== FUNÇÕES AUXILIARES =====================
int buscarContaPorNumero(int numero) {/*Função utilizada para buscar a conta pelo número, 
utilizada nas Funções Depositar, Sacar, Transferir, Consultar Saldo e Dados, Atualizar Dados e Encerrar conta*/
    for (int i = 0; i < totalContas; i++) {
        if (contas[i].numero == numero) return i;
    }
    return -1;
}

int buscarContaPorCPF(char cpf[]) {
    for (int i = 0; i < totalContas; i++) {
        if (strcmp(contas[i].cpf, cpf) == 0 && contas[i].status == ATIVA)
            return i;
    }
    return -1;
}

