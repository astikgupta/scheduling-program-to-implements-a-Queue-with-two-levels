#include <stdio.h>
#include <stdlib.h>

// Process structure
struct Process {
    int process_id;
    int priority;
    int burst_time;
};

// Queue structure
struct Queue {
    struct Process *array;
    int front;
    int rear;
    int capacity;
};

// Create a new queue
struct Queue* createQueue(int capacity) {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->array = (struct Process*)malloc(capacity * sizeof(struct Process));
    queue->front = queue->rear = -1;
    queue->capacity = capacity;
    return queue;
}

// Enqueue a process into the queue
void enqueue(struct Queue* queue, struct Process process) {
    if (queue->rear == queue->capacity - 1) {
        printf("Queue is full. Process %d cannot be enqueued.\n", process.process_id);
    } else {
        if (queue->front == -1) {
            queue->front = 0;
        }
        queue->rear++;
        queue->array[queue->rear] = process;
    }
}

// Dequeue a process from the queue
struct Process dequeue(struct Queue* queue) {
    struct Process process;
    if (queue->front == -1) {
        process.process_id = -1;
        process.priority = -1;
        process.burst_time = -1;
    } else {
        process = queue->array[queue->front];
        if (queue->front == queue->rear) {
            queue->front = queue->rear = -1;
        } else {
            queue->front++;
        }
    }
    return process;
}

// Function to execute queues
void executeQueues(struct Queue* queue1, struct Queue* queue2) {
    int quantum = 2; // Quantum time for Round Robin scheduling
    while (queue1->front != -1 || queue2->front != -1) {
        struct Process currentProcess;
        if (queue1->front != -1) {
            currentProcess = dequeue(queue1);
        } else {
            currentProcess = dequeue(queue2);
        }

        printf("Executing Process %d from Queue %d\n", currentProcess.process_id, currentProcess.priority + 1);

        // Execute the process for quantum time
        if (currentProcess.burst_time > quantum) {
            currentProcess.burst_time -= quantum;
            enqueue(queue2, currentProcess); // Move the process to the lower priority queue
        } else {
            printf("Process %d execution complete.\n", currentProcess.process_id);
        }
    }
}

int main() {
    int num_processes;
    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);

    // Create queues for Queue 1 and Queue 2
    struct Queue* queue1 = createQueue(num_processes);
    struct Queue* queue2 = createQueue(num_processes);

    // Input process details
    for (int i = 0; i < num_processes; i++) {
        struct Process process;
        printf("Enter details for Process %d (Priority BurstTime): ", i + 1);
        scanf("%d %d", &process.priority, &process.burst_time);
        process.process_id = i + 1;

        if (process.priority == 0) {
            enqueue(queue1, process);
        } else {
            enqueue(queue2, process);
        }
    }

    // Execute the queues
    executeQueues(queue1, queue2);

    // Free allocated memory
    free(queue1->array);
    free(queue1);
    free(queue2->array);
    free(queue2);

    return 0;
}