#ifndef INTERFACE_H
#define INTERFACE_H

#include "maze.h"

void printMaze(Maze* maze);
void printHeader();

void waitForUser();
int getValidatedInput(const char* prompt, int min, int max);
void showMenu();
void clearScreen();

#endif