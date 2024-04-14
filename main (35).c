#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura do nó da Árvore 2-3
struct no23 {
    int chave_esq; // chave esquerda
    int chave_dir; // chave direita
    struct no23 *esq; // subárvore esquerda
    struct no23 *meio; // subárvore do meio
    struct no23 *dir; // subárvore direita
    int n; // número de chaves no nó
};

// Definição do tipo arvore23 como um ponteiro para a estrutura do nó
typedef struct no23* arvore23;

// Função para adicionar uma chave em um nó da Árvore 2-3
void adicionaChave(arvore23 r, int chave, arvore23 p) {
    if (r->chave_esq < chave) {
        r->chave_dir = chave;
        r->dir = p;
    } else {
        r->chave_dir = r->chave_esq;
        r->chave_esq = chave;
        r->dir = r->meio;
        r->meio = p;
    }
    r->n = 2;
}

// Função para criar um novo nó da Árvore 2-3
arvore23 criaNo23(int chave_esq, int chave_dir, arvore23 esq, arvore23 meio, arvore23 dir, int n) {
    arvore23 novoNo = (arvore23)malloc(sizeof(struct no23));
    if (novoNo != NULL) {
        novoNo->chave_esq = chave_esq;
        novoNo->chave_dir = chave_dir;
        novoNo->esq = esq;
        novoNo->meio = meio;
        novoNo->dir = dir;
        novoNo->n = n;
    }
    return novoNo;
}
// Função para verificar se um nó é uma folha
int eh_folha(arvore23 r) {
    return (r->esq == NULL && r->meio == NULL && r->dir == NULL);
}

// Função para dividir um nó da Árvore 2-3
arvore23 split(arvore23 p, int chave, arvore23 subarvore, int *chave_promovida) {
    arvore23 paux;
    if (chave > p->chave_dir) {
        *chave_promovida = p->chave_dir;
        paux = p->dir;
        p->dir = NULL;
        p->n = 1;
        return criaNo23(chave, 0, paux, subarvore, NULL, 1);
    }
    if (chave >= p->chave_esq) {
        *chave_promovida = chave;
        paux = p->dir;
        p->dir = NULL;
        p->n = 1;
        return criaNo23(p->chave_dir, 0, subarvore, paux, NULL, 1);
    }
    *chave_promovida = p->chave_esq;
    paux = criaNo23(p->chave_dir, 0, p->meio, p->dir, NULL, 1);
    p->chave_esq = chave;
    p->n = 1;
    p->dir = NULL;
    p->meio = subarvore;
    return paux;
}


// Função para buscar um nó na Árvore 2-3
arvore23 busca(arvore23 r, int chave) {
    if (r == NULL) return NULL;
    if (r->chave_esq == chave) return r;
    if (r->n == 2 && r->chave_dir == chave) return r;
    if (chave < r->chave_esq) return busca(r->esq, chave);
    else if (r->n == 1) return busca(r->meio, chave);
    else if (chave < r->chave_dir) return busca(r->meio, chave);
    else return busca(r->dir, chave);
}

// Função auxiliar para inserção de um nó na Árvore 2-3
arvore23 inserir_aux(arvore23 r, int chave, int* chave_promovida) {
    if (eh_folha(r)) {
        if (r->n == 1) {
            adicionaChave(r, chave, NULL);
            return NULL;
        } else {
            return split(r, chave, NULL, chave_promovida);
        }
    } else {
        arvore23 paux;
        int ch_aux;
        if (chave < r->chave_esq) {
            paux = inserir_aux(r->esq, chave, &ch_aux);
        } else if ((r->n == 1) || (chave < r->chave_dir)) {
            paux = inserir_aux(r->meio, chave, &ch_aux);
        } else {
            paux = inserir_aux(r->dir, chave, &ch_aux);
        }
        if (paux == NULL) return NULL;
        if (r->n == 1) {
            adicionaChave(r, ch_aux, paux);
            return NULL;
        } else {
            return split(r, ch_aux, paux, chave_promovida);
        }
    }
}

// Função para inserir um nó na Árvore 2-3
arvore23 inserir(arvore23 r, int chave) {
    if (r == NULL) return criaNo23(chave, 0, NULL, NULL, NULL, 1);
    else {
        int chave_promovida;
        arvore23 aux = inserir_aux(r, chave, &chave_promovida);
        if (aux != NULL) return criaNo23(chave_promovida, 0, r, aux, NULL, 1);
        else return r;
    }
}



