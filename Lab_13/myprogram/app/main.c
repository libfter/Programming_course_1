#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//для замера времени выполнения функции
#include <sys/time.h>
#include <unistd.h>

#include "task.h"


int main() {
    srand(time(NULL));

    int N[3] = { 100, 10000, 100000 };

    for (int i = 0; i < 3; i++) {
        task tasks[N[i]];
        add_tasks(tasks, N[i]);

        struct timeval start, end;

        // Замеряем время начала
        gettimeofday(&start, NULL);


        sort_tasks(tasks, N[i]);


        // Замеряем время окончания
        gettimeofday(&end, NULL);

        // Вычисляем разницу в микросекундах
        long seconds = end.tv_sec - start.tv_sec;
        long useconds = end.tv_usec - start.tv_usec;
        long mtime = (seconds * 1000000 + useconds);

        printf("Размер данных: %d\n", N[i]);
        printf("Время выполнения: %ld микросекунд (%.4f сек)\n\n", mtime, mtime / 1000000.0);
    }

    return 0;
}
