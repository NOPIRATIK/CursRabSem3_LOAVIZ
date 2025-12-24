#ifndef MAZE_H
#define MAZE_H

#include <stdbool.h>

#define MAX_SIZE 40
#define WALL '#'
#define PATH '.'
#define START 'S'
#define END 'E'
#define SOLUTION '+'

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    char** grid;
    int rows;
    int cols;
    Point start;
    Point end;
} Maze;

Maze* createMaze(int rows, int cols);
void freeMaze(Maze* maze);
bool isValidPoint(Maze* maze, Point p);
bool isPassable(Maze* maze, Point p);
void clearSolution(Maze* maze);
bool saveMazeToFile(Maze* maze, const char* filename);
Maze* loadMazeFromFile(const char* filename);
void generateMazeWithPath(Maze* maze, int wallPercentage);

#endif