#include "Grid.h"

Grid::Grid()
{
	this->sudoku_cells = Cells();
	this->sudoku_board = Board();
	this->svm_model = MachineLearning(TRAINING_STATUS);
}

Grid::~Grid()
{
}

/* Keep detecting squares in images until 81 of them are found. */
void Grid::extractSquares(const cv::Mat &src, vector<cv::Rect>& orderedRect)
{
	while (true)
	{
		this->squares.clear();
		
		this->findSquares(src);
		if (squares.size() == 81) { break; }
		
		this->thresh_val += 1;
		if (this->thresh_val > 255)
		{
			std::cerr << "Couldn't find cells. Exiting program." << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	this->sortSquares(orderedRect);

#if SEE_IMAGES
	this->drawSquares(src);
#endif // SEE_IMAGES
}

void Grid::populate_sudoku_grid(const cv::Mat & input, vector<cv::Rect>& orderedRect)
{
	for (size_t i = 0; i < orderedRect.size(); ++i)
	{
		int digit = 0;
		cv::Mat src = input(orderedRect[i]);
		this->sudoku_cells.find_cell_contours(src);
		
		int predicted_value = this->parse_cell(this->sudoku_cells.getContour(), src);
		this->sudoku_board.setBoard(i / 9, i % 9, predicted_value);
	}

#if SEE_IMAGES
	cv::namedWindow("Sudoku cell", cv::WINDOW_AUTOSIZE);
	cv::imshow("Sudoku cell", input(orderedRect[0]));
#endif 
}

void Grid::print_sudoku_grid()
{
	this->sudoku_board.printBoard();
}

double Grid::angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

bool sort_x(cv::Rect a, cv::Rect b)
{
	return a.x < b.x;
}

bool sort_y(cv::Rect a, cv::Rect b)
{
	return a.y < b.y;
}

/* Code taken from opencv's sample code "squares.cpp". */
void Grid::findSquares(const cv::Mat & src)
{
	this->squares.clear();
	cv::Mat blur, thresh;
	vector<vector<cv::Point>> contours;

	cv::GaussianBlur(src, blur, cv::Size(3, 3), 0, 0);
	cv::threshold(blur, thresh, this->thresh_val, MAX_THRESH, cv::THRESH_BINARY);

	// find contours and store them all as a list
	cv::findContours(thresh, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

	vector<cv::Point> approx;
	// test each contour
	for (size_t i = 0; i < contours.size(); ++i)
	{
		// approximate contour with accuracy proportional to the contour perimeter
		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);
		
		// square contours should have 4 vertices and area within a range to filter out noise.
		// modify area scaling until all squares can be found
		if (approx.size() == 4 &&
			fabs(cv::contourArea(cv::Mat(approx))) > src.size().area() / 500 &&
			fabs(cv::contourArea(cv::Mat(approx))) < src.size().area() / 50 &&
			cv::isContourConvex(cv::Mat(approx)))
		{
			double maxCosine = 0;
			for (int j = 2; j < 5; j++)
			{
				// maximum cosine of angle between joint edges
				double cosine = fabs(this->angle(approx[j % 4], approx[j - 2], approx[j - 1]));
				maxCosine = MAX(maxCosine, cosine);
			}
			// a small cosine indicates all angles are ~90 degrees, then write vertices to the resultant sequence.
			if (maxCosine < 0.3) this->squares.push_back(approx);
		}
	}
}

void Grid::sortSquares(vector<cv::Rect>& orderedRect)
{
	for (size_t row = 0; row < this->squares.size(); ++row)
	{
		int compare_max = 0, compare_min = INT_MAX;
		cv::Point top_left, bottom_right;
		// sort squares from left to right and top to bottom.
		for (size_t col = 0; col < squares[row].size(); ++col)
		{
			if (this->squares[row][col].x + this->squares[row][col].y < compare_min)
			{
				compare_min = this->squares[row][col].x + this->squares[row][col].y;
				top_left = this->squares[row][col];
			}
			if (this->squares[row][col].x + this->squares[row][col].y > compare_max)
			{
				compare_max = this->squares[row][col].x + this->squares[row][col].y;
				bottom_right = this->squares[row][col];
			}
		}
		cv::Rect pair(top_left, bottom_right);
		orderedRect.push_back(pair);
	}
	std::sort(orderedRect.begin(), orderedRect.end(), sort_y);
	for (size_t i = 0; i < 9; ++i)
	{
		size_t offset = i * 9;
		std::vector<cv::Rect>::iterator start = orderedRect.begin() + offset;
		std::vector<cv::Rect>::iterator end = orderedRect.begin() + offset + 9;
		std::sort(start, end, sort_x);
	}
}

int Grid::parse_cell(std::vector<std::vector<cv::Point>> contour, cv::Mat & src)
{
	for (size_t i = 0; i < contour.size(); ++i)
	{
		// enclose the detected contour inside a circle
		cv::Point2f center; float radius;
		cv::minEnclosingCircle((cv::Mat)contour[i], center, radius);
		radius *= 0.9;

		// if radius and center of the enclosed circle is within the contour, we can say a number has been detected
		if (center.x + radius < src.cols && center.x - radius > 0 &&
			center.y + radius < src.rows && center.y - radius > 0 &&
			src.cols / 2 > center.x - radius && src.cols / 2 < center.x + radius &&
			src.rows / 2 > center.y - radius && src.rows / 2 < center.y + radius)
		{
			cv::Rect roi = cv::boundingRect(cv::Mat(contour[i]));
			cv::Mat dst = src(roi);

			// resize image so that it matches training image size
			cv::resize(dst, dst, cv::Size(15, 15));

			// predict value
			int val = svm_model.predict(dst);
			return val;
		}
	}
	
	// if digit was not found, return 0
	return 0;
}

void Grid::drawSquares(const cv::Mat & image)
{
	cv::Mat temp = image;
	for (size_t i = 0; i < this->squares.size(); ++i)
	{
		const cv::Point* p = &this->squares[i][0];
		int n = (int)this->squares[i].size();
		cv::polylines(temp, &p, &n, 1, true, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
	}

	cv::namedWindow("Detected squares", cv::WINDOW_AUTOSIZE);
	cv::imshow("Detected squares", temp);
}
