#pragma once
#include "libraries.h"

#define MAX_ROW 9
#define MAX_COL 9
/*
	@brief Class containing sudoku board
	Class that stores the sudoku board and the relavant actions.
	@param game 9x9 array representation of the sudoku board.
*/
class Board
{
public:

	/*	@brief Default contrustor
		Default constructor of the board class. Initializes the game board to 0.
	*/
	Board()
	{
		for (size_t row = 0; row < MAX_ROW; ++row)
		{
			for (size_t col = 0; col < MAX_COL; ++col)
			{
				this->game[row][col] = 0;
			}
		}
	}

	/*	Default destructor.	*/
	~Board()
	{
	}

	/*	@brief Function for updating sudoku board.
		@params row	Row on Sudoku grid.
		@params col Column on Sudoku grid.
		@param value value to update Sudoku grid with.
	*/
	void setBoard(int row, int col, int value)
	{
		this->game[row][col] = value;
	}

	/*	@brief Function for printing sudoku board. */
	void printBoard()
	{
		for (size_t row = 0; row < MAX_ROW; ++row)
		{
			for (size_t col = 0; col < MAX_COL; ++col)
			{
				std::cout << this->game[row][col] << " ";
			}
			std::cout << std::endl;
		}
	}

	/*	@brief Function for clearing sudoku board.	*/
	void clear()
	{
		for (size_t row = 0; row < MAX_ROW; ++row)
		{
			for (size_t col = 0; col < MAX_COL; ++col)
			{
				this->game[row][col] = 0;
			}
		}
	}

private:
	/*	@param game 9x9 array representation of the sudoku board.	*/
	int game[9][9];
};

