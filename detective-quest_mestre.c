/*
 * Detective Quest - Sistema Completo de Investigação
 * Enigma Studios
 * 
 * Sistema integrado com:
 * - Árvore binária para navegação na mansão
 * - BST para armazenamento ordenado de pistas
 * - Tabela Hash para associação pista-suspeito
 * - Sistema de julgamento final
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_HASH 20  // Tamanho da tabela hash

// Estrutura para nó da tabela hash (lista encadeada para colisões)
typedef struct HashNode {
    char pista[100];                // Chave: pista
    char suspeito[50];              // Valor: nome do suspeito
    struct HashNode *proximo;       // Próximo nó em caso de colisão
} HashNode;

// Estrutura da tabela hash
typedef struct {
    HashNode *tabela[TAMANHO_HASH]; // Array de ponteiros para listas encadeadas
} TabelaHash;

// Estrutura para armazenar pistas em uma árvore BST
typedef struct PistaNode {
    char pista[100];                // Conteúdo da pista
    struct PistaNode *esquerda;     // Subárvore esquerda
    struct PistaNode *direita;      // Subárvore direita
} PistaNode;

// Estrutura que representa cada sala da mansão
typedef struct Sala {
    char nome[50];                  // Nome do cômodo
    struct Sala *esquerda;          // Sala à esquerda
    struct Sala *direita;           // Sala à direita
} Sala;

/*
 * Função: funcaoHash
 * Descrição: Calcula o índice hash para uma string (pista)
 * Parâmetros:
 *   - chave: string da pista
 * Retorno: índice na tabela hash (0 a TAMANHO_HASH-1)
 */
unsigned int funcaoHash(const char *chave) {
    unsigned int hash = 0;
    while (*chave) {
        hash = (hash * 31) + (*chave);
        chave++;
    }
    return hash % TAMANHO_HASH;
}

/*
 * Função: inicializarHash
 * Descrição: Inicializa a tabela hash com todos os ponteiros NULL
 * Parâmetros:
 *   - hash: ponteiro para a tabela hash
 * Retorno: void
 */
void inicializarHash(TabelaHash *hash) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        hash->tabela[i] = NULL;
    }
}

/*
 * Função: inserirNaHash
 * Descrição: Insere uma associação pista-suspeito na tabela hash
 * Parâmetros:
 *   - hash: ponteiro para a tabela hash
 *   - pista: string da pista (chave)
 *   - suspeito: string do nome do suspeito (valor)
 * Retorno: void
 */
void inserirNaHash(TabelaHash *hash, const char *pista, const char *suspeito) {
    unsigned int indice = funcaoHash(pista);
    
    // Cria novo nó
    HashNode *novoNo = (HashNode*)malloc(sizeof(HashNode));
    if (novoNo == NULL) {
        printf("Erro ao alocar memória para hash!\n");
        exit(1);
    }
    
    strcpy(novoNo->pista, pista);
    strcpy(novoNo->suspeito, suspeito);
    novoNo->proximo = hash->tabela[indice];  // Insere no início da lista
    hash->tabela[indice] = novoNo;
}

/*
 * Função: encontrarSuspeito
 * Descrição: Busca o suspeito associado a uma pista na tabela hash
 * Parâmetros:
 *   - hash: ponteiro para a tabela hash
 *   - pista: string da pista a ser buscada
 * Retorno: ponteiro para string com nome do suspeito (ou NULL se não encontrado)
 */
const char* encontrarSuspeito(TabelaHash *hash, const char *pista) {
    unsigned int indice = funcaoHash(pista);
    HashNode *atual = hash->tabela[indice];
    
    // Percorre a lista encadeada no índice
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    
    return NULL;  // Pista não encontrada
}

/*
 * Função: criarSala
 * Descrição: Cria dinamicamente uma nova sala com nome
 * Parâmetros:
 *   - nome: string com o nome do cômodo
 * Retorno: ponteiro para a sala criada
 */
