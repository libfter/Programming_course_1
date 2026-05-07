#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int flag = 1;

int ** a, **b, **c;
int n;

void cleanup(void *arg) {
    int thread_id = *(int*)arg;
    printf("Поток %d завершен\n", thread_id);
}

void *child_thread(void *arg) {
    int *args = (int*)arg;
    int start = args[0];
    int end = args[1];

    for (int i = start; i < end; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += a[i][k] * b[k][j];
            }
            c[i][j] = sum;
        }
    }

    free(args);
    return NULL;
}

int **create_matrix(int n) {
    int **matrix = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrix[i] = malloc(n * sizeof(int));
    }

    return matrix;
}

void free_matrix(int n, int **m) {
    for (int i = 0; i < n; i++) {
        free(m[i]);
    }
    free(m);
}

void print_matrix(int n, int **m, char *name) {
    printf("%s:\n", name);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("usage: %s <N> <threads_count>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    int threads_count = atoi(argv[2]);
    if (threads_count > n) {
        threads_count = n;
    }

    a = create_matrix(n);
    b = create_matrix(n);
    c = create_matrix(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = 1;
            b[i][j] = 1;
        }
    }

    int rpt = n / threads_count;
    int remainder = n % threads_count;

    pthread_t *threads = malloc(threads_count * sizeof(pthread_t));
    int start = 0;

    for (int i = 0; i < threads_count; i++) {
        int rows = rpt + (i < remainder ? 1 : 0); //добавляем на 1 больше пока есть остаток

        int *args = malloc(2 * sizeof(int));
        args[0] = start;
        args[1] = start + rows;

        pthread_create(&threads[i], NULL, child_thread, args);

        start += rows;
    }

    for (int i = 0; i < threads_count; i++) {
        pthread_join(threads[i], NULL);
    }

    if (n < 5) {
        print_matrix(n, a, "A");
        print_matrix(n, b, "B");
        print_matrix(n, c, "C");
    }

    free_matrix(n, a);
    free_matrix(n, b);
    free_matrix(n, c);
    free(threads);

    return 0;
}
