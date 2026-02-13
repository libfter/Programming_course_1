#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "task.h"


void add_tasks(task *arr, int quantity) {
    /*ручной ввод
     *   for (int i = 0; i < quantity; i++) {
     *       task new_task;
     *       char buffer[20];
     *
     *       printf("введите текст задачи %d (64)- ", i);
     *       fgets(new_task.text, sizeof(new_task.text), stdin);
     *       new_task.text[strcspn(new_task.text, "\n")] = 0;
     *
     *       printf("введите приоритет задачи %d (1-3) - ", i);
     *       fgets(buffer, sizeof(buffer), stdin);
     *       new_task.priority = (int)strtol(buffer, NULL, 10);
     *
     *       printf("введите дедлайн задачи %d (дни) - ", i);
     *       fgets(buffer, sizeof(buffer), stdin);
     *       new_task.deadline = (int)strtol(buffer, NULL, 10);
     *
     *       printf("введите статус задачи %d (0-1) - ", i);
     *       fgets(buffer, sizeof(buffer), stdin);
     *       new_task.status = (int)strtol(buffer, NULL, 10);
     *
     *       arr[i] = new_task;
     *
     *       printf("\n");
}
*/
    for (int i = 0; i < quantity; i++) {
        task new_task;

        strcpy(new_task.text, "text");
        new_task.priority = rand() % 3 + 1;
        new_task.deadline = rand() % 30 + 1;
        new_task.status = rand() % 2;

        arr[i] = new_task;
    }
}

void sort_tasks(task *arr, int quantity) {
    int min;
    task temp;

    for (int i = 0; i < quantity-1; i++) {
        //find min num index
        min = i;
        for (int j = i+1; j < quantity; j++) {
            if (arr[min].deadline > arr[j].deadline) {
                min = j;
            }
        }

        //swap
        temp = arr[i];
        arr[i] = arr[min];
        arr[min] = temp;
    }
}


void print_tasks(task *arr, int quantity) {
    printf("text priority deadline status\n");
    for (int i = 0; i < quantity; i++) {
        printf("%s\t", arr[i].text);
        printf("%d\t", arr[i].priority);
        printf("%d\t", arr[i].deadline);
        printf("%d\n", arr[i].status);
    }
}
