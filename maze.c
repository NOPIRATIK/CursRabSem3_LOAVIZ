#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "maze.h"
#include "algoritm.h"
//создаем структуру лабиринта
Maze* createMaze(int rows, int cols) {
    Maze* maze = (Maze*)malloc(sizeof(Maze));
    if (!maze) return NULL;

    maze->rows = rows;
    maze->cols = cols;

    //выделяем память для строк
    maze->grid = (char**)malloc(rows * sizeof(char*));
    if (!maze->grid) {
        free(maze);
        return NULL;
    }

    //выделяем память для каждой строки
    for (int i = 0; i < rows; i++) {
        maze->grid[i] = (char*)malloc(cols * sizeof(char));
        if (!maze->grid[i]) {
            //освобождаем ранее выделенную память
            for (int j = 0; j < i; j++) free(maze->grid[j]);
            free(maze->grid);
            free(maze);
            return NULL;
        }

        //инициализируем путь '.'
        for (int j = 0; j < cols; j++) {
            maze->grid[i][j] = PATH;
        }
    }

    maze->start.x = 0;
    maze->start.y = 0;
    maze->end.x = rows - 1;
    maze->end.y = cols - 1;

    maze->grid[maze->start.x][maze->start.y] = START;
    maze->grid[maze->end.x][maze->end.y] = END;

    return maze;
}
//освобождаем память выделенную под лабиринт
void freeMaze(Maze* maze) {
    if (maze) {
        if (maze->grid) {
            for (int i = 0; i < maze->rows; i++) {
                free(maze->grid[i]);
            }
            free(maze->grid);
        }
        free(maze);
    }
}
//проверяем наличие точки в границах лабиринта
bool isValidPoint(Maze* maze, Point p) {
    return p.x >= 0 && p.x < maze->rows && p.y >= 0 && p.y < maze->cols;
}
//проверяем можно ли пройти через клетку
bool isPassable(Maze* maze, Point p) {
    if (!isValidPoint(maze, p)) return false;
    char cell = maze->grid[p.x][p.y];
    return cell != WALL;
}
//убираем найденный путь '+'
void clearSolution(Maze* maze) {
    for (int i = 0; i < maze->rows; i++) {
        for (int j = 0; j < maze->cols; j++) {
            if (maze->grid[i][j] == SOLUTION) {
                maze->grid[i][j] = PATH;
            }
        }
    }
    maze->grid[maze->start.x][maze->start.y] = START;
    maze->grid[maze->end.x][maze->end.y] = END;
}
//записываем в файл
bool saveMazeToFile(Maze* maze, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Ошибка открытия файла\n");
        return false;
    }

    fprintf(file, "%d %d\n", maze->rows, maze->cols);
    fprintf(file, "%d %d %d %d\n", maze->start.x, maze->start.y, maze->end.x, maze->end.y);

    for (int i = 0; i < maze->rows; i++) {
        for (int j = 0; j < maze->cols; j++) {
            fprintf(file, "%c", maze->grid[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Лабиринт сохранен: %s\n", filename);
    return true;
}
//открываем файл
Maze* loadMazeFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка открытия файла\n");
        return NULL;
    }

    int rows, cols;
    if (fscanf(file, "%d %d", &rows, &cols) != 2) {
        printf("Неверный формат файла (размеры)\n");
        fclose(file);
        return NULL;
    }

    if (rows > MAX_SIZE || cols > MAX_SIZE) {
        printf("Слишком большой лабиринт (максимум %dx%d)\n", MAX_SIZE, MAX_SIZE);
        fclose(file);
        return NULL;
    }

    Maze* maze = createMaze(rows, cols);
    if (!maze) {
        printf("Ошибка выделения памяти\n");
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "%d %d %d %d", &maze->start.x, &maze->start.y, &maze->end.x, &maze->end.y) != 4) {
        printf("Неверный формат файла (координаты)\n");
        freeMaze(maze);
        fclose(file);
        return NULL;
    }

    int ch;
    while ((ch = fgetc(file)) == ' ' || ch == '\t') {}
    if (ch != '\n') ungetc(ch, file);

    for (int i = 0; i < maze->rows; i++) {
        for (int j = 0; j < maze->cols; j++) {
            int ch = fgetc(file);
            if (ch == EOF || ch == '\n') {
                printf("Файл поврежден или неполный\n");
                freeMaze(maze);
                fclose(file);
                return NULL;
            }
            maze->grid[i][j] = (char)ch;
        }

        ch = fgetc(file);
        if (ch != '\n' && ch != EOF) {
            printf("Неожиданный символ в конце строки %d\n", i);
        }
    }

    fclose(file);
    printf("Лабиринт загружен: %s\n", filename);
    return maze;
}
//инициализируем генератор СЧ
void generateMazeWithPath(Maze* maze, int wallPercentage) {
    srand((unsigned int)time(NULL));

    do {
        for (int i = 0; i < maze->rows; i++) {
            for (int j = 0; j < maze->cols; j++) {
                maze->grid[i][j] = PATH;
            }
        }

        maze->grid[maze->start.x][maze->start.y] = START;//старт
        maze->grid[maze->end.x][maze->end.y] = END;//конец

        int wallsToAdd = (maze->rows * maze->cols * wallPercentage) / 100;//кол-во стен
        //добавляем случайные стены
        for (int w = 0; w < wallsToAdd; w++) {
            int x, y;
            do {
                x = rand() % maze->rows;
                y = rand() % maze->cols;
            } while ((x == maze->start.x && y == maze->start.y) ||
                (x == maze->end.x && y == maze->end.y) ||
                maze->grid[x][y] == WALL);

            maze->grid[x][y] = WALL;
        }
    } while (!hasPathBFS(maze));
}