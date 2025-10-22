/*
 * Detective Quest - Sistema de Exploração da Mansão com Coleta de Pistas
 * Enigma Studios
 * 
 * Este programa simula a exploração de uma mansão representada por uma
 * árvore binária, onde cada nó representa um cômodo que pode conter pistas.
 * As pistas coletadas são armazenadas em uma BST e exibidas em ordem alfabética.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para armazenar pistas em uma árvore BST
typedef struct PistaNode {
    char pista[100];                // Conteúdo da pista
    struct PistaNode *esquerda;     // Subárvore esquerda (pistas menores)
    struct PistaNode *direita;      // Subárvore direita (pistas maiores)
} PistaNode;

// Estrutura que representa cada sala da mansão
typedef struct Sala {
    char nome[50];                  // Nome do cômodo
    char pista[100];                // Pista encontrada neste cômodo (vazia se não houver)
    struct Sala *esquerda;          // Ponteiro para sala à esquerda
    struct Sala *direita;           // Ponteiro para sala à direita
} Sala;

/*
 * Função: criarSala
 * Descrição: Cria dinamicamente uma nova sala com nome e pista opcional
 * Parâmetros:
 *   - nome: string com o nome do cômodo
 *   - pista: string com a pista (pode ser "" se não houver pista)
 * Retorno: ponteiro para a sala criada
 */
Sala* criarSala(const char *nome, const char *pista) {
    // Aloca memória dinamicamente para a nova sala
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    
    // Verifica se a alocação foi bem-sucedida
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para sala!\n");
        exit(1);
    }
    
    // Copia o nome e a pista para a estrutura
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    
    // Inicializa os ponteiros como NULL
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/*
 * Função: inserirPista
 * Descrição: Insere uma nova pista na árvore BST de forma ordenada
 * Parâmetros:
 *   - raiz: ponteiro para a raiz da árvore de pistas
 *   - pista: string com a pista a ser inserida
 * Retorno: ponteiro para a raiz da árvore (pode ser alterada)
 */
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    // Caso base: posição encontrada para inserção
    if (raiz == NULL) {
        PistaNode *novaPista = (PistaNode*)malloc(sizeof(PistaNode));
        
        if (novaPista == NULL) {
            printf("Erro ao alocar memória para pista!\n");
            exit(1);
        }
        
        strcpy(novaPista->pista, pista);
        novaPista->esquerda = NULL;
        novaPista->direita = NULL;
        
        return novaPista;
    }
    
    // Compara a pista com o nó atual para decidir o lado de inserção
    int comparacao = strcmp(pista, raiz->pista);
    
    if (comparacao < 0) {
        // Pista é menor alfabeticamente, vai para a esquerda
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        // Pista é maior alfabeticamente, vai para a direita
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se comparacao == 0, a pista já existe (não insere duplicata)
    
    return raiz;
}

/*
 * Função: exibirPistas
 * Descrição: Exibe todas as pistas em ordem alfabética (percurso in-order)
 * Parâmetros:
 *   - raiz: ponteiro para a raiz da árvore de pistas
 * Retorno: void
 */
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) {
        return;
    }
    
    // Percurso in-order: esquerda -> raiz -> direita
    exibirPistas(raiz->esquerda);
    printf("  - %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

/*
 * Função: explorarSalasComPistas
 * Descrição: Permite a navegação interativa pela mansão com coleta de pistas
 * Parâmetros:
 *   - salaAtual: ponteiro para a sala onde o jogador está
 *   - arvorePistas: ponteiro para ponteiro da árvore de pistas (para modificá-la)
 * Retorno: void
 */
void explorarSalasComPistas(Sala *salaAtual, PistaNode **arvorePistas) {
    char escolha;
    int pistasTotais = 0;
    
    // Loop principal de exploração
    while (salaAtual != NULL) {
        // Exibe informações da sala atual
        printf("\n================================================\n");
        printf("Você está em: %s\n", salaAtual->nome);
        printf("================================================\n");
        
        // Verifica se há pista nesta sala
        if (strlen(salaAtual->pista) > 0) {
            printf("\n🔍 PISTA ENCONTRADA!\n");
            printf("   \"%s\"\n", salaAtual->pista);
            
            // Adiciona a pista à árvore BST
            *arvorePistas = inserirPista(*arvorePistas, salaAtual->pista);
            pistasTotais++;
            
            printf("\n   [Pista adicionada ao diário do detetive]\n");
        } else {
            printf("\nNenhuma pista encontrada nesta sala.\n");
        }
        
        // Verifica se é uma sala sem saídas
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("\n⚠️  Esta sala não possui mais caminhos!\n");
            printf("    Você pode sair para revisar as pistas coletadas.\n");
        }
        
        // Mostra as opções disponíveis
        printf("\n--- Menu de Navegação ---\n");
        if (salaAtual->esquerda != NULL) {
            printf("  [E] - Ir para a esquerda\n");
        }
        if (salaAtual->direita != NULL) {
            printf("  [D] - Ir para a direita\n");
        }
        printf("  [S] - Sair e revisar pistas coletadas\n");
        printf("\nPistas coletadas até agora: %d\n", pistasTotais);
        printf("\nSua escolha: ");
        
        // Lê a escolha do jogador
        scanf(" %c", &escolha);
        
        // Processa a escolha
        if (escolha == 'e' || escolha == 'E') {
            if (salaAtual->esquerda != NULL) {
                printf("\n➜ Seguindo para a esquerda...\n");
                salaAtual = salaAtual->esquerda;
            } else {
                printf("\n❌ Não há caminho à esquerda!\n");
            }
        } 
        else if (escolha == 'd' || escolha == 'D') {
            if (salaAtual->direita != NULL) {
                printf("\n➜ Seguindo para a direita...\n");
                salaAtual = salaAtual->direita;
            } else {
                printf("\n❌ Não há caminho à direita!\n");
            }
        } 
        else if (escolha == 's' || escolha == 'S') {
            printf("\n➜ Encerrando exploração...\n");
            break;
        } 
        else {
            printf("\n❌ Opção inválida! Tente novamente.\n");
        }
    }
}

