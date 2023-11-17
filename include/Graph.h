#ifndef graph_h
#define graph_h
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct Edge
{
    int neighbor;
    int weight;
};

struct Graph
{
    int nodeAmount;
    int *index;
    int *edges_array;
};

struct Graph *getGraph(int nodes);
void generateBinaryTree(int, int **index, int **edges_array);
void printGraph(struct Graph *graph);

#endif // graph_h