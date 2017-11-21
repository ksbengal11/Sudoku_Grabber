#include "main.h"

int main(void)
{
	std::cout << "Sudoku Solver.\n";
	std::cout << "Compiled using OPENCV version " << CV_VERSION << "\n";
	std::cout << "Version 1.0. \n\n";

	// Read input image.
	cv::Mat input_image = cv::imread(SUDOKU_IMAGE_PATH, cv::IMREAD_GRAYSCALE);
	if (input_image.empty())
	{
		std::cerr << "Could not load image: " << SUDOKU_IMAGE_PATH << " \n";
		return EXIT_FAILURE;
	}

#if SEE_IMAGES
	cv::namedWindow("Input Image", cv::WINDOW_AUTOSIZE);
	cv::imshow("Input Image", input_image);
#endif 

	// Extract squares from the input image and store as rectangles.
	std::vector<cv::Rect> orderedRects;
	Grid sudoku_grid = Grid();
	sudoku_grid.extractSquares(input_image, orderedRects);

	// Read digits from cells that contain them and populate sudoku board.
	sudoku_grid.populate_sudoku_grid(input_image, orderedRects);

	// Print sudoku board.
	sudoku_grid.print_sudoku_grid();
	

#if SEE_IMAGES
	while (true)
	{
		char keyPress = cv::waitKey(20);
		if (keyPress == VK_ESCAPE) { break; }
	}
	cv::destroyAllWindows();

#endif
	return EXIT_SUCCESS;
}