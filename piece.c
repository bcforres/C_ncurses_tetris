/*
 * Bailey Forrest
 * baileycforrest@gmail.com
 *
 * piece.c
 *
 * Contains operations on pieces
 */

#include "piece.h"
#include "board.h"
#include <stdio.h>

// place a piece on the board
void setPiece(piece *p)
{
    int i, row;
    block b;
    bRow *r = p->cRow;
    for(row = 0; row < PHEIGHT; row++)
    {
        for(i = 0; i < PHEIGHT; i++)
        {
            b = p->blocks[row][i];
            if(b)
                r->blocks[p->x + i] = b;
        }
        if(r)
            r = r->prev;
    }
}


bool validLocation(piece *p)
{
    int row = 0, col = 0;
    bRow* rp = p->cRow;
    for(row = 0; row < PHEIGHT; row++)
    {
        for(col = 0; col < PHEIGHT; col++)
        {
            // If there's a block here
            if(p->blocks[row][col])
            {
                // Block off the board
                if(rp == NULL)
                    return false;

                // Check if block out of bounds
                if((p->y + row < 0) || (p->x + col < 0) ||
                   (p->x + col >= B_WIDTH))
                    return false;

                // Check if there is an overlapping block
                if(rp->blocks[p->x + col])
                    return false;
            }
        }
        if(rp)
            rp = rp->prev;
    }

    return true;
}

// Move a block.  Returns true if piece moved successfully
// dir: -1 (left), 0 (down), 1 (right)
bool movePiece(piece *p, int dir)
{
    // Moving down
    if(!dir)
    {
        p->y++;
        p->cRow = p->cRow->prev;
    }
    else
        p->x += dir;

    if(validLocation(p))
        return true;

    // Move wasn't valid, undo it
    if(!dir)
    {
        p->y--;
        p->cRow = p->cRow->next;
    }
    else
        p->x -= dir;
    
    return false;
}

inline void swap(block *b1, block *b2)
{
    block t;
    t = *b1;
    *b1 = *b2;
    *b2 = t;
}

// Transpose PHEIGHTxPHEIGHT 2d array
void trans(block blocks[PHEIGHT][PHEIGHT])
{
    int i, j;

    for(i = 0; i < PHEIGHT - 1; i++)
    {
        for(j = i + 1; j < PHEIGHT; j++)
        {
            swap(&blocks[i][j], &blocks[j][i]);
        }
    }
}

// Rotate upper 3x3 square of block matrix
void rotate(block blocks[PHEIGHT][PHEIGHT])
{
    block temp[PHEIGHT][PHEIGHT];
    bzero(temp, sizeof(block) * PHEIGHT * PHEIGHT);
    
    int oRow, oCol;
    for(oRow = 0; oRow < 3; oRow++)
        for(oCol = 0; oCol < 3; oCol++)
        {
            temp[oCol][2 - oRow] = blocks[oRow][oCol];
        }

    memcpy(blocks, temp, sizeof(block) * PHEIGHT * PHEIGHT);
}

inline void flipZ(block blocks[PHEIGHT][PHEIGHT])
{
    swap(&blocks[0][0], &blocks[1][0]);
    swap(&blocks[1][2], &blocks[2][0]);
}

inline void flipS(block blocks[PHEIGHT][PHEIGHT])
{
    swap(&blocks[0][2], &blocks[1][2]);
    swap(&blocks[1][0], &blocks[2][2]);
}


// Rotates a piece clockwise.  Returns false if rotation could not be completed
bool rotatePiece(piece *p)
{
    bool success = false;
    piece *testPiece = malloc(sizeof(piece));
    memcpy(testPiece, p, sizeof(piece));
    
    switch(p->t)
    {
    case O: break; // Do nothing for O pieces
    case I: trans(testPiece->blocks); break;
    case S: flipS(testPiece->blocks); break;
    case Z: flipZ(testPiece->blocks); break;
        
    case J:
    case L:
    case T: rotate(testPiece->blocks); break;
    default: break; // Should not get here
    }

    // If the move was successful, copy it to the original piece
    if(validLocation(testPiece))
    {
        memcpy(p, testPiece, sizeof(piece));
        success = true;
    }
    
    free(testPiece);

    return success;
}

// Initializes piece with I pattern
// Assumes 2d array currently clear
void initI(piece *p)
{
    int i;
    for(i = 0; i < 4; i++)
        p->blocks[PHEIGHT / 2][i] = (block)I;
}

// Initializes piece with J pattern
// Assumes 2d array currently clear
void initJ(piece *p)
{
    int i;
    for(i = 0; i < 3; i++)
        p->blocks[PHEIGHT / 2 - 1][i] = (block)J;
    p->blocks[PHEIGHT / 2][2] = (block)J;
}

// Initializes piece with L pattern
// Assumes 2d array currently clear
void initL(piece *p)
{
    int i;
    for(i = 0; i < 3; i++)
        p->blocks[PHEIGHT / 2 - 1][i] = (block)L;
    p->blocks[0][2] = (block)L;
}

// Initializes piece with O pattern
// Assumes 2d array currently clear
void initO(piece *p)
{
    int i, j;
    for(i = PHEIGHT / 2 - 1; i < PHEIGHT / 2 + 1; i++)
        for(j = PHEIGHT / 2 - 1; j < PHEIGHT / 2 + 1; j++)
            p->blocks[i][j] = (block)O;
}

// Initializes piece with S pattern
// Assumes 2d array currently clear
void initS(piece *p)
{
    int i;
    for(i = 1; i < PHEIGHT / 2 + 1; i++)
        p->blocks[0][i] = (block)S;
    for(i = 0; i < PHEIGHT / 2; i++)
        p->blocks[1][i] = (block)S;    
}

// Initializes piece with T pattern
// Assumes 2d array currently clear
void initT(piece *p)
{
    int i;
    for(i = 0; i < 3; i++)
        p->blocks[1][i] = (block)T;
    p->blocks[0][1] = (block)T;
}

// Initializes piece with Z pattern
// Assumes 2d array currently clear
void initZ(piece *p)
{
    int i;
    for(i = 0; i < PHEIGHT / 2; i++)
        p->blocks[0][i] = (block)Z;
    for(i = 1; i < PHEIGHT / 2 + 1; i++)
        p->blocks[1][i] = (block)Z;
}


// Creates new piece of specified type
piece *newPiece(board *b)
{
    pieceType t;
    t = rand() % NUM_PIECES + 1; // Generate random piece type
    
    piece* p = calloc(1, sizeof(piece));
    p->y = 0;
    p->x = B_WIDTH / 2 - PHEIGHT / 2;
    p->t = t;

    bRow *r = b->top;
    p->cRow = r;

    switch(t)
    {
    case I: initI(p); break;
    case J: initJ(p); break;
    case L: initL(p); break;
    case O: initO(p); break;
    case S: initS(p); break;
    case T: initT(p); break;
    case Z: initZ(p); break;
    }

    // rotate the piece a random number of times
    int rotates = rand() % NUM_DIR;

    while(rotates--)
        rotatePiece(p);

    return p;
}

void freePiece(piece *p)
{
    free(p);
}
