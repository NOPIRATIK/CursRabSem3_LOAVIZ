#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include "maze.h"

typedef struct {
    Point point;
    int distance;
} Node;

typedef struct {
    Node data[MAX_SIZE * MAX_SIZE];//массив для элементов
    int front;//индекс начала очереди
    int rear;//индекс конца очереди
} Queue;

typedef struct {
    Point data[MAX_SIZE * MAX_SIZE];
    int top;
} Stack;

void initQueue(Queue* q);
bool isQueueEmpty(Queue* q);
void enqueue(Queue* q, Node node);
Node dequeue(Queue* q);

void initStack(Stack* s);
bool isStackEmpty(Stack* s);
void push(Stack* s, Point point);
Point pop(Stack* s);

#endif