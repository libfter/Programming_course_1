#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int flag = 1;

void cleanup(void *arg) {
    int thread_id = *(int*)arg;
    printf("Поток %d завершен\n", thread_id);
}

void *child_thread(void *arg) {
    char **strings = (char**)arg;
    int thread_id;

    sscanf(strings[0], "%d", &thread_id);
    pthread_cleanup_push(cleanup, &thread_id);

    for (int i = 0; i < 5; i++) {
        while (1) {
            pthread_mutex_lock(&mutex);

            if (flag == 0) {
                printf("child %s\n", strings[i]);
                flag = 1;
                pthread_mutex_unlock(&mutex);
                break;
            }

            pthread_mutex_unlock(&mutex);
            usleep(1000);
        }
        sleep(1);
    }

    pthread_cleanup_pop(0);
    return NULL;
}

int main() {
    pthread_t threads[4];
    int result;

    char *strings[4][5] = {
        {"1 1", "1 2", "1 3", "1 4", "1 5"},
        {"2 1", "2 2", "2 3", "2 4", "2 5"},
        {"3 1", "3 2", "3 3", "3 4", "3 5"},
        {"4 1", "4 2", "4 3", "4 4", "4 5"}
    };

    for (int i = 0; i < 4; i++) {
        result = pthread_create(&threads[i], NULL, child_thread, strings[i]);

        if (result != 0) {
            printf("Ошибка создания потока\n");
            return 1;
        }
    }

    for (int i = 1; i <= 4; i++) {
        while (1) {
            pthread_mutex_lock(&mutex);

            if (flag == 1) {
                printf("parent %d\n", i);
                flag = 0;
                pthread_mutex_unlock(&mutex);
                break;
            }

            pthread_mutex_unlock(&mutex);
            usleep(1000);
        }
        sleep(1);
    }

    sleep(2);

    printf("Завершение дочерних потоков\n");
    for (int i = 0; i < 4; i++) {
        pthread_cancel(threads[i]);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
