#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TASKS 50
#define MAX_LENGTH 100
#define MAX_FOLDERS 20

typedef struct {
    char description[MAX_LENGTH];
    char deadline[20];  // Format: YYYY-MM-DD
    int completed;
    time_t deadline_time; // For sorting
} Task;

typedef struct {
    char name[MAX_LENGTH];
    Task tasks[MAX_TASKS];
    int task_count;
} Folder;

Folder folders[MAX_FOLDERS];
int folder_count = 0;
int current_folder = -1;  // Index of currently selected folder

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

// Convert date string to time_t for sorting
time_t parse_date(const char *date_str) {
    struct tm tm = {0};
    if (strlen(date_str) == 10 && 
        sscanf(date_str, "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday) == 3) {
        tm.tm_year -= 1900; // Years since 1900
        tm.tm_mon -= 1;     // Months are 0-11
        return mktime(&tm);
    }
    return 0; // Invalid date
}

// Compare function for sorting tasks by deadline
int compare_tasks(const void *a, const void *b) {
    const Task *taskA = (const Task *)a;
    const Task *taskB = (const Task *)b;
    
    // Completed tasks go to the bottom
    if (taskA->completed && !taskB->completed) return 1;
    if (!taskA->completed && taskB->completed) return -1;
    
    // Sort by deadline time
    if (taskA->deadline_time > taskB->deadline_time) return 1;
    if (taskA->deadline_time < taskB->deadline_time) return -1;
    
    return 0;
}

// Sort tasks in a folder by deadline
void sort_tasks(Folder *folder) {
    // Update deadline_time for all tasks
    for (int i = 0; i < folder->task_count; i++) {
        folder->tasks[i].deadline_time = parse_date(folder->tasks[i].deadline);
    }
    
    // Sort using qsort
    qsort(folder->tasks, folder->task_count, sizeof(Task), compare_tasks);
}

void display_banner() {
    printf("\n=== TO-DO LIST MANAGER (C + Assembly) ===\n");
    if (current_folder != -1) {
        printf("Current List: %s (%d tasks)\n", folders[current_folder].name, folders[current_folder].task_count);
    } else {
        printf("No list selected - Please select or create a list first\n");
    }
    printf("\n");
}

void create_folder() {
    if (folder_count >= MAX_FOLDERS) {
        printf("Maximum number of lists reached!\n");
        return;
    }

    printf("Enter list name: ");
    getchar();
    fgets(folders[folder_count].name, MAX_LENGTH, stdin);
    folders[folder_count].name[strcspn(folders[folder_count].name, "\n")] = 0;
    
    folders[folder_count].task_count = 0;
    folder_count = asm_increment(folder_count);
    
    printf("List '%s' created successfully!\n", folders[folder_count-1].name);
    
    // Auto-select the newly created folder
    current_folder = folder_count - 1;
    printf("Automatically selected: %s\n", folders[current_folder].name);
}

void view_folders() {
    if (folder_count == 0) {
        printf("No lists available. Create a list first!\n");
        return;
    }

    printf("\nAvailable Lists:\n");
    printf("------------------------------------------\n");
    for (int i = 0; i < folder_count; i++) {
        char marker = (i == current_folder) ? '>' : ' ';
        printf("%c %d. %s (%d tasks)\n", marker, i + 1, folders[i].name, folders[i].task_count);
    }
    printf("------------------------------------------\n");
}

void select_folder() {
    view_folders();
    if (folder_count == 0) return;

    int num;
    printf("\nEnter list number to select: ");
    scanf("%d", &num);

    if (num < 1 || num > folder_count) {
        printf("Invalid list number!\n");
        return;
    }

    current_folder = num - 1;
    printf("Selected list: %s\n", folders[current_folder].name);
}

void delete_folder() {
    view_folders();
    if (folder_count == 0) return;

    int num;
    printf("\nEnter list number to delete: ");
    scanf("%d", &num);

    if (num < 1 || num > folder_count) {
        printf("Invalid list number!\n");
        return;
    }

    printf("Deleted list: %s\n", folders[num - 1].name);

    // Shift folders to remove the deleted one
    for (int i = num - 1; i < folder_count - 1; i++) {
        folders[i] = folders[i + 1];
    }

    folder_count = asm_subtract(folder_count, 1);
    
    // Reset current folder if it was deleted
    if (current_folder == num - 1) {
        current_folder = -1;
        printf("No list selected. Please select or create a new list.\n");
    } else if (current_folder > num - 1) {
        current_folder = asm_subtract(current_folder, 1);
    }
}

void add_task() {
    if (current_folder == -1) {
        printf("Please select or create a list first!\n");
        return;
    }

    Folder *current = &folders[current_folder];
    
    if (current->task_count >= MAX_TASKS) {
        printf("Task list is full!\n");
        return;
    }

    printf("Enter task description: ");
    getchar();
    fgets(current->tasks[current->task_count].description, MAX_LENGTH, stdin);
    current->tasks[current->task_count].description[strcspn(current->tasks[current->task_count].description, "\n")] = 0;

    printf("Enter deadline (YYYY-MM-DD format): ");
    fgets(current->tasks[current->task_count].deadline, sizeof(current->tasks[current->task_count].deadline), stdin);
    current->tasks[current->task_count].deadline[strcspn(current->tasks[current->task_count].deadline, "\n")] = 0;

    // Validate date format
    if (strlen(current->tasks[current->task_count].deadline) != 10 ||
        current->tasks[current->task_count].deadline[4] != '-' ||
        current->tasks[current->task_count].deadline[7] != '-') {
        printf("Warning: Date format should be YYYY-MM-DD. Task will still be added.\n");
    }

    current->tasks[current->task_count].completed = 0;
    current->tasks[current->task_count].deadline_time = parse_date(current->tasks[current->task_count].deadline);
    current->task_count = asm_increment(current->task_count);

    // Auto-sort after adding
    sort_tasks(current);
    
    printf("Task added successfully to '%s'!\n", current->name);
}

void view_tasks() {
    if (current_folder == -1) {
        printf("Please select or create a list first!\n");
        return;
    }

    Folder *current = &folders[current_folder];
    
    if (current->task_count == 0) {
        printf("No tasks in '%s' list.\n", current->name);
        return;
    }

    // Sort tasks before displaying
    sort_tasks(current);
    
    printf("\nTasks in '%s' (sorted by deadline):\n", current->name);
    printf("------------------------------------------\n");
    
    // Get current time for overdue comparison
    time_t now = time(NULL);
    
    for (int i = 0; i < current->task_count; i++) {
        char status = current->tasks[i].completed ? 'X' : ' ';
        char *overdue = "";
        
        // Check if task is overdue (not completed and deadline passed)
        if (!current->tasks[i].completed && current->tasks[i].deadline_time > 0 && 
            current->tasks[i].deadline_time < now) {
            overdue = " [OVERDUE]";
        }
        
        printf("%d. [%c] %s (Deadline: %s)%s\n", i + 1, status, 
               current->tasks[i].description, current->tasks[i].deadline, overdue);
    }
    printf("------------------------------------------\n");
}

void complete_task() {
    if (current_folder == -1) {
        printf("Please select or create a list first!\n");
        return;
    }

    Folder *current = &folders[current_folder];
    view_tasks();
    
    if (current->task_count == 0) return;

    int num;
    printf("\nEnter task number to mark as complete: ");
    scanf("%d", &num);

    if (num < 1 || num > current->task_count) {
        printf("Invalid task number!\n");
        return;
    }

    current->tasks[num - 1].completed = 1;
    
    // Re-sort after completion
    sort_tasks(current);
    
    printf("Task marked as complete!\n");
}

void delete_task() {
    if (current_folder == -1) {
        printf("Please select or create a list first!\n");
        return;
    }

    Folder *current = &folders[current_folder];
    view_tasks();
    
    if (current->task_count == 0) return;

    int num;
    printf("\nEnter task number to delete: ");
    scanf("%d", &num);

    if (num < 1 || num > current->task_count) {
        printf("Invalid task number!\n");
        return;
    }

    printf("Deleted: %s\n", current->tasks[num - 1].description);

    for (int i = num - 1; i < current->task_count - 1; i++) {
        current->tasks[i] = current->tasks[i + 1];
    }

    current->task_count = asm_subtract(current->task_count, 1);
}

void save_data() {
    FILE *file = fopen("todo_data.dat", "wb");
    if (file == NULL) {
        printf("Error: Could not save data to file!\n");
        return;
    }

    // Save folder_count first
    fwrite(&folder_count, sizeof(int), 1, file);
    
    // Save all folders and their tasks
    for (int i = 0; i < folder_count; i++) {
        // Save folder name and task_count
        fwrite(folders[i].name, sizeof(char), MAX_LENGTH, file);
        fwrite(&folders[i].task_count, sizeof(int), 1, file);
        
        // Save all tasks in this folder
        for (int j = 0; j < folders[i].task_count; j++) {
            fwrite(&folders[i].tasks[j], sizeof(Task), 1, file);
        }
    }
    
    // Save current_folder
    fwrite(&current_folder, sizeof(int), 1, file);
    
    fclose(file);
    printf("Data saved successfully to 'todo_data.dat'!\n");
}

void load_data() {
    FILE *file = fopen("todo_data.dat", "rb");
    if (file == NULL) {
        printf("No saved data found.\n");
        return;
    }

    // Load folder_count
    fread(&folder_count, sizeof(int), 1, file);
    
    // Load all folders and their tasks
    for (int i = 0; i < folder_count; i++) {
        // Load folder name and task_count
        fread(folders[i].name, sizeof(char), MAX_LENGTH, file);
        fread(&folders[i].task_count, sizeof(int), 1, file);
        
        // Load all tasks in this folder
        for (int j = 0; j < folders[i].task_count; j++) {
            fread(&folders[i].tasks[j], sizeof(Task), 1, file);
        }
    }
    
    // Load current_folder
    fread(&current_folder, sizeof(int), 1, file);
    
    fclose(file);
    printf("Data loaded successfully from 'todo_data.dat'!\n");
}

int main() {
    int choice;
    
    // Load data automatically at startup
    load_data();

    while (1) {
        display_banner();
        
        printf("MAIN MENU:\n");
        printf("1. Create New List\n");
        printf("2. View All Lists\n");
        printf("3. Select List\n");
        printf("4. Delete List\n");
        printf("5. Add Task to Current List\n");
        printf("6. View Tasks in Current List\n");
        printf("7. Complete Task\n");
        printf("8. Delete Task\n");
        printf("9. Save Data\n");
        printf("10. Load Data\n");
        printf("11. Exit\n");
        printf("-------------------------\n");
        printf("Choose an option (1-11): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: create_folder(); break;
            case 2: view_folders(); break;
            case 3: select_folder(); break;
            case 4: delete_folder(); break;
            case 5: add_task(); break;
            case 6: view_tasks(); break;
            case 7: complete_task(); break;
            case 8: delete_task(); break;
            case 9: save_data(); break;
            case 10: load_data(); break;
            case 11: 
                save_data();
                printf("Goodbye!\n"); 
                return 0;
            default: printf("Invalid choice! Please enter 1-11.\n");
        }
    }
}
