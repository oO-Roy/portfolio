#ifndef __KNIGHT_TOUR_H__
#define __KNIGHT_TOUR_H__

#include <stddef.h> /* for size_t type */

#define ROWS 8
#define COLS 8

typedef enum 
{
    SUCCESS,
    FAILURE
} knight_status;

/* -----------------------------------------------------------------------------
    @ description:      Function that calculates the possible movements
                        of the knight on chess board.
    
    @ params:           

    @ time complexity:
    
    @ space complexity: 
------------------------------------------------------------------------------*/
int KnightTour(int board[ROWS][COLS], size_t row_pos, size_t col_pos);

#endif  /* __KNIGHT_TOUR_H__ */
 
