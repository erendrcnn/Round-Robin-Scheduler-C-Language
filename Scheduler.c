#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// [Round-Robin Scheduler]

/* Maximum length of a string */
#define MAX_ID_LENGTH 100

/* Task structure */
typedef struct task {
    char id[MAX_ID_LENGTH];     // Task ID
    int init_runtime;           // Initial runtime
    int runtime;                // Remaining runtime
    int completion_time;        // Completion time
    int elapsed_time;           // Elapsed time
    int submission_time;        // Submission time
    struct task *next;          // Pointer to the next task
} task_t;

/* Queue structure */
typedef struct queue {
    task_t *head;               // Pointer to the head of the queue
    task_t *tail;               // Pointer to the tail of the queue
    int size;                   // Size of the queue
} queue_t;

/* Global variables */
int system_time = 0;
int total_idle_time = 0;
queue_t waiting_tasks_queue   = {NULL, NULL};
queue_t completed_tasks_queue = {NULL, NULL};

/* Function prototypes */
void enqueue(queue_t *q, task_t *t);
task_t *dequeue(queue_t *q);
void new_task(char *task_id, int runtime);
void simulate(int k);
void display_system_time();
void display_total_idle_time();
void display_recently_completed_task();
void dump_completed_tasks(char *filename);
void empty_completed_tasks_queue();
void cleanup();
void help();

/* Main Function */
int main()
{
    char command[MAX_ID_LENGTH];                        // Command string
    char task_id[MAX_ID_LENGTH];                        // Task ID string
    int runtime;                                        // Task runtime
    int k;                                              // Number of units of time to simulate
    char filename[MAX_ID_LENGTH];                       // Filename string

    while (1) {
        printf("\n> ");                                 // Prompt
        scanf("%s", command);                           // Read the command

        if (strcmp(command, "SNT") == 0) {              // New task
            scanf("%s %d", task_id, &runtime);
            if (runtime <= 0) {                         // Check if the runtime is greater than 0.
                printf("[WARNING] : Runtime must be greater than 0. Please enter again.\n");
            } else {                                    // Otherwise, create a new task.
                new_task(task_id, runtime);
                printf("Task '%s' is created with runtime %d.\n", task_id, runtime);
            }
        } else if (strcmp(command, "S") == 0) {         // Simulate
            scanf("%d", &k);
            if (k < 0) {                                // Check if units of time to simulate is greater than or equal to 0.
                printf("[WARNING] : Number of units of time to simulate must be greater than or equal to 0. Please enter again.\n");
            } else {
                simulate(k);
                printf("%d units of time simulated.\n", k);
            }
        } else if (strcmp(command, "DST") == 0) {       // Display system time
            display_system_time();
        } else if (strcmp(command, "DTIT") == 0) {      // Display total idle time
            display_total_idle_time();
        } else if (strcmp(command, "DIMRCT") == 0) {    // Display most recently completed task
            display_recently_completed_task();
        } else if (strcmp(command, "DCTQ") == 0) {      // Dump completed tasks queue
            scanf("%s", filename);
            dump_completed_tasks(filename);
            printf("Completed tasks queue is dumped to %s.\n", filename);
        } else if (strcmp(command, "ECTS") == 0) {      // Empty completed tasks queue
            empty_completed_tasks_queue();
            printf("Completed tasks queue is emptied.\n");
        } else if (strcmp(command, "QUIT") == 0) {      // Quit
            cleanup();
            break;
        } else {
            printf("\n[ERROR] : Invalid command\n");    // Invalid command
            help();
        }
    }

    return 0;
}

/* Enqueues a task into a queue */
void enqueue(queue_t *q, task_t *t)
{
    t->next = NULL;         // Set the next pointer of the task to NULL.
    
    if (q->tail == NULL) {  // If the queue is empty, set the head and tail to the task.
        q->head = t;        // Set the head to the task.
        q->tail = t;        // Set the tail to the task.
        q->size = 1;        // Set the size of the queue to 1.
    } else {                // Otherwise, add the task to the end of the queue.
        q->tail->next = t;  // Set the next pointer of the tail to the task.
        q->tail = t;        // Set the tail to the task.
        q->size++;          // Increment the size of the queue.
    }
}

/* Dequeues a task from a queue */
task_t *dequeue(queue_t *q)
{
    task_t *t = q->head;        // Get the task at the head of the queue.

    if (t != NULL) {            // If the queue is not empty, remove the task from the head of the queue.
        q->head = t->next;      // Set the head of the queue to the next task.
        q->size--;              // Decrement the size of the queue.
        if (q->head == NULL) {  // If the queue is empty, set the tail to NULL.
            q->tail = NULL;
        }
    }

    return t;
}

