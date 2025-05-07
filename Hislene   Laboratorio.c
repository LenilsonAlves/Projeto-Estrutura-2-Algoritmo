#include <stdio.h>    // Biblioteca padrão de entrada e saída
#include <stdlib.h>   // Biblioteca padrão para funções de alocação de memória, etc.
#include <string.h>   // Biblioteca para manipulação de strings
#include <math.h>     // Biblioteca para funções matemáticas

#define TAMANHO_TABELA 75   // Tamanho da tabela hash

// Definição da estrutura para armazenar os dados do contato
typedef struct ItemDados {
    char nome[50];
    char email[100];
    char telefone[15];
    int chave;  // Chave usada para indexação na tabela hash
} ItemDados;

ItemDados *tabelaHash[TAMANHO_TABELA]; // Tabela hash para armazenar os contatos

// Função para calcular o primeiro hash
int multiplicacao(ItemDados *d) {
    long long int chave = (long long int)d->chave;  // Converte a chave para long long int
    chave = chave * 2654435761LL;  // Multiplica a chave por um número primo
    return (int)(chave % TAMANHO_TABELA);  // Retorna o resultado do módulo
}

// Função para calcular o segundo hash
int segundoHash(ItemDados *d) {
    long long int chave = (long long int)d->chave;  // Converte a chave para long long int
    chave = chave * 31415 + 1;  // Multiplica a chave por outro número primo e adiciona 1
    return (int)(chave % (TAMANHO_TABELA - 1)) + 1; // Retorna o resultado do módulo, garantindo que seja diferente de zero
}

// Função para inicializar a tabela hash
void inicializarTabelaHash() {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        tabelaHash[i] = NULL;  // Define todos os elementos da tabela como NULL
    }
}

// Função para inserir um contato na tabela hash
void inserir(ItemDados *item) {
    int chave = item->chave;  // Obtém a chave do item
    int indiceHash = multiplicacao(item);  // Calcula o primeiro hash
    int segundoPasso = segundoHash(item);  // Calcula o segundo hash
    int tentativas = 0;  // Contador de tentativas de inserção

    // Procura um índice vazio na tabela ou até esgotar o número máximo de tentativas
    while (tabelaHash[indiceHash] != NULL && tentativas < TAMANHO_TABELA) {
        indiceHash = (indiceHash + segundoPasso) % TAMANHO_TABELA;  // Próximo índice
        tentativas++;  // Incrementa o contador de tentativas
    }

    // Se encontrou um espaço vazio na tabela, insere o item
    if (tabelaHash[indiceHash] == NULL) {
        tabelaHash[indiceHash] = item;
    } else {
        printf("Tentativas esgotadas. Nao e possivel inserir o item.\n");
    }
}

// Função para listar todos os contatos da tabela hash
void listarContatos() {
    printf("\nLista de Contatos:\n");
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        if (tabelaHash[i] != NULL) {
            printf("Chave: %d\n", tabelaHash[i]->chave);
            printf("Nome: %s\n", tabelaHash[i]->nome);
            printf("Email: %s\n", tabelaHash[i]->email);
            printf("Telefone: %s\n\n", tabelaHash[i]->telefone);
        }
    }
}

// Função para editar um contato na tabela hash
void editarContato(int chave) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        if (tabelaHash[i] != NULL && tabelaHash[i]->chave == chave) {
            printf("Novo nome: ");
            scanf("%s", tabelaHash[i]->nome);
            printf("Novo email: ");
            scanf("%s", tabelaHash[i]->email);
            printf("Novo telefone: ");
            scanf("%s", tabelaHash[i]->telefone);
            printf("Contato editado com sucesso!\n");
            return;
        }
    }
    printf("Contato nao encontrado.\n");
}

// Função para excluir um contato da tabela hash
void excluirContato(int chave) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        if (tabelaHash[i] != NULL && tabelaHash[i]->chave == chave) {
            free(tabelaHash[i]);  // Libera a memória alocada para o contato
            tabelaHash[i] = NULL;  // Define o elemento como NULL
            printf("Contato excluido com sucesso!\n");
            return;
        }
    }
    printf("Contato nao encontrado.\n");
}

// Função para visualizar as colisões de entrada na tabela hash
void visualizarColisoes() {
    printf("\nColisoes de Entrada:\n");
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        if (tabelaHash[i] != NULL) {
            int indiceHash = multiplicacao(tabelaHash[i]);  // Calcula o primeiro hash
            int segundoPasso = segundoHash(tabelaHash[i]);  // Calcula o segundo hash
            // Se o primeiro hash não coincidir com o índice atual, há uma colisão
            if (indiceHash != i) {
                printf("\nChave: %d, houve uma colisao na posicao: %d\n", tabelaHash[i]->chave, i);
                printf("Metodo do duplo hash, indices investigados: ");
                int tentativas = 0;
                // Procura pelos índices investigados durante a inserção
                while (tabelaHash[indiceHash] != NULL && tentativas < TAMANHO_TABELA) {
                    printf("%d ", indiceHash);
                    indiceHash = (indiceHash + segundoPasso) % TAMANHO_TABELA;  // Próximo índice
                    tentativas++;  // Incrementa o contador de tentativas
                }
                printf("\n");
            }
        }
    }
}

