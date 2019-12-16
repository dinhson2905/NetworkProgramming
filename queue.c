#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include <gtk/gtk.h>
GMutex queueMutex;
// A linked list (LL) node to store a queue entry 
struct QNode { 
    char key[1024]; 
    struct QNode *next; 
}; 
  
// The queue, front stores the front node of LL and rear stores ths 
// last node of LL 
struct Queue { 
    struct QNode *front, *rear; 
}; 
  
// A utility function to create a new linked list node. 
struct QNode* newNode(char *k) { 
    struct QNode *temp = (struct QNode*)malloc(sizeof(struct QNode)); 
    strcpy(temp->key, k);
    temp->next = NULL; 
    return temp;  
} 
  
// A utility function to create an empty queue 
struct Queue *createQueue() { 
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue)); 
    q->front = q->rear = NULL; 
    return q; 
} 
  
// The function to add a key k to q 
void enQueue(struct Queue *q, char *k) { 
    g_mutex_lock(&queueMutex);
    // Create a new LL node 
    struct QNode *temp = newNode(k); 
  
    // If queue is empty, then new node is front and rear both 
    if (q->rear == NULL) 
    { 
       q->front = q->rear = temp;
       g_mutex_unlock(&queueMutex); 
       return; 
    } 
  
    // Add the new node at the end of queue and change rear 
    q->rear->next = temp; 
    q->rear = temp; 
    g_mutex_unlock(&queueMutex);
} 

void displayQueue(struct Queue *q) {
    struct QNode *temp = q->front;
    while(temp != NULL) {
        printf("%s",temp->key);
        temp = temp->next;
    }
}  
// Function to remove a key from given queue q 
struct QNode *deQueue(struct Queue *q) { 
    g_mutex_lock(&queueMutex);
    // If queue is empty, return NULL. 
    if (q->front == NULL) {
        g_mutex_unlock(&queueMutex);
        return NULL;
    }
        
  
    // Store previous front and move front one node ahead 
    struct QNode *temp = q->front; 
    q->front = q->front->next; 
  
    // If front becomes NULL, then change rear also as NULL 
    if (q->front == NULL) 
       q->rear = NULL; 
    g_mutex_unlock(&queueMutex);
    return temp; 
} 