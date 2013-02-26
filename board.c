/*
 * Bailey Forrest - bcforres
 * baileycforrest@gmail.com
 *
 * board.c - main functions for tetris board
 *
 */

#include "board.h"

inline bRow* createBRow()
{
    bRow* newR = calloc(1, sizeof(bRow));
    newR->next = NULL;
    newR->prev = NULL;

    return newR;
}

board *createBoard()
{
    board *output = malloc(sizeof(board));
    board->bottom = createBRow();
    bRow *prev = board->bottom;
    
    bRow *new;
    for(int i = 0; i < B_HEIGHT - 1; i++)
    {
        new = createBRow();
        prev->next = new;
        new->prev = prev;
        prev = new;
    }
    output->top = new;

    return output;
}

// Gets row number from board
inline bRow *getBrow(board *board, int rowN)
{
    register bRow *row = board->bottom;
    while(rowN--)
        row = row->next;

    return row;
}

// Remove row, Responsibility of calling function to put new rows at top
void removeRow(bRow *row);
{
    bRow* row = getBRow(board, rowN);

    row->prev->next = row->next;
    row->next->prev = row->prev;

    free(row);
}

// Returns true if the row is full, else false
bool rowFull(bRow* row)
{
    for(int i = 0; i < B_WIDTH; i++)
        if(!row->blocks[i])
            return false;

    return true;
}

// Removes empty rows from the board
// Returns the number of rows removed
void removeEmptyRows(board* board)
{
    int removed = 0;
    bRow *next, *row = board->bottom;
    while(row)
    {
        next = row->next;
        if(rowFull(row))
        {
            removeRow(row);
            removed++;
        }
        row = next;
    }
    
    for(int i = 0; i < removed; i++)
    {
        bRow *new = createBRow();
        board->top->next = new;
        new->prev = board->top;
        board->top = new;
    }

    return removed;
}
