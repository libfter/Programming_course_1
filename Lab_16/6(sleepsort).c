#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void *sleep_sort_thread(void *arg) {
    int data = *(int*)arg;

    usleep(data * 10000);

    printf("%d ", data);

    return NULL;
}


int main() {
    int array[] = {4, 1, 15, 2, 6, 8, 2, 5, 9, 7, 10, 3, 11};
    int n = sizeof(array) / sizeof(array[0]);
    pthread_t threads[50];

    printf("       Исходный: \t");
    for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    printf("Отсортированный:\t");
    for (int i = 0; i < n; i++) {
        int *value = malloc(sizeof(int));
        *value = array[i];

        pthread_create(&threads[i], NULL, sleep_sort_thread, value);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n");

    return 0;
}
