#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *child_thread(void *arg) {
    char **strings = (char**)arg;

    for (int i = 0; i < 5; i++) {
        printf("child %s\n", strings[i]);
        sleep(1);
    }

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

    sleep(2);

    printf("Завершение дочерних потоков\n");
    for (int i = 0; i < 4; i++) {
        pthread_cancel(threads[i]);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 1; i <= 5; i++) {
        printf("parent %d\n", i);
        sleep(1);
    }

    return 0;
}
