#include <stdio.h>

#include "knight_tour.h"

#define UNUSED __attribute__ ((unused))

int main()
{
	knight_status result = 0;

	size_t i = 0;
	size_t j = 0;

	int board[ROWS][COLS] = {0};
	
	result = KnightTour(board, 0, 0);

	if (!result)
	{
		for (i = 0 ; i < ROWS ; ++i)
		{
			printf("-------------------------------------------------\n");
			
			for (j = 0 ; j < COLS ; ++j)
			{
				if (10 > board[i][j])
				{
					printf("|  %d  ", board[i][j]);
				}
				
				else
				{
					printf("| %d  ", board[i][j]);
				}
			}

			printf("|\n");
		}

		printf("-------------------------------------------------\n");
	}

	return 0;
}
