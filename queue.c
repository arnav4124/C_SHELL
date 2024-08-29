#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

// Function to create and initialize a new queue
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (queue == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    queue->front = queue->rear = NULL;
    return queue;
}

// Function to add a string to the queue
void enqueue(Queue* queue, const char* item) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Copy the string into the node's data field
    strncpy(newNode->data, item, MAX_STRING_LENGTH - 1);
    newNode->data[MAX_STRING_LENGTH - 1] = '\0';  // Ensure null termination
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
        return;
    }

    queue->rear->next = newNode;
    queue->rear = newNode;
}

// Function to remove a string from the queue
char* dequeue(Queue* queue) {
    if (queue->front == NULL) {
        fprintf(stderr, "Queue is empty\n");
        return NULL;
    }

    Node* temp = queue->front;
    char* item = (char*)malloc(MAX_STRING_LENGTH);
    if (item == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    // Copy the string from the node to return it
    strncpy(item, temp->data, MAX_STRING_LENGTH);
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    return item;
}

// Function to check if the queue is empty
int isQueueEmpty(Queue* queue) {
    return queue->front == NULL;
}

// Function to free the queue
void freeQueue(Queue* queue) {
    while (!isQueueEmpty(queue)) {
        free(dequeue(queue));
    }
    free(queue);
}