/*
 * Função: liberarArvore
 * Descrição: Libera toda a memória alocada para a árvore de salas
 * Parâmetros:
 *   - raiz: ponteiro para a raiz da árvore
 * Retorno: void
 */
void liberarArvore(Sala *raiz) {
    if (raiz == NULL) {
        return;
    }
    
    // Libera recursivamente as subárvores
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    
    // Libera o nó atual
    free(raiz);
}

/*
 * Função: liberarArvorePistas
 * Descrição: Libera toda a memória alocada para a árvore de pistas
 * Parâmetros:
 *   - raiz: ponteiro para a raiz da árvore de pistas
 * Retorno: void
 */
void liberarArvorePistas(PistaNode *raiz) {
    if (raiz == NULL) {
        return;
    }
    
    // Libera recursivamente as subárvores
    liberarArvorePistas(raiz->esquerda);
    liberarArvorePistas(raiz->direita);
    
    // Libera o nó atual
    free(raiz);
}

/*
 * Função: main
 * Descrição: Ponto de entrada do programa. Cria o mapa da mansão com pistas
 *            e gerencia o sistema de exploração e coleta
 * Retorno: 0 se execução bem-sucedida
 */
int main() {
    printf("==============================================\n");
    printf("     DETECTIVE QUEST - ENIGMA STUDIOS\n");
    printf("        Sistema de Coleta de Pistas\n");
    printf("==============================================\n");
    printf("\nBem-vindo à mansão misteriosa!\n");
    printf("Explore os cômodos e colete pistas para\n");
    printf("desvendar o mistério...\n");
    
    // Inicializa a árvore BST de pistas (vazia no início)
    PistaNode *arvorePistas = NULL;
    
    // Criação da estrutura da mansão com pistas
    
    // Nível 0 - Entrada
    Sala *hall = criarSala("Hall de Entrada", "Pegadas molhadas no tapete");
    
    // Nível 1
    Sala *salaEstar = criarSala("Sala de Estar", "");
    Sala *cozinha = criarSala("Cozinha", "Faca desaparecida do bloco");
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    
    // Nível 2 - Ramificações da Sala de Estar
    Sala *biblioteca = criarSala("Biblioteca", "Livro aberto sobre venenos");
    Sala *escritorio = criarSala("Escritório", "");
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = escritorio;
    
    // Nível 2 - Ramificações da Cozinha
    Sala *despensa = criarSala("Despensa", "Frasco vazio de arsênico");
    Sala *jardim = criarSala("Jardim", "");
    cozinha->esquerda = despensa;
    cozinha->direita = jardim;
    
    // Nível 3 - Ramificações da Biblioteca
    Sala *salaSecreta = criarSala("Sala Secreta", "Documento queimado parcialmente");
    Sala *salaLeitura = criarSala("Sala de Leitura", "Carta ameaçadora escondida");
    biblioteca->esquerda = salaSecreta;
    biblioteca->direita = salaLeitura;
    
    // Nível 3 - Ramificações do Escritório
    Sala *cofre = criarSala("Cofre", "Testamento adulterado");
    escritorio->esquerda = cofre;
    
    // Nível 3 - Ramificações do Jardim
    Sala *estufa = criarSala("Estufa", "Planta venenosa cultivada");
    jardim->direita = estufa;
    
    // Inicia a exploração com coleta de pistas
    explorarSalasComPistas(hall, &arvorePistas);
    
    // Exibe o resumo das pistas coletadas
    printf("\n==============================================\n");
    printf("        DIÁRIO DO DETETIVE\n");
    printf("     Pistas Coletadas (em ordem alfabética)\n");
    printf("==============================================\n");
    
    if (arvorePistas == NULL) {
        printf("\nNenhuma pista foi coletada.\n");
    } else {
        printf("\n");
        exibirPistas(arvorePistas);
    }
    
    // Libera toda a memória alocada
    liberarArvore(hall);
    liberarArvorePistas(arvorePistas);
    
    printf("\n==============================================\n");
    printf("Obrigado por jogar Detective Quest!\n");
    printf("Use as pistas para desvendar o mistério...\n");
    printf("==============================================\n");
    
    return 0;
}