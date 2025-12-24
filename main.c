#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>

#include "maze.h"
#include "algoritm.h"
#include "interface.h"

int main() {
    setlocale(LC_ALL, "Ru");

    Maze* maze = NULL;
    int choice;

    printHeader();

    while (1) {
        if (maze != NULL) {
            printf("\nТекущий лабиринт: %dx%d\n", maze->rows, maze->cols);
        }
        else {
            printf("\nЛабиринт не загружен\n");
        }

        showMenu();

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        if (choice == 0) break;

        printHeader();

        if (choice == 1) {
            printf("=== ГЕНЕРАЦИЯ ЛАБИРИНТА ===\n");
            int rows = getValidatedInput("Количество строк (1-40): ", 1, MAX_SIZE);
            int cols = getValidatedInput("Количество столбцов (1-40): ", 1, MAX_SIZE);
            int walls = getValidatedInput("Процент стен (0-40): ", 0, 40);

            if (maze != NULL) freeMaze(maze);
            maze = createMaze(rows, cols);
            if (maze) {
                generateMazeWithPath(maze, walls);
                printf("\nЛабиринт сгенерирован\n");
                printMaze(maze);
            }
            waitForUser();
            printHeader();
        }
        else if (choice == 2) {
            printf("=== ЗАГРУЗКА ЛАБИРИНТА ===\n");
            char filename[256];
            printf("Имя файла: ");
            scanf("%255s", filename);
            while (getchar() != '\n');

            Maze* newMaze = loadMazeFromFile(filename);
            if (newMaze) {
                if (maze) freeMaze(maze);
                maze = newMaze;
                printf("\nЛабиринт загружен\n");
                printMaze(maze);
            }
            waitForUser();
            printHeader();
        }
        else if (choice == 3) {
            printf("=== СОХРАНЕНИЕ ЛАБИРИНТА ===\n");
            if (maze == NULL) {
                printf("Ошибка: лабиринт не загружен\n");
                waitForUser();
                printHeader();
                continue;
            }

            char filename[256];
            printf("Имя файла: ");
            scanf("%255s", filename);
            while (getchar() != '\n');

            if (saveMazeToFile(maze, filename)) {
            }
            waitForUser();
            printHeader();
        }
        else if (choice == 4) {
            printf("=== ПРОСМОТР ЛАБИРИНТА ===\n");
            if (maze == NULL) {
                printf("Ошибка: лабиринт не загружен\n");
            }
            else {
                printMaze(maze);
            }
            waitForUser();
            printHeader();
        }
        else if (choice == 5) {
            printf("=== ПОИСК ПУТИ (BFS) ===\n");
            if (maze == NULL) {
                printf("Ошибка: лабиринт не загружен\n");
                waitForUser();
                printHeader();
                continue;
            }

            clock_t start = clock();
            int pathLength = findShortestPathBFS(maze, true);
            clock_t end = clock();
            double timeTaken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;

            if (pathLength != -1) {
                printf("\nПуть найден! Длина: %d шагов\n", pathLength);
                printf("Время выполнения: %.2f мс\n", timeTaken);
                printMaze(maze);
            }
            else {
                printf("\nПуть не найден\n");
                printf("Время выполнения: %.2f мс\n", timeTaken);
                printMaze(maze);
            }
            waitForUser();
            printHeader();
        }
        else if (choice == 6) {
            printf("=== ПОИСК ПУТИ (DFS) ===\n");
            if (maze == NULL) {
                printf("Ошибка: лабиринт не загружен\n");
                waitForUser();
                printHeader();
                continue;
            }

            clock_t start = clock();
            int pathLength = findPathDFS(maze, true);
            clock_t end = clock();
            double timeTaken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;

            if (pathLength != -1) {
                printf("\nПуть найден! Длина: %d шагов\n", pathLength);
                printf("Время выполнения: %.2f мс\n", timeTaken);
                printMaze(maze);
            }
            else {
                printf("\nПуть не найден\n");
                printf("Время выполнения: %.2f мс\n", timeTaken);
                printMaze(maze);
            }
            waitForUser();
            printHeader();
        }
        else if (choice == 7) {
            if (maze == NULL) {
                printf("Ошибка: лабиринт не загружен\n");
                waitForUser();
                printHeader();
                continue;
            }

            compareAlgorithms(maze);
            waitForUser();
            printHeader();
        }
        else if (choice == 8) {
            printf("=== ИНФОРМАЦИЯ О ЛАБИРИНТЕ ===\n");
            if (maze == NULL) {
                printf("Ошибка: лабиринт не загружен\n");
                waitForUser();
                printHeader();
                continue;
            }

            printMaze(maze);

            int walls = 0;
            for (int i = 0; i < maze->rows; i++) {
                for (int j = 0; j < maze->cols; j++) {
                    if (maze->grid[i][j] == WALL) walls++;
                }
            }

            printf("\nИнформация:\n");
            printf("Размер: %d x %d\n", maze->rows, maze->cols);
            printf("Стен: %d (%.1f%%)\n", walls, (float)walls * 100 / (maze->rows * maze->cols));
            printf("Старт: (%d, %d)\n", maze->start.x + 1, maze->start.y + 1);
            printf("Финиш: (%d, %d)\n", maze->end.x + 1, maze->end.y + 1);
            printf("Путь существует (BFS): %s\n", hasPathBFS(maze) ? "да" : "нет");
            printf("Путь существует (DFS): %s\n", hasPathDFS(maze) ? "да" : "нет");

            waitForUser();
            printHeader();
        }
        else {
            printf("Неверный выбор\n");
            waitForUser();
            printHeader();
        }
    }

    if (maze) freeMaze(maze);
    clearScreen();
    printf("Выход из программы\n");
    return 0;
}