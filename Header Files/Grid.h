#pragma once
#include "libraries.h"
#include "Cells.h"
#include "Board.h"
#include "MachineLearning.h"

#define MAX_THRESH 255
#define TRAINING_STATUS 1
using std::vector;

class Grid
{
public:
	/*	@brief Default constructor.
		Instantiates Board, Cell, and Machine Learning class.
		*/
	Grid();
	
	/*	Default destructor	*/
	virtual ~Grid();
	
	/*	@brief Extract squares on sudoku grid.
		Function for extracting squares in the order they are displayed on the grid.
		@param src Input sudoku image.
		@param orderedRect Varaible for storing the extracted sudoku squares.
	*/
	void extractSquares(const cv::Mat &src, vector<cv::Rect> &orderedRect);
	
	/*	@brief Populate sudoku board.
		Function for reading digits and populating the sudoku board.
		@param src Input sudoku image.
		@param orderedRect Varaible containing the extracted sudoku squares.
	*/
	void populate_sudoku_grid(const cv::Mat &src, vector<cv::Rect> &orderedRect);

	/*	@brief Print sudoku_grid
		Function for printing sudoku grid.
	*/
	void print_sudoku_grid();

private:
	/*	@param sudoku_cells Variable for storing an instance of the Cells class. */
	Cells sudoku_cells;

	/* @param sudoku_board Varaiable for storing an instance of the Board class. */
	Board sudoku_board;

	/* @param svm_model Varaiable for storing an instance of the MachineLearning class. */
	MachineLearning svm_model;

	/*	@param squares Coordinates of squares on the sudoku grid. */
	vector<vector<cv::Point>> squares;
	
	/*	@param thresh_val Thershold value used in the Threshold filter. */
	int thresh_val;
	
	/*	Helper function. Finds a cosine of angles between vectors from pt0->pt1 and from pt0->pt2.
		@param pt1 Point 1.
		@param pt2 Point 2.
		@param pt0 Point 0.
		@returns Cosine of angle between vectors.
	*/
	double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
	
	/*	Function for finding a sequence of squares detected on the image.
		@param src Input image.
	*/
	void findSquares(const cv::Mat &src);
	
	/*	Function for converting the detected squares into rectangle and sorting to their actual location on the grid.
		@param orderedRect Vector for storing the sorted squares. 
	*/
	void sortSquares(vector<cv::Rect> &orderedRect);

	/*	@brief Reads digits in cells.
		Function for parsing cells and reading the detected digits within them.
		@params contour Contours within the sudoku square.
		@params src Image containing cell.
		@returns Value of digit if detected, 0 if no number is present in the cell.
	*/
	int parse_cell(std::vector<std::vector<cv::Point>> contour, cv::Mat & src);

	/* Functions for debugging only. */
	void drawSquares(const cv::Mat &image);
};

