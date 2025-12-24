#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "interface.h"

void printMaze(Maze* maze) {
    printf("\nЛабиринт %dx%d:\n", maze->rows, maze->cols);

    if (maze->cols <= 40) {

        printf(" ");
        for (int j = 0; j < maze->cols; j++) {
            printf("--");
        }
        printf("\n");

        for (int i = 0; i < maze->rows; i++) {
            printf("|");
            for (int j = 0; j < maze->cols; j++) {
                printf("%c ", maze->grid[i][j]);
            }
            printf("|\n");
        }

        printf(" ");
        for (int j = 0; j < maze->cols; j++) {
            printf("--");
        }
        printf("\n");
    }
}

void waitForUser() {
    printf("\nНажмите Enter для продолжения...");
    while (getchar() != '\n');
}

int getValidatedInput(const char* prompt, int min, int max) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1 && value >= min && value <= max) {
            break;
        }
        printf("Ошибка: введите число от %d до %d\n", min, max);
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
    return value;
}

void showMenu() {
    printf("\n1. Сгенерировать лабиринт\n");
    printf("2. Загрузить лабиринт из файла\n");
    printf("3. Сохранить лабиринт в файл\n");
    printf("4. Показать лабиринт\n");
    printf("5. Найти путь (BFS)\n");
    printf("6. Найти путь (DFS)\n");
    printf("7. Сравнить алгоритмы BFS и DFS\n");
    printf("8. Информация о лабиринте\n");
    printf("0. Выход\n");
    printf("\nВыберите пункт: ");
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printHeader() {
    clearScreen();
    printf("=============================================================================\n");
    printf("Меню, Курсовая работа на тему 'Реализация алгоритма поиска путей в лабиринте'\n");
    printf("ст.гр. 24ВВВ1 Гурин А.Н.\n");
    printf("=============================================================================\n");
}