#include "stdafx.h"
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <string.h> 

struct Graph {
    int n;
    int** adj;
};

struct Node {
    int val;
    struct Node* next;
};

struct AdjList {
    int n;
    struct Node** head;
};

struct Graph* create_graph(int n) {
    struct Graph* g = (struct Graph*)malloc(sizeof(struct Graph));
    g->n = n;
    g->adj = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) g->adj[i] = (int*)calloc(n, sizeof(int));
    return g;
}

void free_graph(struct Graph* g) {
    if (!g) return;
    for (int i = 0; i < g->n; i++) free(g->adj[i]);
    free(g->adj);
    free(g);
}

void print_adj(struct Graph* g) {
    printf("Матрица смежности (%d x %d):\n     ", g->n, g->n);
    for (int j = 0; j < g->n; j++) printf("v%-2d ", j + 1);
    printf("\n");
    for (int i = 0; i < g->n; i++) {
        printf("v%-3d ", i + 1);
        for (int j = 0; j < g->n; j++) printf("%-4d", g->adj[i][j]);
        printf("\n");
    }
}

struct Graph* generate_random_graph(int n, int prob) {
    struct Graph* g = create_graph(n);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (rand() % 100 < prob) g->adj[i][j] = g->adj[j][i] = 1;
        }
    }
    return g;
}

struct AdjList* create_list_graph(int n) {
    struct AdjList* l = (struct AdjList*)malloc(sizeof(struct AdjList));
    l->n = n;
    l->head = (struct Node**)calloc(n, sizeof(struct Node*));
    return l;
}

void free_list_graph(struct AdjList* l) {
    if (!l) return;
    for (int i = 0; i < l->n; i++) {
        struct Node* curr = l->head[i];
        while (curr) {
            struct Node* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(l->head);
    free(l);
}

void add_node(struct AdjList* l, int u, int v) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->val = v;
    node->next = NULL;
    if (!l->head[u]) l->head[u] = node;
    else {
        struct Node* curr = l->head[u];
        while (curr->next) curr = curr->next;
        curr->next = node;
    }
}

struct AdjList* convert_matrix_to_list(struct Graph* g) {
    struct AdjList* l = create_list_graph(g->n);
    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->n; j++) {
            if (g->adj[i][j]) add_node(l, i, j);
        }
    }
    return l;
}

void print_list(struct AdjList* l) {
    printf("Списки смежности (вершин: %d):\n", l->n);
    for (int i = 0; i < l->n; i++) {
        printf("  v%-2d: ", i + 1);
        struct Node* curr = l->head[i];
        if (!curr) printf("нет смежных вершин");
        while (curr) {
            printf("-> v%d ", curr->val + 1);
            curr = curr->next;
        }
        printf("\n");
    }
}

int is_isolated(struct Graph* g, int v) {
    for (int i = 0; i < g->n; i++) {
        if (g->adj[v][i] == 1) return 0; 
    }
    return 1; 
}

void dfs_matrix_rec(struct Graph* g, int v, int* num) {
    num[v] = 1;
    printf("%d ", v + 1);
    for (int i = 0; i < g->n; i++) {
        if (g->adj[v][i] == 1 && num[i] == 0) dfs_matrix_rec(g, i, num);
    }
}

void pog_matrix_rec(struct Graph* g, int start_v, int* num) {
    if (start_v >= 0 && start_v < g->n) dfs_matrix_rec(g, start_v, num);
    for (int v = 0; v < g->n; v++) {
        if (num[v] == 0) dfs_matrix_rec(g, v, num);
    }
}

void dfs_list_rec(struct AdjList* l, int v, int* num) {
    num[v] = 1;
    printf("%d ", v + 1);
    struct Node* curr = l->head[v];
    while (curr) {
        if (num[curr->val] == 0) dfs_list_rec(l, curr->val, num);
        curr = curr->next;
    }
}

void pog_list_rec(struct AdjList* l, int start_v, int* num) {
    if (start_v >= 0 && start_v < l->n) dfs_list_rec(l, start_v, num);
    for (int v = 0; v < l->n; v++) {
        if (num[v] == 0) dfs_list_rec(l, v, num);
    }
}

void dfs_matrix_stack(struct Graph* g, int start_v, int* num) {
    int* stack = (int*)malloc(g->n * sizeof(int)); 
    int top = -1;
    num[start_v] = 1;
    printf("%d ", start_v + 1);
    stack[++top] = start_v;

    while (top >= 0) {
        int v = stack[top];
        int found = 0;
        for (int i = 0; i < g->n; i++) {
            if (g->adj[v][i] == 1 && num[i] == 0) {
                num[i] = 1;
                printf("%d ", i + 1);
                stack[++top] = i;
                found = 1;
                break;
            }
        }
        if (!found) top--;
    }
    free(stack);
}

