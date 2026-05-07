#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *child_thread(void *arg) {
    for (int i = 1; i <= 5; i++) {
        printf("child %d\n", i);
        sleep(1);
    }

    return NULL;
}

int main() {
    pthread_t thread;
    int result;

    result = pthread_create(&thread, NULL, child_thread, NULL);

    if (result != 0) {
        printf("Ошибка создания потока\n");
        return 1;
    }

    for (int i = 1; i <= 5; i++) {
        printf("parent %d\n", i);
        sleep(1);
    }

    pthread_join(thread, NULL);

    return 0;
}