Sala* criarSala(const char *nome) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para sala!\n");
        exit(1);
    }
    
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/*
 * Função: obterPistaDaSala
 * Descrição: Retorna a pista associada a uma sala específica (lógica codificada)
 * Parâmetros:
 *   - nomeSala: string com o nome da sala
 * Retorno: ponteiro para string com a pista (ou string vazia se não houver)
 */
const char* obterPistaDaSala(const char *nomeSala) {
    // Mapeamento estático: sala -> pista
    if (strcmp(nomeSala, "Hall de Entrada") == 0)
        return "Pegadas molhadas no tapete";
    else if (strcmp(nomeSala, "Cozinha") == 0)
        return "Faca desaparecida do bloco";
    else if (strcmp(nomeSala, "Biblioteca") == 0)
        return "Livro aberto sobre venenos";
    else if (strcmp(nomeSala, "Despensa") == 0)
        return "Frasco vazio de arsenico";
    else if (strcmp(nomeSala, "Sala Secreta") == 0)
        return "Documento queimado parcialmente";
    else if (strcmp(nomeSala, "Sala de Leitura") == 0)
        return "Carta ameacadora escondida";
    else if (strcmp(nomeSala, "Cofre") == 0)
        return "Testamento adulterado";
    else if (strcmp(nomeSala, "Estufa") == 0)
        return "Planta venenosa cultivada";
    else
        return "";  // Sala sem pista
}

/*
 * Função: inserirPista (ou adicionarPista)
 * Descrição: Insere uma nova pista na árvore BST de forma ordenada
 * Parâmetros:
 *   - raiz: ponteiro para a raiz da árvore de pistas
 *   - pista: string com a pista a ser inserida
 * Retorno: ponteiro para a raiz da árvore
 */
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
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
    
    int comparacao = strcmp(pista, raiz->pista);
    
    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    
    return raiz;
}

/*
 * Função: contarPistasPorSuspeito
 * Descrição: Conta quantas pistas apontam para um suspeito específico
 * Parâmetros:
 *   - raiz: ponteiro para a raiz da árvore de pistas
 *   - hash: ponteiro para a tabela hash
 *   - suspeitoAlvo: nome do suspeito a ser contado
 * Retorno: número de pistas que apontam para o suspeito
 */
int contarPistasPorSuspeito(PistaNode *raiz, TabelaHash *hash, const char *suspeitoAlvo) {
    if (raiz == NULL) {
        return 0;
    }
    
    int contador = 0;
    const char *suspeito = encontrarSuspeito(hash, raiz->pista);
    
    if (suspeito != NULL && strcmp(suspeito, suspeitoAlvo) == 0) {
        contador = 1;
    }
    
    // Recursivamente conta nas subárvores
    contador += contarPistasPorSuspeito(raiz->esquerda, hash, suspeitoAlvo);
    contador += contarPistasPorSuspeito(raiz->direita, hash, suspeitoAlvo);
    
    return contador;
}

/*
 * Função: exibirPistasComSuspeitos
 * Descrição: Exibe todas as pistas coletadas com seus respectivos suspeitos
 * Parâmetros:
 *   - raiz: ponteiro para a raiz da árvore de pistas
 *   - hash: ponteiro para a tabela hash
 * Retorno: void
 */
void exibirPistasComSuspeitos(PistaNode *raiz, TabelaHash *hash) {
    if (raiz == NULL) {
        return;
    }
    
    exibirPistasComSuspeitos(raiz->esquerda, hash);
    
    const char *suspeito = encontrarSuspeito(hash, raiz->pista);
    printf("  📋 \"%s\"\n", raiz->pista);
    if (suspeito != NULL) {
        printf("     ➜ Aponta para: %s\n\n", suspeito);
    }
    
    exibirPistasComSuspeitos(raiz->direita, hash);
}

/*
 * Função: explorarSalas
 * Descrição: Controla a navegação pela mansão e o sistema de coleta de pistas
 * Parâmetros:
 *   - salaAtual: ponteiro para a sala atual
 *   - arvorePistas: ponteiro para ponteiro da árvore de pistas
 * Retorno: void
 */
