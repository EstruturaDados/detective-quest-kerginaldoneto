/*
 * Detective Quest - Sistema de Exploração da Mansão
 * Enigma Studios
 * 
 * Este programa simula a exploração de uma mansão representada
 * por uma árvore binária, onde cada nó representa um cômodo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura que representa cada sala da mansão
typedef struct Sala {
    char nome[50];           // Nome do cômodo
    struct Sala *esquerda;   // Ponteiro para sala à esquerda
    struct Sala *direita;    // Ponteiro para sala à direita
} Sala;

/*
 * Função: criarSala
 * Descrição: Cria dinamicamente uma nova sala com o nome especificado
 * Parâmetros:
 *   - nome: string com o nome do cômodo
 * Retorno: ponteiro para a sala criada
 */
Sala* criarSala(const char *nome) {
    // Aloca memória dinamicamente para a nova sala
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    
    // Verifica se a alocação foi bem-sucedida
    if (novaSala == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    
    // Copia o nome para a estrutura
    strcpy(novaSala->nome, nome);
    
    // Inicializa os ponteiros como NULL (sem salas conectadas)
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/*
 * Função: explorarSalas
 * Descrição: Permite a navegação interativa do jogador pela mansão
 * Parâmetros:
 *   - salaAtual: ponteiro para a sala onde o jogador está no momento
 * Retorno: void
 */
void explorarSalas(Sala *salaAtual) {
    char escolha;
    
    // Loop principal de exploração
    while (salaAtual != NULL) {
        // Exibe informações da sala atual
        printf("\n================================================\n");
        printf("Você está em: %s\n", salaAtual->nome);
        printf("================================================\n");
        
        // Verifica se é uma sala sem saídas (nó-folha)
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("\nEsta sala não possui mais caminhos!\n");
            printf("Fim da exploração.\n");
            break;
        }
        
        // Mostra as opções disponíveis
        printf("\nEscolha seu caminho:\n");
        if (salaAtual->esquerda != NULL) {
            printf("  [E] - Ir para a esquerda\n");
        }
        if (salaAtual->direita != NULL) {
            printf("  [D] - Ir para a direita\n");
        }
        printf("  [S] - Sair da exploração\n");
        printf("\nSua escolha: ");
        
        // Lê a escolha do jogador
        scanf(" %c", &escolha);
        
        // Processa a escolha
        if (escolha == 'e' || escolha == 'E') {
            if (salaAtual->esquerda != NULL) {
                printf("\n-> Seguindo para a esquerda...\n");
                salaAtual = salaAtual->esquerda;
            } else {
                printf("\nNão há caminho à esquerda!\n");
            }
        } 
        else if (escolha == 'd' || escolha == 'D') {
            if (salaAtual->direita != NULL) {
                printf("\n-> Seguindo para a direita...\n");
                salaAtual = salaAtual->direita;
            } else {
                printf("\nNão há caminho à direita!\n");
            }
        } 
        else if (escolha == 's' || escolha == 'S') {
            printf("\nSaindo da exploração...\n");
            break;
        } 
        else {
            printf("\nOpção inválida! Tente novamente.\n");
        }
    }
}

/*
 * Função: liberarArvore
 * Descrição: Libera toda a memória alocada para a árvore
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
 * Função: main
 * Descrição: Ponto de entrada do programa. Cria o mapa da mansão
 *            e inicia a exploração
 * Retorno: 0 se execução bem-sucedida
 */
int main() {
    printf("==============================================\n");
    printf("     DETECTIVE QUEST - ENIGMA STUDIOS\n");
    printf("==============================================\n");
    printf("\nBem-vindo à mansão misteriosa!\n");
    printf("Explore os cômodos para encontrar pistas...\n");
    
    // Criação da estrutura da mansão (árvore binária)
    
    // Nível 0 - Entrada
    Sala *hall = criarSala("Hall de Entrada");
    
    // Nível 1
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    
    // Nível 2 - Ramificações da Sala de Estar
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *escritorio = criarSala("Escritório");
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = escritorio;
    
    // Nível 2 - Ramificações da Cozinha
    Sala *despensa = criarSala("Despensa");
    Sala *jardim = criarSala("Jardim");
    cozinha->esquerda = despensa;
    cozinha->direita = jardim;
    
    // Nível 3 - Ramificações da Biblioteca
    Sala *salaSecreta = criarSala("Sala Secreta");
    Sala *salaLeitura = criarSala("Sala de Leitura");
    biblioteca->esquerda = salaSecreta;
    biblioteca->direita = salaLeitura;
    
    // Nível 3 - Ramificações do Escritório
    Sala *cofre = criarSala("Cofre");
    escritorio->esquerda = cofre;
    
    // Nível 3 - Ramificações do Jardim
    Sala *estufa = criarSala("Estufa");
    jardim->direita = estufa;
    
    // Inicia a exploração a partir do Hall de Entrada
    explorarSalas(hall);
    
    // Libera toda a memória alocada
    liberarArvore(hall);
    
    printf("\n==============================================\n");
    printf("Obrigado por jogar Detective Quest!\n");
    printf("==============================================\n");
    
    return 0;
}