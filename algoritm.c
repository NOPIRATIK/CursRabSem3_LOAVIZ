#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "algoritm.h"
#include "maze.h"
#include "interface.h"
//проверяем наличие пути через bfs
bool hasPathBFS(Maze* maze) {
    bool visited[MAX_SIZE][MAX_SIZE] = { false };
    Queue q;//массив посещенных клеток
    initQueue(&q);//инициализируем нулями

    Node startNode = { maze->start, 0 };
    enqueue(&q, startNode);
    visited[maze->start.x][maze->start.y] = true;

    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };
    //через fifo
    while (!isQueueEmpty(&q)) {
        Node current = dequeue(&q);

        if (current.point.x == maze->end.x && current.point.y == maze->end.y) {
            return true;//если точка конечная
        }

        for (int i = 0; i < 4; i++) {
            Point next = { current.point.x + dx[i], current.point.y + dy[i] };//проверяем возможные смещения (вверх,вниз...)
            if (isValidPoint(maze, next) && !visited[next.x][next.y] && isPassable(maze, next)) {
                visited[next.x][next.y] = true;
                Node nextNode = { next, current.distance + 1 };//если клетка внутри лабиринта, не посещена и не является  делаем новый узелстеной
                enqueue(&q, nextNode);
            }
        }
    }

    return false;//если очередь пустая - пути нет
}
//проверяем наличие пути через dfs
bool hasPathDFS(Maze* maze) {
    bool visited[MAX_SIZE][MAX_SIZE] = { false };
    Stack s;
    initStack(&s);

    push(&s, maze->start);
    visited[maze->start.x][maze->start.y] = true;

    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };
    //через lifo
    while (!isStackEmpty(&s)) {
        Point current = pop(&s);

        if (current.x == maze->end.x && current.y == maze->end.y) {
            return true;//если точка конечная
        }

        for (int i = 0; i < 4; i++) {
            Point next = { current.x + dx[i], current.y + dy[i] };//проверяем возможные смещения

            if (isValidPoint(maze, next) && !visited[next.x][next.y] && isPassable(maze, next)) {
                visited[next.x][next.y] = true;//если клетка внутри лабиринта, не посещена
                push(&s, next);
            }
        }
    }

    return false;
}
//поиск кратчайшего пути от старта
int findShortestPathBFS(Maze* maze, bool markPath) {
    clearSolution(maze);

    bool visited[MAX_SIZE][MAX_SIZE] = { false };
    Point parent[MAX_SIZE][MAX_SIZE];//массив клеток по которым шли
    Queue q;
    initQueue(&q);

    Node startNode = { maze->start, 0 };
    enqueue(&q, startNode);
    visited[maze->start.x][maze->start.y] = true;
    parent[maze->start.x][maze->start.y] = maze->start;

    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };

    bool found = false;
    Node finalNode;

    while (!isQueueEmpty(&q)) {
        Node current = dequeue(&q);

        if (current.point.x == maze->end.x && current.point.y == maze->end.y) {
            finalNode = current;
            found = true;
            break;//если финиш найден
        }

        for (int i = 0; i < 4; i++) {
            Point next = { current.point.x + dx[i], current.point.y + dy[i] };

            if (isValidPoint(maze, next) && !visited[next.x][next.y] && isPassable(maze, next)) {
                visited[next.x][next.y] = true;
                parent[next.x][next.y] = current.point;
                Node nextNode = { next, current.distance + 1 };
                enqueue(&q, nextNode);
            }
        }
    }

    if (!found) {
        return -1;
    }

    int pathLength = finalNode.distance;

    if (markPath) {
        Point current = maze->end;
        while (!(current.x == maze->start.x && current.y == maze->start.y)) {
            if (!(current.x == maze->end.x && current.y == maze->end.y)) {
                maze->grid[current.x][current.y] = SOLUTION;
            }
            current = parent[current.x][current.y];
        }
    }

    return pathLength;
}
//находим путь dfs
int findPathDFS(Maze* maze, bool markPath) {
    clearSolution(maze);

    bool visited[MAX_SIZE][MAX_SIZE] = { false };
    Point parent[MAX_SIZE][MAX_SIZE];
    Stack s;
    initStack(&s);

    push(&s, maze->start);
    visited[maze->start.x][maze->start.y] = true;
    parent[maze->start.x][maze->start.y] = maze->start;

    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };

    bool found = false;
    Point finalPoint;

    while (!isStackEmpty(&s)) {
        Point current = pop(&s);

        if (current.x == maze->end.x && current.y == maze->end.y) {
            finalPoint = current;
            found = true;
            break;
        }

        for (int i = 0; i < 4; i++) {
            Point next = { current.x + dx[i], current.y + dy[i] };

            if (isValidPoint(maze, next) && !visited[next.x][next.y] && isPassable(maze, next)) {
                visited[next.x][next.y] = true;
                parent[next.x][next.y] = current;
                push(&s, next);
            }
        }
    }

    if (!found) {
        return -1;
    }

    int pathLength = 0;
    Point current = finalPoint;

    if (markPath) {
        while (!(current.x == maze->start.x && current.y == maze->start.y)) {
            if (!(current.x == maze->end.x && current.y == maze->end.y)) {
                maze->grid[current.x][current.y] = SOLUTION;
            }
            current = parent[current.x][current.y];
            pathLength++;
        }
    }
    else {
        current = finalPoint;
        while (!(current.x == maze->start.x && current.y == maze->start.y)) {
            current = parent[current.x][current.y];
            pathLength++;
        }
    }

    return pathLength;
}
//сравнение dfs and bfs
void compareAlgorithms(Maze* maze) {
    printf("\n=== СРАВНЕНИЕ АЛГОРИТМОВ BFS И DFS ===\n");

    //создаем копии лабиринта для каждого алгоритма
    Maze* mazeBFS = createMaze(maze->rows, maze->cols);
    Maze* mazeDFS = createMaze(maze->rows, maze->cols);

    //копируем содержимое
    for (int i = 0; i < maze->rows; i++) {
        for (int j = 0; j < maze->cols; j++) {
            mazeBFS->grid[i][j] = maze->grid[i][j];
            mazeDFS->grid[i][j] = maze->grid[i][j];
        }
    }
    mazeBFS->start = maze->start;
    mazeBFS->end = maze->end;
    mazeDFS->start = maze->start;
    mazeDFS->end = maze->end;

    LARGE_INTEGER frequency, startTime, endTime;
    QueryPerformanceFrequency(&frequency);
    double bfsTime, dfsTime;

    QueryPerformanceCounter(&startTime);
    int bfsLength = findShortestPathBFS(mazeBFS, true);
    QueryPerformanceCounter(&endTime);
    bfsTime = ((double)(endTime.QuadPart - startTime.QuadPart) * 1000000.0) / frequency.QuadPart;

    QueryPerformanceCounter(&startTime);
    int dfsLength = findPathDFS(mazeDFS, true);
    QueryPerformanceCounter(&endTime);
    dfsTime = ((double)(endTime.QuadPart - startTime.QuadPart) * 1000000.0) / frequency.QuadPart;

    printf("\nРезультаты сравнения (точность в микросекундах):\n");
    printf("Алгоритм | Длина пути | Время (мкс) | Время (мс) | Статус\n");
    printf("---------|------------|-------------|------------|--------\n");

    if (bfsLength != -1) {
        printf("BFS      | %10d | %11.2f | %10.3f | Путь найден\n",
            bfsLength, bfsTime, bfsTime / 1000.0);
    }
    else {
        printf("BFS      |     --     | %11.2f | %10.3f | Путь не найден\n",
            bfsTime, bfsTime / 1000.0);
    }

    if (dfsLength != -1) {
        printf("DFS      | %10d | %11.2f | %10.3f | Путь найден\n",
            dfsLength, dfsTime, dfsTime / 1000.0);
    }
    else {
        printf("DFS      |     --     | %11.2f | %10.3f | Путь не найден\n",
            dfsTime, dfsTime / 1000.0);
    }

    printf("\nСравнение производительности:\n");
    if (bfsLength != -1 && dfsLength != -1) {
        if (bfsTime < dfsTime) {
            printf("• BFS быстрее DFS на %.2f%%\n",
                ((dfsTime - bfsTime) / dfsTime) * 100.0);
        }
        else if (dfsTime < bfsTime) {
            printf("• DFS быстрее BFS на %.2f%%\n",
                ((bfsTime - dfsTime) / bfsTime) * 100.0);
        }
        else {
            printf("• Алгоритмы работают одинаково быстро\n");
        }

        if (bfsLength < dfsLength) {
            printf("• BFS нашел более короткий путь (короче на %d шагов)\n",
                dfsLength - bfsLength);
        }
        else if (dfsLength < bfsLength) {
            printf("• DFS нашел более короткий путь (короче на %d шагов)\n",
                bfsLength - dfsLength);
        }
        else {
            printf("• Оба алгоритма нашли пути одинаковой длины\n");
        }
    }

    if (bfsLength != -1) {
        printf("\n--- Путь, найденный алгоритмом BFS ---\n");
        printMaze(mazeBFS);
    }

    if (dfsLength != -1) {
        printf("\n--- Путь, найденный алгоритмом DFS ---\n");
        printMaze(mazeDFS);
    }

    freeMaze(mazeBFS);
    freeMaze(mazeDFS);
}
