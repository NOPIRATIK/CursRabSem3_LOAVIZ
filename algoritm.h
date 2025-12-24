#ifndef ALGORITM_H
#define ALGORITM_H

#include "maze.h"
#include "data_struct.h"
#include <stdbool.h>

bool hasPathBFS(Maze* maze);
bool hasPathDFS(Maze* maze);

int findShortestPathBFS(Maze* maze, bool markPath);
int findPathDFS(Maze* maze, bool markPath);

void compareAlgorithms(Maze* maze);

#endif