void explorarSalas(Sala *salaAtual, PistaNode **arvorePistas) {
    char escolha;
    int pistasTotais = 0;
    
    while (salaAtual != NULL) {
        printf("\n================================================\n");
        printf("📍 Localização: %s\n", salaAtual->nome);
        printf("================================================\n");
        
        // Verifica se há pista nesta sala
        const char *pista = obterPistaDaSala(salaAtual->nome);
        
        if (strlen(pista) > 0) {
            printf("\n🔍 PISTA ENCONTRADA!\n");
            printf("   \"%s\"\n", pista);
            
            *arvorePistas = inserirPista(*arvorePistas, pista);
            pistasTotais++;
            
            printf("\n   ✓ Pista registrada no diário\n");
        } else {
            printf("\n   Nenhuma pista encontrada aqui.\n");
        }
        
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("\n⚠️  Beco sem saída! Use [S] para revisar as pistas.\n");
        }
        
        printf("\n--- Opções de Navegação ---\n");
        if (salaAtual->esquerda != NULL) {
            printf("  [E] - Seguir para a esquerda\n");
        }
        if (salaAtual->direita != NULL) {
            printf("  [D] - Seguir para a direita\n");
        }
        printf("  [S] - Finalizar exploração\n");
        printf("\n📊 Pistas coletadas: %d\n", pistasTotais);
        printf("\nSua escolha: ");
        
        scanf(" %c", &escolha);
        
        if (escolha == 'e' || escolha == 'E') {
            if (salaAtual->esquerda != NULL) {
                printf("\n➜ Indo para a esquerda...\n");
                salaAtual = salaAtual->esquerda;
            } else {
                printf("\n❌ Caminho bloqueado!\n");
            }
        } 
        else if (escolha == 'd' || escolha == 'D') {
            if (salaAtual->direita != NULL) {
                printf("\n➜ Indo para a direita...\n");
                salaAtual = salaAtual->direita;
            } else {
                printf("\n❌ Caminho bloqueado!\n");
            }
        } 
        else if (escolha == 's' || escolha == 'S') {
            printf("\n➜ Retornando para análise das evidências...\n");
            break;
        } 
        else {
            printf("\n❌ Comando inválido!\n");
        }
    }
}

/*
 * Função: verificarSuspeitoFinal
 * Descrição: Conduz a fase de julgamento final e verifica a acusação
 * Parâmetros:
 *   - arvorePistas: ponteiro para a árvore de pistas
 *   - hash: ponteiro para a tabela hash
 * Retorno: void
 */
void verificarSuspeitoFinal(PistaNode *arvorePistas, TabelaHash *hash) {
    char acusado[50];
    
    printf("\n==============================================\n");
    printf("        ⚖️  FASE DE JULGAMENTO  ⚖️\n");
    printf("==============================================\n");
    
    if (arvorePistas == NULL) {
        printf("\n❌ Você não coletou pistas suficientes!\n");
        printf("   O caso permanece sem solução.\n");
        return;
    }
    
    printf("\n📂 Pistas coletadas e suspeitos relacionados:\n\n");
    exibirPistasComSuspeitos(arvorePistas, hash);
    
    printf("==============================================\n");
    printf("\nCom base nas evidências, quem você acusa?\n");
    printf("Digite o nome completo do suspeito: ");
    
    // Limpa o buffer antes de ler string com espaços
    getchar();
    fgets(acusado, sizeof(acusado), stdin);
    // Remove o \n do final
    acusado[strcspn(acusado, "\n")] = 0;
    
    printf("\n==============================================\n");
    printf("        🔎 ANALISANDO ACUSAÇÃO...\n");
    printf("==============================================\n");
    
    int quantidadePistas = contarPistasPorSuspeito(arvorePistas, hash, acusado);
    
    printf("\n📊 Resultado da análise:\n");
    printf("   Pistas apontando para %s: %d\n\n", acusado, quantidadePistas);
    
    if (quantidadePistas >= 2) {
        printf("✅ CASO RESOLVIDO!\n\n");
        printf("   Há evidências suficientes (%d pistas) para\n", quantidadePistas);
        printf("   sustentar a acusação contra %s.\n\n", acusado);
        printf("   🎉 Parabéns, detetive! O culpado foi capturado!\n");
    } else if (quantidadePistas == 1) {
        printf("⚠️  EVIDÊNCIAS INSUFICIENTES!\n\n");
        printf("   Apenas 1 pista aponta para %s.\n", acusado);
        printf("   São necessárias pelo menos 2 pistas para\n");
        printf("   uma acusação conclusiva.\n\n");
        printf("   O caso permanece em aberto...\n");
    } else {
        printf("❌ ACUSAÇÃO INCORRETA!\n\n");
        printf("   Nenhuma pista aponta para %s.\n", acusado);
        printf("   Revise as evidências com mais atenção.\n\n");
        printf("   O verdadeiro culpado permanece livre...\n");
    }
}

