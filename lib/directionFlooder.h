#include "borrowed/queue.h"
#include "datatypes.h"
#include <stdio.h>
#include <stdlib.h>

SIMPLEQ_HEAD(simplehead, cNode) coordQ = SIMPLEQ_HEAD_INITIALIZER(coordQ);
struct cNode
{
    coord c;
    SIMPLEQ_ENTRY(cNode) cNodes;
} *c1, *c2;

void enque(const int row, const int col)
{
    c1 = malloc(sizeof(struct cNode));
    SIMPLEQ_INSERT_TAIL(&coordQ, c1, cNodes);
    c1->c.y = row;
    c1->c.x = col;
    //printf("Enqued %d,%d ", row, col);
}

coord deque()
{
    c1 = SIMPLEQ_FIRST(&coordQ);
    SIMPLEQ_REMOVE_HEAD(&coordQ, cNodes);
    struct coord c = c1->c;
    free(c1);
    //printf("Dequed %d,%d ", c.y, c.x);
    return c;
}

/// If a tile does not have a direction yet, set the direction and add it to the frontier
void addSurroundingToQ(int **map, int **result, const struct coord c)
{
    int row = c.y, col = c.x;
    if(map[row+1][col] == FLOOR && result[row+1][col] == 0)
    {
        result[row+1][col] = NORTH;
        enque(row+1, col);
    }
    if(map[row-1][col] == FLOOR && result[row-1][col] == 0)
    {
        result[row-1][col] = SOUTH;
        enque(row-1, col);
    }
    if(map[row][col+1] == FLOOR && result[row][col+1] == 0)
    {
        result[row][col+1] = WEST;
        enque(row, col+1);
    }
    if(map[row][col-1] == FLOOR && result[row][col-1] == 0)
    {
        result[row][col-1] = EAST;
        enque(row, col-1);
    }
}
/// Function that returns a direction mapping for AI to follow towards a destination.
///
/// map is defines FLOORs and WALLS
/// result is the map that is modified in-place
/// Warning: There's no out of bounds checking! It is expected that **map has WALLS surrounding the limits of the map!
void flood(int **map, int **result, const int sourceRow, const int sourceCol)
{
    // Add source to frontier
    enque(sourceRow, sourceCol);

    // Set source to non-zero to prevent it from being overwritten during the recursive algo
    result[sourceRow][sourceCol] = DESTINATION;

    // While frontier isn't empty, there's tiles to be explored
    while(!SIMPLEQ_EMPTY(&coordQ))
    {
        // Pop the frontier
        struct coord c = deque();

        // Add surrounding non-explored FLOORs to frontier
        addSurroundingToQ(map, result, c);

        //puts("coords in queue:");
        //SIMPLEQ_FOREACH(c2, &coordQ, cNodes) printf("%d,%d\n",c2->c.y, c2->c.x);
    }
}
