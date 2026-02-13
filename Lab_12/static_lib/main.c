#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "task.h"

#define N 10


int main() {
    srand(time(NULL));

    task tasks[N];

    add_tasks(tasks, N);

    print_tasks(tasks, N);
    printf("\n");
    sort_tasks(tasks, N);
    print_tasks(tasks, N);

    return 0;
}
