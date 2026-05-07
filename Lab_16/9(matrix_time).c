#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

int **a, **b, **c;
int n;

void* multiply_part(void* arg) {
    int *params = (int*)arg;
    int start_row = params[0];
    int end_row = params[1];

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += a[i][k] * b[k][j];
            }
            c[i][j] = sum;
        }
    }

    free(params);
    return NULL;
}

int** create_matrix(int n) {
    int **m = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
        m[i] = (int*)calloc(n, sizeof(int));
    return m;
}

void free_matrix(int n, int **m) {
    for (int i = 0; i < n; i++) free(m[i]);
    free(m);
}

long long current_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void multiply_matrix(int threads_count, int *rows_per_thread, int *remainder) {
    *rows_per_thread = n / threads_count;
    *remainder = n % threads_count;

    pthread_t *threads = malloc(threads_count * sizeof(pthread_t));

    for (int t = 0; t < threads_count; t++) {
        int *params = malloc(2 * sizeof(int));
        params[0] = t * (*rows_per_thread);
        params[1] = (t + 1) * (*rows_per_thread);
        pthread_create(&threads[t], NULL, multiply_part, params);
    }

    if (*remainder > 0) {
        int start_row = threads_count * (*rows_per_thread);
        for (int i = start_row; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int sum = 0;
                for (int k = 0; k < n; k++) {
                    sum += a[i][k] * b[k][j];
                }
                c[i][j] = sum;
            }
        }
    }

    for (int t = 0; t < threads_count; t++) {
        pthread_join(threads[t], NULL);
    }

    free(threads);
}

int main() {
    int sizes[] = {250, 500, 750, 1000, 1250, 1500, 1750, 2000, 2250, 2500};
    int threads_list[] = {1, 2, 4, 8, 16, 32, 64, 128};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int num_threads = sizeof(threads_list) / sizeof(threads_list[0]);

    printf("N\t");
    for (int t = 0; t < num_threads; t++) {
        printf("%d thr\t", threads_list[t]);
    }
    printf("\n");

    for (int s = 0; s < num_sizes; s++) {
        n = sizes[s];
        printf("%d\t", n);

        a = create_matrix(n);
        b = create_matrix(n);
        c = create_matrix(n);

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                a[i][j] = b[i][j] = 1;

        for (int t = 0; t < num_threads; t++) {
            int threads = threads_list[t];

            long long start = current_time_ms();

            int rows_per_thread, remainder;
            multiply_matrix(threads, &rows_per_thread, &remainder);

            long long end = current_time_ms();
            printf("%lld\t", end - start);
        }
        printf("\n");

        free_matrix(n, a);
        free_matrix(n, b);
        free_matrix(n, c);
    }

    return 0;
}