// Função para imprimir a Árvore 2-3 em ordem
void inOrder(arvore23 r) {
    if (r) {
        inOrder(r->esq);
        printf("%d ", r->chave_esq);
        inOrder(r->meio);
        if (r->n == 2) printf("%d ", r->chave_dir);
        inOrder(r->dir);
    }
}

// Função para imprimir a Árvore 2-3 em pré-ordem
void preOrdem(arvore23 raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->chave_esq);
        preOrdem(raiz->esq);
        preOrdem(raiz->meio);
        preOrdem(raiz->dir);
    }
}

// Função para imprimir a Árvore 2-3 em pós-ordem
void posOrdem(arvore23 raiz) {
    if (raiz != NULL) {
        posOrdem(raiz->esq);
        posOrdem(raiz->meio);
        if (raiz->n == 2) {
            posOrdem(raiz->dir);
            printf("%d ", raiz->chave_dir);
        }
        printf("%d ", raiz->chave_esq);
    }
}

// Função para encontrar o valor máximo em uma Árvore 2-3
int maximoelemento(arvore23 raiz) {
    if (raiz == NULL) return -1;
    while (raiz->dir != NULL) raiz = raiz->dir;
    if (raiz->n == 2) return raiz->chave_dir;
    else return raiz->chave_esq;
}

// Função para encontrar o valor mínimo em uma Árvore 2-3
int minimo(arvore23 raiz) {
    if (raiz == NULL) return -1;
    while (raiz->esq != NULL) raiz = raiz->esq;
    return raiz->chave_esq;
}

// Função para calcular a altura de uma Árvore 2-3
int altura(arvore23 raiz) {
    if (raiz == NULL) return 0;
    if (raiz->esq == NULL && raiz->meio == NULL && raiz->dir == NULL) return 1;
    int altura_esq = 0, altura_meio = 0, altura_dir = 0;
    if (raiz->esq != NULL) altura_esq = altura(raiz->esq);
    if (raiz->meio != NULL) altura_meio = altura(raiz->meio);
    if (raiz->dir != NULL) altura_dir = altura(raiz->dir);
    return 1 + maximo(altura_esq, altura_meio, altura_dir);
}

// Função auxiliar para retornar o máximo entre três números
int maximo(int a, int b, int c) {
    int max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    return max;
}

// Função para somar todos os elementos de uma Árvore 2-3
int soma_elementos(arvore23 raiz) {
    if (raiz == NULL) return 0;
    if (raiz->esq == NULL && raiz->meio == NULL && raiz->dir == NULL) return raiz->chave_esq + raiz->chave_dir;
    int soma_esq = 0, soma_meio = 0, soma_dir = 0;
    if (raiz->esq != NULL) soma_esq = soma_elementos(raiz->esq);
    if (raiz->meio != NULL) soma_meio = soma_elementos(raiz->meio);
    if (raiz->dir != NULL) soma_dir = soma_elementos(raiz->dir);
    return raiz->chave_esq + raiz->chave_dir + soma_esq + soma_meio + soma_dir;
}

// Função para contar o número de nós em uma Árvore 2-3
int num_nos(arvore23 raiz) {
    if (raiz == NULL) return 0;
    if (raiz->esq == NULL && raiz->meio == NULL && raiz->dir == NULL) return 1;
    int nos_esq = 0, nos_meio = 0, nos_dir = 0;
    if (raiz->esq != NULL) nos_esq = num_nos(raiz->esq);
    if (raiz->meio != NULL) nos_meio = num_nos(raiz->meio);
    if (raiz->dir != NULL) nos_dir = num_nos(raiz->dir);
    return 1 + nos_esq + nos_meio + nos_dir;
}

// Função para buscar um nó específico na Árvore 2-3
arvore23 busca_no(arvore23 raiz, int numero) {
    if (raiz == NULL) return NULL;
    if (numero == raiz->chave_esq || numero == raiz->chave_dir) return raiz;
    if (numero < raiz->chave_esq) return busca_no(raiz->esq, numero);
    else if (numero > raiz->chave_dir && raiz->dir != NULL) return busca_no(raiz->dir, numero);
    else if (raiz->meio != NULL) return busca_no(raiz->meio, numero);
    return NULL;
}

// Função para imprimir todos os números contidos em um intervalo dado
void imprimir_intervalo(arvore23 r, int a, int b) {
    if (r) {
        if (a < r->chave_esq) imprimir_intervalo(r->esq, a, b);
        if (a <= r->chave_esq && r->chave_esq <= b) printf("%d ", r->chave_esq);
        if (a <= r->chave_dir && r->chave_dir <= b) printf("%d ", r->chave_dir);
        if (b > r->chave_dir) imprimir_intervalo(r->dir, a, b);
    }
}

