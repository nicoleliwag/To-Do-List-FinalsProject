#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TASKS 50
#define MAX_LENGTH 100

typedef struct {
    char description[MAX_LENGTH];
    char deadline[20];  // Format: YYYY-MM-DD or any string
    int completed;
} Task;

Task tasks[MAX_TASKS];
int task_count = 0;

// Assembly function to add two numbers
int asm_add(int a, int b) {
    int result;
#if defined(_MSC_VER)
    __asm {
        mov eax, a
        add eax, b
        mov result, eax
    }
#elif defined(__GNUC__)
    __asm__ (
        "movl %1, %%eax\n\t"
        "addl %2, %%eax\n\t"
        "movl %%eax, %0\n\t"
        : "=r" (result)
        : "r" (a), "r" (b)
        : "%eax"
    );
#else
    result = a + b;
#endif
    return result;
}

// Assembly function to subtract two numbers
int asm_subtract(int a, int b) {
    int result;
#if defined(_MSC_VER)
    __asm {
        mov eax, a
        sub eax, b
        mov result, eax
    }
#elif defined(__GNUC__)
    __asm__ (
        "movl %1, %%eax\n\t"
        "subl %2, %%eax\n\t"
        "movl %%eax, %0\n\t"
        : "=r" (result)
        : "r" (a), "r" (b)
        : "%eax"
    );
#else
    result = a - b;
#endif
    return result;
}

// Assembly function to increment a number
int asm_increment(int a) {
    int result;
#if defined(_MSC_VER)
    __asm {
        mov eax, a
        inc eax
        mov result, eax
    }
#elif defined(__GNUC__)
    __asm__ (
        "movl %1, %%eax\n\t"
        "incl %%eax\n\t"
        "movl %%eax, %0\n\t"
        : "=r" (result)
        : "r" (a)
        : "%eax"
    );
#else
    result = a + 1;
#endif
    return result;
}

void display_banner() {
    printf("\n=== TO-DO LIST MANAGER (C + Assembly) ===\n\n");
}

void add_task() {
    if (task_count >= MAX_TASKS) {
        printf("Task list is full!\n");
        return;
    }

    printf("Enter task description: ");
    getchar();
    fgets(tasks[task_count].description, MAX_LENGTH, stdin);
    tasks[task_count].description[strcspn(tasks[task_count].description, "\n")] = 0;

    printf("Enter deadline (e.g., YYYY-MM-DD): ");
    fgets(tasks[task_count].deadline, sizeof(tasks[task_count].deadline), stdin);
    tasks[task_count].deadline[strcspn(tasks[task_count].deadline, "\n")] = 0;

    tasks[task_count].completed = 0;
    task_count = asm_increment(task_count);

    printf("Task added successfully!\n");
}

void view_tasks() {
    if (task_count == 0) {
        printf("No tasks in your list.\n");
        return;
    }

    printf("\nYour To-Do List:\n");
    printf("------------------------------------------\n");
    for (int i = 0; i < task_count; i++) {
        char status = tasks[i].completed ? 'X' : ' ';
        printf("%d. [%c] %s (Deadline: %s)\n", i + 1, status, tasks[i].description, tasks[i].deadline);
    }
    printf("------------------------------------------\n");
}

void complete_task() {
    view_tasks();
    if (task_count == 0) return;

    int num;
    printf("\nEnter task number to mark as complete: ");
    scanf("%d", &num);

    if (num < 1 || num > task_count) {
        printf("Invalid task number!\n");
        return;
    }

    tasks[num - 1].completed = 1;
    printf("Task marked as complete!\n");
}

void delete_task() {
    view_tasks();
    if (task_count == 0) return;

    int num;
    printf("\nEnter task number to delete: ");
    scanf("%d", &num);

    if (num < 1 || num > task_count) {
        printf("Invalid task number!\n");
        return;
    }

    printf("Deleted: %s\n", tasks[num - 1].description);

    for (int i = num - 1; i < task_count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }

    task_count = asm_subtract(task_count, 1);
}

int main() {
    int choice;
    display_banner();

    while (1) {
        printf("\nMENU OPTIONS:\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Complete Task\n");
        printf("4. Delete Task\n");
        printf("5. Exit\n");
        printf("-------------------------\n");
        printf("Choose an option (1-5): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_task(); break;
            case 2: view_tasks(); break;
            case 3: complete_task(); break;
            case 4: delete_task(); break;
            case 5: printf("Goodbye!\n"); return 0;
            default: printf("Invalid choice! Please enter 1-5.\n");
        }
    }
}
