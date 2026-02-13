#ifndef _TASK_H_
#define _TASK_H_

typedef struct {
    char text[64];
    int priority;
    int deadline;
    int status;
} task;

void add_tasks(task *arr, int quantity);
void sort_tasks(task *arr, int quantity);
void print_tasks(task *arr, int quantity);

#endif //_TASK_H_