// Função para criar um arquivo de contatos
void criarArquivoContatos() {
    FILE *arquivo = fopen("contatos.txt", "w");  // Abre o arquivo para escrita
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo de contatos.\n");
        return;
    }

    // Escreve os contatos no arquivo
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        if (tabelaHash[i] != NULL) {
            fprintf(arquivo, "Chave: %d\n", tabelaHash[i]->chave);
            fprintf(arquivo, "Nome: %s\n", tabelaHash[i]->nome);
            fprintf(arquivo, "Email: %s\n", tabelaHash[i]->email);
            fprintf(arquivo, "Telefone: %s\n\n", tabelaHash[i]->telefone);
        }
    }

    fclose(arquivo);  // Fecha o arquivo após a escrita
    printf("Arquivo de contatos criado com sucesso.\n");
}

// Função para ler os contatos de um arquivo
void lerContatosDoArquivo() {
    FILE *arquivo = fopen("todosOsContatos.txt", "r");  // Abre o arquivo para leitura
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de contatos.\n");
        return;
    }

    ItemDados *novoItem;  // Variável temporária para armazenar os novos contatos lidos
    char buffer[100];  // Buffer para armazenar temporariamente cada linha do arquivo

    // Lê o arquivo linha por linha
    while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
        // Verifica se a linha contém o padrão "Nome: " para identificar um novo contato
        if (strstr(buffer, "Nome: ") != NULL) {
            novoItem = (ItemDados *)malloc(sizeof(ItemDados));  // Aloca memória para o novo contato
            sscanf(buffer, "Nome: %[^\n]", novoItem->nome); // Lê o nome até a quebra de linha

            fgets(buffer, sizeof(buffer), arquivo); // Lê a linha "Telefone: "
            sscanf(buffer, "Telefone: %[^\n]", novoItem->telefone); // Lê o telefone até a quebra de linha

            fgets(buffer, sizeof(buffer), arquivo); // Lê a linha "Email: "
            sscanf(buffer, "Email: %[^\n]", novoItem->email); // Lê o email até a quebra de linha

            // Calcula a chave do contato a partir do nome
            novoItem->chave = 0;
            for (int i = 0; novoItem->nome[i] != '\0'; i++) {
                novoItem->chave += novoItem->nome[i];
            }

            inserir(novoItem);  // Insere o novo contato na tabela hash
        }
    }

    fclose(arquivo);  // Fecha o arquivo após a leitura
}

int main() {
    inicializarTabelaHash();  // Inicializa a tabela hash
    lerContatosDoArquivo();  // Lê os contatos do arquivo

    int escolha;

    // Loop do menu principal
    do {
        // Apresenta o menu principal e solicita a escolha do usuário
        printf("\nMenu:\n");
        printf("1. Adicionar novo contato\n");
        printf("2. Listar todos os contatos\n");
        printf("3. Editar um contato\n");
        printf("4. Excluir um contato\n");
        printf("5. Visualizar colisoes de entrada\n");
        printf("6. Criar arquivo de contatos\n");
        printf("7. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);

        // Executa a opção escolhida pelo usuário
        switch (escolha) {
            case 1:
                {
                    ItemDados *novoItem = (ItemDados *)malloc(sizeof(ItemDados));  // Aloca memória para o novo contato
                    printf("Chave: ");
                    scanf("%d", &novoItem->chave);
                    printf("Nome: ");
                    scanf("%s", novoItem->nome);
                    printf("Email: ");
                    scanf("%s", novoItem->email);
                    printf("Telefone: ");
                    scanf("%s", novoItem->telefone);
                    inserir(novoItem);  // Insere o novo contato na tabela hash
                }
                break;
            case 2:
                listarContatos();  // Lista todos os contatos da tabela hash
                break;
            case 3:
                {
                    int chave;
                    printf("Digite a chave do contato que deseja editar: ");
                    scanf("%d", &chave);
                    editarContato(chave);  // Edita um contato existente na tabela hash
                }
                break;
            case 4:
                {
                    int chave;
                    printf("Digite a chave do contato que deseja excluir: ");
                    scanf("%d", &chave);
                    excluirContato(chave);  // Exclui um contato da tabela hash
                }
                break;
            case 5:
                visualizarColisoes();  // Visualiza as colisões de entrada na tabela hash
                break;
            case 6:
                criarArquivoContatos();  // Cria um arquivo de contatos com os dados da tabela hash
                break;
            case 7:
                printf("Saindo do programa...\n");  // Encerra o programa
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");  // Mensagem de erro para opções inválidas
                break;
        }
    } while (escolha != 7);  // Repete o loop até que o usuário escolha sair

    return 0;
}