void pog_matrix_stack(struct Graph* g, int start_v, int* num) {
    if (start_v >= 0 && start_v < g->n) dfs_matrix_stack(g, start_v, num);
    for (int v = 0; v < g->n; v++) {
        if (num[v] == 0) dfs_matrix_stack(g, v, num);
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand((unsigned int)time(NULL));

    int n = 6, prob = 40;
    struct Graph* g = generate_random_graph(n, prob);
    struct AdjList* l = convert_matrix_to_list(g);
    
    int* num = (int*)calloc(n, sizeof(int));

    int choice;
    while (1) {
        printf("1. Задание 1 (матрица, рекурсивно)\n");
        printf("2. Задание 1 (3*) (списки смежности)\n");
        printf("3. Задание 2* (матрица, стек)\n");
        printf("4. Выполнить все задания\n");
        printf("5. Новый случайный граф\n");
        printf("6. Тестовый граф из методички\n");
        printf("0. Выход\n");
        printf("Выбор: ");

        if (scanf("%d", &choice) != 1) { while (getchar() != '\n'); continue; }
        if (choice == 0) break;

        if (choice >= 1 && choice <= 4) {
            int v = 1;
            int valid = 0;
            
            while (!valid) {
                printf("Стартовая вершина (1-%d): ", g->n);
                if (scanf("%d", &v) != 1 || v < 1 || v > g->n) {
                    while (getchar() != '\n');
                    v = 1;
                }
                if (is_isolated(g, v - 1)) {
                    printf("Вершина изолирована!\n", v);
                } else {
                    valid = 1;
                }
            }
            
            print_adj(g);
            print_list(l);

            clock_t start, end;
            double time_spent;

            if (choice == 1) {
                memset(num, 0, g->n * sizeof(int)); 
                printf("Обход (матрица, рекурсия): ");
                
                start = clock(); 
                pog_matrix_rec(g, v - 1, num);
                end = clock();   
                
                time_spent = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
                printf("\nВремя: %.4f мс\n", time_spent);
            } 
            else if (choice == 2) {
                memset(num, 0, g->n * sizeof(int));
                printf("Обход (списки, рекурсия): ");
                
                start = clock();
                pog_list_rec(l, v - 1, num);
                end = clock();
                
                time_spent = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
                printf("\nВремя: %.4f мс\n", time_spent);
            } 
            else if (choice == 3) {
                memset(num, 0, g->n * sizeof(int));
                printf("Обход (матрица, стек): ");
                
                start = clock();
                pog_matrix_stack(g, v - 1, num);
                end = clock();
                
                time_spent = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
                printf("\nВремя: %.4f мс\n", time_spent);
            } 
            else if (choice == 4) {       
                memset(num, 0, g->n * sizeof(int));
				printf("Обход (матрица, рекурсия): ");
                start = clock();
                pog_matrix_rec(g, v - 1, num);
                end = clock();
                printf("\nВремя: %.4f мс\n", ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0);

                memset(num, 0, g->n * sizeof(int));
				printf("Обход (списки, рекурсия): ");
                start = clock();
                pog_list_rec(l, v - 1, num);
                end = clock();
                printf("\nВремя: %.4f мс\n", ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0);

                memset(num, 0, g->n * sizeof(int));
				printf("Обход (матрица, стек): ");
                start = clock();
                pog_matrix_stack(g, v - 1, num);
                end = clock();
                printf("\nВремя: %.4f мс\n", ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0);
            }
        }
        else if (choice == 5) {
            printf("Количество вершин: ");
            if (scanf("%d", &n) != 1 || n < 1) { while (getchar() != '\n'); n = 6; }
            printf("Вероятность (1-100%%): ");
            if (scanf("%d", &prob) != 1 || prob < 1 || prob > 100) { while (getchar() != '\n'); prob = 40; }

            free_graph(g); free_list_graph(l); free(num);
            
            g = generate_random_graph(n, prob);
            l = convert_matrix_to_list(g);
            num = (int*)calloc(n, sizeof(int)); 
            
            print_adj(g);
        }
        else if (choice == 6) {
            free_graph(g); free_list_graph(l); free(num);
            g = create_graph(6);
            g->adj[0][1] = g->adj[1][0] = 1; g->adj[0][2] = g->adj[2][0] = 1;
            g->adj[0][5] = g->adj[5][0] = 1; g->adj[1][3] = g->adj[3][1] = 1;
            g->adj[2][4] = g->adj[4][2] = 1;
            l = convert_matrix_to_list(g);
            num = (int*)calloc(6, sizeof(int));
            
            print_adj(g); print_list(l);
            
            memset(num, 0, 6 * sizeof(int));
            printf("Обход (матрица, рекурсия): "); pog_matrix_rec(g, 0, num); printf("\n");
            memset(num, 0, 6 * sizeof(int));
            printf("Обход (списки, рекурсия): "); pog_list_rec(l, 0, num); printf("\n");
            memset(num, 0, 6 * sizeof(int));
            printf("Обход (матрица, стек): "); pog_matrix_stack(g, 0, num); printf("\n");
        }
    }

    free_graph(g); free_list_graph(l); free(num);
    return 0;
}