/*
 * Função: liberarArvore
 * Descrição: Libera memória da árvore de salas
 */
void liberarArvore(Sala *raiz) {
    if (raiz == NULL) return;
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

/*
 * Função: liberarArvorePistas
 * Descrição: Libera memória da árvore de pistas
 */
void liberarArvorePistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    liberarArvorePistas(raiz->esquerda);
    liberarArvorePistas(raiz->direita);
    free(raiz);
}

/*
 * Função: liberarHash
 * Descrição: Libera memória da tabela hash
 */
void liberarHash(TabelaHash *hash) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        HashNode *atual = hash->tabela[i];
        while (atual != NULL) {
            HashNode *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
}

/*
 * Função: main
 * Descrição: Função principal que integra todos os sistemas
 */
int main() {
    printf("==============================================\n");
    printf("     DETECTIVE QUEST - ENIGMA STUDIOS\n");
    printf("          Capítulo Final\n");
    printf("==============================================\n");
    printf("\n🕵️  Uma mansão misteriosa...\n");
    printf("   Pistas escondidas...\n");
    printf("   E um culpado a ser desmascarado!\n\n");
    printf("   Sua missão: explorar, coletar evidências\n");
    printf("   e fazer justiça!\n");
    
    // Inicializa estruturas de dados
    PistaNode *arvorePistas = NULL;
    TabelaHash hash;
    inicializarHash(&hash);
    
    // Popula a tabela hash com associações pista-suspeito
    inserirNaHash(&hash, "Pegadas molhadas no tapete", "Jardineiro");
    inserirNaHash(&hash, "Faca desaparecida do bloco", "Cozinheiro");
    inserirNaHash(&hash, "Livro aberto sobre venenos", "Mordomo");
    inserirNaHash(&hash, "Frasco vazio de arsenico", "Mordomo");
    inserirNaHash(&hash, "Documento queimado parcialmente", "Advogado");
    inserirNaHash(&hash, "Carta ameacadora escondida", "Advogado");
    inserirNaHash(&hash, "Testamento adulterado", "Advogado");
    inserirNaHash(&hash, "Planta venenosa cultivada", "Jardineiro");
    
    // Cria o mapa da mansão
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *escritorio = criarSala("Escritorio");
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = escritorio;
    
    Sala *despensa = criarSala("Despensa");
    Sala *jardim = criarSala("Jardim");
    cozinha->esquerda = despensa;
    cozinha->direita = jardim;
    
    Sala *salaSecreta = criarSala("Sala Secreta");
    Sala *salaLeitura = criarSala("Sala de Leitura");
    biblioteca->esquerda = salaSecreta;
    biblioteca->direita = salaLeitura;
    
    Sala *cofre = criarSala("Cofre");
    escritorio->esquerda = cofre;
    
    Sala *estufa = criarSala("Estufa");
    jardim->direita = estufa;
    
    // Fase 1: Exploração
    explorarSalas(hall, &arvorePistas);
    
    // Fase 2: Julgamento
    verificarSuspeitoFinal(arvorePistas, &hash);
    
    // Libera memória
    liberarArvore(hall);
    liberarArvorePistas(arvorePistas);
    liberarHash(&hash);
    
    printf("\n==============================================\n");
    printf("   Obrigado por jogar Detective Quest!\n");
    printf("==============================================\n");
    
    return 0;
}