// Função para calcular a soma de todos os números contidos em um intervalo dado
int soma_intervalo(arvore23 raiz, int inicio, int fim) {
    if (raiz == NULL) return 0;
    int soma = 0;
    if (inicio < raiz->chave_esq && raiz->esq != NULL) soma += soma_intervalo(raiz->esq, inicio, fim);
    if (inicio <= raiz->chave_esq && raiz->chave_esq <= fim) soma += raiz->chave_esq;
    if (inicio <= raiz->chave_dir && raiz->chave_dir <= fim) soma += raiz->chave_dir;
    if (fim > raiz->chave_dir && raiz->dir != NULL) soma += soma_intervalo(raiz->dir, inicio, fim);
    return soma;
}

// Função para contar a quantidade de folhas em uma Árvore 2-3
int qde_folhas(arvore23 raiz) {
    if (raiz == NULL) return 0;
    if (raiz->esq == NULL && raiz->meio == NULL && raiz->dir == NULL) return 1;
    return qde_folhas(raiz->esq) + qde_folhas(raiz->meio) + qde_folhas(raiz->dir);
}

// Função para encontrar a chave seguinte ao número dado em uma Árvore 2-3
int chave_seguinte(arvore23 raiz, int numero) {
    if (raiz == NULL) return -1;
    arvore23 no = busca_no(raiz, numero);
    if (no == NULL) return -1;
    if (no->chave_dir == numero && no->dir != NULL) {
        arvore23 temp = no->dir;
        while (temp->esq != NULL) temp = temp->esq;
        return temp->chave_esq;
    }
    return -1;
}

// Função para encontrar a chave anterior ao número dado em uma Árvore 2-3
int chave_anterior(arvore23 raiz, int numero) {
    if (raiz == NULL) return -1;
    arvore23 no = busca_no(raiz, numero);
    if (no == NULL) return -1;
    if (no->chave_esq == numero && no->esq != NULL) {
        arvore23 temp = no->esq;
        while (temp->dir != NULL) temp = temp->dir;
        return temp->chave_dir;
    }
    return -1;
}

int main() {
    // Criação da Árvore 2-3 vazia
    arvore23 raiz = NULL;

    // Inserção de elementos na Árvore 2-3
    raiz = inserir(raiz, 10);
    raiz = inserir(raiz, 20);
    raiz = inserir(raiz, 5);
    raiz = inserir(raiz, 30);
    raiz = inserir(raiz, 25);

    // Impressão da Árvore 2-3 em ordem
    printf("Árvore 2-3 em ordem: ");
    inOrder(raiz);
    printf("\n");

    // Impressão da Árvore 2-3 em pré-ordem
    printf("Árvore 2-3 em pré-ordem: ");
    preOrdem(raiz);
    printf("\n");

    // Impressão da Árvore 2-3 em pós-ordem
    printf("Árvore 2-3 em pós-ordem: ");
    posOrdem(raiz);
    printf("\n");

    // Busca de elementos na Árvore 2-3
    int chave_busca = 20;
    arvore23 encontrado = busca(raiz, chave_busca);
    if (encontrado != NULL) {
        printf("Chave %d encontrada na Árvore 2-3.\n", chave_busca);
    } else {
        printf("Chave %d não encontrada na Árvore 2-3.\n", chave_busca);
    }

    // Exemplo de outras operações com a Árvore 2-3
    printf("Máximo na Árvore 2-3: %d\n", maximoelemento(raiz));
    printf("Mínimo na Árvore 2-3: %d\n", minimo(raiz));
    printf("Altura da Árvore 2-3: %d\n", altura(raiz));
    printf("Soma dos elementos na Árvore 2-3: %d\n", soma_elementos(raiz));
    printf("Número de nós na Árvore 2-3: %d\n", num_nos(raiz));
    printf("Quantidade de folhas na Árvore 2-3: %d\n", qde_folhas(raiz));

    // Operações adicionais
    printf("Chave seguinte a %d: %d\n", chave_busca, chave_seguinte(raiz, chave_busca));
    printf("Chave anterior a %d: %d\n", chave_busca, chave_anterior(raiz, chave_busca));

    // Intervalo de busca e soma
    int inicio = 5, fim = 25;
    printf("Intervalo [%d, %d] na Árvore 2-3: ", inicio, fim);
    imprimir_intervalo(raiz, inicio, fim);
    printf("\n");
    printf("Soma dos elementos no intervalo [%d, %d]: %d\n", inicio, fim, soma_intervalo(raiz, inicio, fim));

    // Liberação da memória alocada
    free(raiz);

    return 0;
}