/* Starts a new task */
void new_task(char *task_id, int runtime)
{
    task_t *t = (task_t *) malloc(sizeof(task_t));  // Allocate memory for the task.
    strcpy(t->id, task_id);                         // Copy the task ID.
    t->runtime = runtime;                           // Set the runtime.
    t->init_runtime = runtime;                      // Set the initial runtime.
    t->submission_time = system_time;               // Set the submission time.
    t->completion_time = -1;                        // Set the completion time to -1.
    t->elapsed_time = 0;                            // Set the elapsed time to 0.

    enqueue(&waiting_tasks_queue, t);               // Enqueue the task into the waiting queue.
}

/* Simulates the system for k units of time */
void simulate(int k)
{
    int i;              // Loop counter
    int flag = 0;       // Flag to check if there are tasks in the waiting queue
    task_t *t;          // Task pointer

    for (i = 0; i < k; i++) {
        t = dequeue(&waiting_tasks_queue);              // Dequeue a task from the waiting queue.

        if (t != NULL) {                                // If there are tasks in the waiting queue, run them.
            system_time++;                              // Increment system time.

            t->runtime--;                               // Decrement the runtime of the task.
            t->completion_time = system_time + 1;       // Set the completion time of the task.
            t->elapsed_time = system_time - t->submission_time;   // Set the elapsed time of the task.

            if (t->runtime == 0) {
                enqueue(&completed_tasks_queue, t);     // Enqueue the task into the completed queue if it is completed.
            } else {
                enqueue(&waiting_tasks_queue, t);       // Enqueue the task into the waiting queue if it is not completed.
            } 
        } else {
            flag = 1;                                   // Set the flag to indicate that the waiting queue is empty.
        }
    }

    if (flag == 1) {                                    // If the waiting queue is empty, increment the total idle time.
        total_idle_time += 1;
    }
}

/* Displays the current system time */
void display_system_time()
{
    printf("System time: %d units\n", system_time);
}

/* Displays the total idle time */
void display_total_idle_time()
{
    printf("Total idle time: %d units\n", total_idle_time);
}

/* Displays the most recently completed task */
void display_recently_completed_task()
{
    task_t *t = completed_tasks_queue.tail;

    if (t != NULL) {
        printf("Most recently completed task: %s\n", t->id);
    } else {
        printf("No completed tasks\n");
    }
}

/* Dumps the completed tasks queue into a file */
void dump_completed_tasks(char *filename)
{
    FILE *fp;                                           // File pointer
    task_t *curr_task = completed_tasks_queue.head;     // Task pointer

    // Open the file for writing
    if ((fp = fopen(filename, "w")) == NULL) {
        printf("[ERROR] : Cannot open file \"%s\"\n", filename);
        return;
    }

    // Write the number of completed tasks to the file
    if (completed_tasks_queue.head != NULL)
        fprintf(fp, "Incremental order of completed tasks | TOTAL COMPLETED TASKS: %d\n\n", completed_tasks_queue.size);

    // Write the completed tasks to the file
    while (curr_task != NULL) {
        fprintf(fp, "Task Name          : %s\n",   curr_task->id);
        fprintf(fp, " - Initial Runtime : %d\n",   curr_task->init_runtime);
        fprintf(fp, " - Completion Time : %d\n",   curr_task->completion_time);
        fprintf(fp, " - Elapsed Time    : %d\n\n", curr_task->elapsed_time);
        curr_task = curr_task->next;
    }

    // Close the file
    fclose(fp);
}

/* Empties the completed tasks queue */
void empty_completed_tasks_queue()
{
    task_t *t;

    /* Frees all dynamically allocated memory */
    while ((t = dequeue(&completed_tasks_queue)) != NULL) {
        free(t);
    }
}

/* Frees all dynamically allocated memory */
void cleanup()
{
    empty_completed_tasks_queue();
}

/* Displays the list of available commands */
void help() {
    printf("====================================================\n");
    printf("  [Available commands]                              \n");
    printf("                                                    \n");
    printf("- SNT task_name runtime : Start a new task          \n");
    printf("- S k                   : Simulate k units          \n");
    printf("- DST                   : Display system time       \n");
    printf("- DTIT                  : Display total (idle) time \n");
    printf("- DIMRCT                : Display the most recently completed task\n");
    printf("- DCTQ file_name        : Dump completed tasks queue content into a text file\n");
    printf("- ECTS                  : Empty completed tasks queue\n");
    printf("- QUIT                  : Quit the program          \n");
    printf("====================================================\n");
}