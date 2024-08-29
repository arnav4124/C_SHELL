#ifndef STRING_QUEUE_H
#define STRING_QUEUE_H

#define MAX_STRING_LENGTH 1024

// Node structure for the linked list
typedef struct Node {
    char data[MAX_STRING_LENGTH];
    struct Node* next;
} Node;

// Queue structure containing pointers to the front and rear of the linked list
typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

// Function to create and initialize a new queue
Queue* createQueue();

// Function to add a string to the queue
void enqueue(Queue* queue, const char* item);

// Function to remove a string from the queue
char* dequeue(Queue* queue);

// Function to check if the queue is empty
int isQueueEmpty(Queue* queue);

// Function to free the queue
void freeQueue(Queue* queue);

#endif // STRING_QUEUE_H
