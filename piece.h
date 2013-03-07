/*
 * Bailey Forrest
 * baileycforrest@gmail.com
 *
 * piece.h
 *
 * Contains operations on pieces
 */

#ifndef PIECE_H
#define PIECE_H

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include "board.h"

#define PHEIGHT 4
#define NUM_DIR 4
#define NUM_PIECES 7
//#define FALL 0x8
//#define NOT_FALL 0x7

typedef enum { I = 1,
               J = 2,
               L = 3,
               O = 4,
               S = 5,
               T = 6,
               Z = 7
} pieceType;

// Falling piece
typedef struct piece
{
    int x; // Coordinates of the upper left of the piece
    int y;

    block blocks[PHEIGHT][PHEIGHT]; // 4x4 grid of blocks
    pieceType t;
    bRow* cRow; // Points to lowest row piece is in
} piece;

void setPiece(piece *p);
bool validLocation(piece *p);
bool movePiece(piece *p, int dir);
void swap(block *b1, block*b2);
void trans(block blocks[PHEIGHT][PHEIGHT]);
void rotate(block blocks[PHEIGHT][PHEIGHT]);
bool rotatePiece(piece *p);

void initI(piece *p);
void initJ(piece *p);
void initL(piece *p);
void initO(piece *p);
void initS(piece *p);
void initT(piece *p);
void initZ(piece *p);

piece* newPiece(pieceType t, board *b);
void freePiece(piece *p);



#endif
