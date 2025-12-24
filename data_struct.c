#include <stdbool.h>
#include "data_struct.h"

void initQueue(Queue* q) {
    q->front = 0;
    q->rear = 0;
}//если front = rear очередь пустая

bool isQueueEmpty(Queue* q) {
    return q->front == q->rear;
}//возвращаем true если пустая очередь

void enqueue(Queue* q, Node node) {
    q->data[q->rear++] = node;
}//добавляем элемент в конец очереди

Node dequeue(Queue* q) {
    return q->data[q->front++];
}//удаляем и возвращаем первый элемент очереди

void initStack(Stack* s) {
    s->top = -1;
}//иницилизируем стек как пустой

bool isStackEmpty(Stack* s) {
    return s->top == -1;
}//проверяем пустой ли стек

void push(Stack* s, Point point) {
    s->data[++s->top] = point;
}//добавляем элемент на вершину стека

Point pop(Stack* s) {
    return s->data[s->top--];
}//удаляем и возвращаем элемент с вершины стека