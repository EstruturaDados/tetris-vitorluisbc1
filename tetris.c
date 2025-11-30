#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// --------------------------------------
// Estrutura da Peça
// --------------------------------------
typedef struct {
    char nome; // I, O, T, L, S, Z, J
    int id;
} Peca;

// --------------------------------------
// Estruturas: Fila e Pilha
// --------------------------------------
typedef struct {
    Peca itens[MAX_FILA];
    int inicio;
    int fim;
    int tamanho;
} Fila;

typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// Histórico para desfazer
typedef struct {
    Peca ultimaJogada;
    int ocorreu;
} Historico;

// --------------------------------------
// Protótipos
// --------------------------------------
Peca gerarPeca();
void inicializarFila(Fila *f);
void exibirFila(const Fila *f);
void exibirPilha(const Pilha *p);

int enqueue(Fila *f, Peca p);
int dequeue(Fila *f, Peca *removida);

void inicializarPilha(Pilha *p);
int push(Pilha *p, Peca peca);
int pop(Pilha *p, Peca *removida);

// Nível Mestre
void trocarFrenteFilaTopoPilha(Fila *f, Pilha *p);
void desfazerJogada(Fila *f, Historico *h);
void inverterFilaEPilha(Fila *f, Pilha *p);

// --------------------------------------
// Função que gera novas peças automaticamente
// --------------------------------------
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'};
    Peca p;
    p.nome = tipos[rand() % 7];
    p.id = rand() % 10000;
    return p;
}

// --------------------------------------
// Inicialização da Fila
// --------------------------------------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;

    for (int i = 0; i < MAX_FILA; i++) {
        enqueue(f, gerarPeca());
    }
}

// --------------------------------------
// Exibir fila
// --------------------------------------
void exibirFila(const Fila *f) {
    printf("\nFila de Peças (frente -> fim):\n");

    int idx = f->inicio;
    for (int i = 0; i < f->tamanho; i++) {
        Peca p = f->itens[idx];
        printf("[%c | %d] ", p.nome, p.id);
        idx = (idx + 1) % MAX_FILA;
    }
    printf("\n");
}

// --------------------------------------
// Exibir pilha
// --------------------------------------
void exibirPilha(const Pilha *p) {
    printf("\nPilha de Reserva (topo -> base):\n");

    for (int i = p->topo - 1; i >= 0; i--) {
        printf("[%c | %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
}

// --------------------------------------
// Enqueue (fila circular)
// --------------------------------------
int enqueue(Fila *f, Peca p) {
    if (f->tamanho == MAX_FILA) return 0;

    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->tamanho++;
    return 1;
}

// --------------------------------------
// Dequeue (fila circular)
// --------------------------------------
int dequeue(Fila *f, Peca *removida) {
    if (f->tamanho == 0) return 0;

    *removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->tamanho--;
    return 1;
}

// --------------------------------------
// Inicializar pilha
// --------------------------------------
void inicializarPilha(Pilha *p) {
    p->topo = 0;
}

// --------------------------------------
// Push
// --------------------------------------
int push(Pilha *p, Peca peca) {
    if (p->topo == MAX_PILHA) return 0;
    p->itens[p->topo++] = peca;
    return 1;
}

// --------------------------------------
// Pop
// --------------------------------------
int pop(Pilha *p, Peca *removida) {
    if (p->topo == 0) return 0;
    *removida = p->itens[--p->topo];
    return 1;
}

// --------------------------------------
// Trocar frente da fila com topo da pilha
// --------------------------------------
void trocarFrenteFilaTopoPilha(Fila *f, Pilha *p) {
    if (f->tamanho == 0 || p->topo == 0) {
        printf("Impossivel trocar.\n");
        return;
    }

    int frente = f->inicio;
    int topo = p->topo - 1;

    Peca temp = f->itens[frente];
    f->itens[frente] = p->itens[topo];
    p->itens[topo] = temp;

    printf("Troca realizada com sucesso.\n");
}

// --------------------------------------
// Desfazer última jogada
// --------------------------------------
void desfazerJogada(Fila *f, Historico *h) {
    if (!h->ocorreu) {
        printf("Nenhuma jogada para desfazer.\n");
        return;
    }

    // Inserir a peça de volta no início da fila
    f->inicio = (f->inicio - 1 + MAX_FILA) % MAX_FILA;
    f->itens[f->inicio] = h->ultimaJogada;
    f->tamanho++;

    h->ocorreu = 0;

    printf("Ultima jogada desfeita.\n");
}

// --------------------------------------
// Inverter fila com pilha
// --------------------------------------
void inverterFilaEPilha(Fila *f, Pilha *p) {
    Peca tempFila[MAX_FILA];
    Peca tempPilha[MAX_PILHA];

    int tFila = f->tamanho;
    int tPilha = p->topo;

    int idx = f->inicio;
    for (int i = 0; i < tFila; i++) {
        tempFila[i] = f->itens[idx];
        idx = (idx + 1) % MAX_FILA;
    }

    for (int i = 0; i < tPilha; i++) {
        tempPilha[i] = p->itens[i];
    }

    for (int i = 0; i < tPilha; i++) {
        if (i < MAX_FILA) f->itens[i] = tempPilha[i];
    }

    for (int i = 0; i < tFila; i++) {
        if (i < MAX_PILHA) p->itens[i] = tempFila[i];
    }

    f->tamanho = tPilha;
    p->topo = tFila;

    f->inicio = 0;
    f->fim = tPilha % MAX_FILA;

    printf("Fila e pilha invertidas.\n");
}

// --------------------------------------
// MAIN
// --------------------------------------
int main() {
    srand( (unsigned) time(NULL) );

    Fila fila;
    Pilha pilha;
    Historico historico = {.ocorreu = 0};

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int opcao;

    do {
        printf("\n--- MENU TETRIS STACK ---\n");
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca\n");
        printf("3 - Usar peca reservada\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Desfazer ultima jogada\n");
        printf("6 - Inverter fila com pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        Peca aux;

        switch (opcao) {

        case 1:
            if (dequeue(&fila, &aux)) {
                historico.ultimaJogada = aux;
                historico.ocorreu = 1;
                enqueue(&fila, gerarPeca());
                printf("Peca jogada: %c | %d\n", aux.nome, aux.id);
            } else {
                printf("Fila vazia.\n");
            }
            break;

        case 2:
            if (dequeue(&fila, &aux)) {
                if (push(&pilha, aux)) {
                    enqueue(&fila, gerarPeca());
                    printf("Peca reservada.\n");
                } else {
                    printf("Pilha cheia. Nao foi possivel reservar.\n");
                    enqueue(&fila, aux);
                }
            }
            break;

        case 3:
            if (pop(&pilha, &aux)) {
                historico.ultimaJogada = aux;
                historico.ocorreu = 1;
                printf("Peca usada: %c | %d\n", aux.nome, aux.id);
            } else {
                printf("Pilha vazia.\n");
            }
            break;

        case 4:
            trocarFrenteFilaTopoPilha(&fila, &pilha);
            break;

        case 5:
            desfazerJogada(&fila, &historico);
            break;

        case 6:
            inverterFilaEPilha(&fila, &pilha);
            break;

        case 0:
            printf("Saindo...\n");
            break;

        default:
            printf("Opcao invalida.\n");
        }

        exibirFila(&fila);
        exibirPilha(&pilha);

    } while (opcao != 0);

    return 0;
}