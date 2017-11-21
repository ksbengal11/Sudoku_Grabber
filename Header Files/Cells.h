#pragma once
#include "libraries.h"

class Cells
{
public:
	/* Default contructor and destructor. */
	Cells() {}
	virtual ~Cells() {}

	/*	@breif Function for returning the cell contour.
		@returns Cell contour.
	*/
	std::vector<std::vector<cv::Point>> getContour()
	{
		return this->contours;
	}

	/*	@brief Function for finding contours in an image of a cell.
		Process cell and use opencv findContours function to find the contours in the input image.
		@params src Image of a cell.
	*/
	void find_cell_contours(cv::Mat &src)
	{
		cv::Mat temp = src;
		this->process_cell(temp);
		cv::findContours(temp, this->contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	}

private:
	/*	@params contours Varaible for storing contours. */
	std::vector<std::vector<cv::Point>> contours;

	/*	@brief Processing image for contour detection.
		Applying a gaussian filter and adaptive threshold to an image.
		@params src Input image.
	*/
	void process_cell(const cv::Mat &src)
	{
		cv::GaussianBlur(src, src, cv::Size(3, 3), 0, 0);
		cv::adaptiveThreshold(src, src, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 11, 2);
	}
};