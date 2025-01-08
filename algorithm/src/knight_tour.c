/* 
	Developer: Roy Steiner.
	File: Knight's tour.
	Reviewer: Amos Duek. 
	Date: 23/08/24
	Status: Approved.
*/
#include <assert.h> /*assert*/

#include "knight_tour.h" /*size_t, MACROS*/

#define TABLE_SIZE (ROWS * COLS)
#define POSSIBLE_MOVES 8

#define CELL(ROW, COL) ((COLS * ROW) + COL)

typedef struct coordinates
{
    int row_pos;
    int col_pos;
}coordinates_t;

static coordinates_t LUT[POSSIBLE_MOVES] = {{1, -2}, {1, 2}, {2, 1}, {2, -1}, 
                                    {-1, 2}, {-1, -2}, {-2, -1}, {-2, 1}};

static int IsFreeCell(int board[ROWS][COLS], size_t row_pos, size_t col_pos)
{
    assert(board);
    
    return (-1 == board[row_pos][col_pos]);
}

static int IsInBound(int row_pos, int col_pos)
{
    return (((0 <= row_pos) && (ROWS > row_pos))
                    && ((0 <= col_pos) && (COLS > col_pos)));
}

static int IsValidStep(int board[ROWS][COLS], size_t row_pos, size_t col_pos)
{
    assert(board);
    
    return (IsInBound(row_pos, col_pos) && IsFreeCell(board, row_pos, col_pos));
}

static size_t GetDegree(int board[ROWS][COLS], size_t row_pos, size_t col_pos)
{
    size_t i = 0;
    size_t counter = 0;
    
    assert(board);

    for (i = 0 ; i < POSSIBLE_MOVES ; ++i)
    {
        if (IsValidStep(board, (row_pos + LUT[i].row_pos) 
                                                , (col_pos + LUT[i].col_pos)))
        {
            ++counter;
        }
    }
    
    return counter;
}

static size_t NextMinDegree(int board[ROWS][COLS], size_t row_pos
                                                , size_t col_pos, size_t index)
{
    size_t i = 0;
    size_t deg = 0;
    size_t min_deg = POSSIBLE_MOVES;

    assert(board);

    for (i = 0 ; i < POSSIBLE_MOVES ; ++i)
    {
        if (IsValidStep(board, (row_pos + LUT[i].row_pos + LUT[index].row_pos)
                            , (col_pos + LUT[i].col_pos + LUT[index].col_pos)))
        {
            deg = GetDegree(board
                    , (row_pos + LUT[i].row_pos + LUT[index].row_pos)
                            , (col_pos + LUT[i].col_pos + LUT[index].col_pos));
        }
        
        if (min_deg >= deg)
        {
            min_deg = deg;
        }
    }

    return min_deg;   
}

static size_t BreakTie(int board[ROWS][COLS], size_t row_pos, size_t col_pos
                                            ,   size_t index1, size_t index2)
{
    size_t deg1 = 0;
    size_t deg2 = 0;

    assert(board);

    deg1 = NextMinDegree(board, row_pos, col_pos, index1);
    deg2 = NextMinDegree(board, row_pos, col_pos, index2);

    return (((deg1 <= deg2) * index1) + ((deg1 > deg2) * index2));
}                    

static int RecursiveKnightTour(int board[ROWS][COLS], size_t row_pos
                                        , size_t col_pos, size_t step_counter)
{   
    size_t i = 0;
    size_t deg = 0;
    size_t min_deg = POSSIBLE_MOVES;
    size_t next_min_deg = 0;
    size_t min_deg_idx = 0;
    size_t next_min_deg_idx = 0;
    
    assert(board);

    board[row_pos][col_pos] = step_counter;

    if (((ROWS * COLS) - 1) == step_counter)
    {
        return SUCCESS;
    }
        
    for (i = 0 ; i < POSSIBLE_MOVES ; ++i)
    {
        if (IsValidStep(board, (row_pos + LUT[i].row_pos)
                                    , (col_pos + LUT[i].col_pos)))
        {
            deg = GetDegree(board, (row_pos + LUT[i].row_pos)
                                                , (col_pos + LUT[i].col_pos));

            if (min_deg >= deg)
            {
                next_min_deg = min_deg;
                next_min_deg_idx = min_deg_idx;
                min_deg = deg;
                min_deg_idx = i;
            }
        }
    }

    if (min_deg == next_min_deg)
    {
        min_deg_idx = BreakTie(board, (row_pos + LUT[i].row_pos)
                , (col_pos + LUT[i].col_pos), min_deg_idx, next_min_deg_idx);
    }
            
    if (SUCCESS == RecursiveKnightTour(board
                , (row_pos + LUT[min_deg_idx].row_pos)
                    , (col_pos + LUT[min_deg_idx].col_pos), step_counter + 1))
    {
        return SUCCESS;
    }

    board[row_pos][col_pos] = -1;

    return FAILURE;
}

int KnightTour(int board[ROWS][COLS], size_t row_pos, size_t col_pos)
{
    size_t i = 0;
    size_t step_counter = 0;

    assert(board);

    for (i = 0 ; i < TABLE_SIZE ; ++i)
    {
        *((int*)board + i) = -1;
    }

    return RecursiveKnightTour(board, row_pos, col_pos, step_counter);
}

