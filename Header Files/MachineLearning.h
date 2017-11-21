#pragma once

#include "libraries.h"
#include <opencv2\ml\ml.hpp>

/* @param CSV_FILE_LOCATION Path to the CSV file containing image paths. */
const std::string CSV_FILE_LOCATION = "C:\\Users\\Karan\\Documents\\Visual Studio 2015\\Projects\\Sudoku_Solver\\Sudoku_Solver\\image_path.csv";
using namespace cv::ml;

class MachineLearning
{
public:
	/*	@brief Default constructor of the MachineLearning class. */
	MachineLearning() {}

	/*	@brief Constructor of the MachineLearning class.
		Trains a supervised machine learning model if it hasn't previously been trained.
		@params trained	Argument for determining if model has been previously trained.
	*/
	MachineLearning(bool trained);
	
	/*	@brief Default destructor of the MachineLearning class. */
	virtual ~MachineLearning();

	/*	@brief Predicts the value in the given image. 
		@params src Input image.
		@returns value Predicted value of the number found in the image.
	*/
	int predict(const cv::Mat &src);

private:
	/*	@param trained Variable to determine if model has been previously trained.	*/
	bool trained;

	/*	@param svm Supervised machine learning model. */
	cv::Ptr<SVM> svm;

	/*	@param images Vector of training images. */
	std::vector<cv::Mat> images;

	/*	@param labels Vector of training labels. */
	std::vector<int> labels;

	/*	@brief Function for reading images from paths found in csv file. 
		@param filename	Filename of the csv.
		@param images Vector of images to be updated.
		@param labels Vector of image labels to be updated.
		@param separator Character sepearating image path and image label in the csv file.
	*/
	void read_csv(const std::string filename, std::vector<cv::Mat> &images, std::vector<int> &labels, char separator = ';');

	/*	@brief Function for converting a 2D Matrix to a 1D Matrix. 
		@param images Vector of 2D Matrices to be converted to 1D.
		@param trainData 1D Matrix of all images.
	*/
	void convert_to_ml(const std::vector<cv::Mat> &images, cv::Mat &trainData);

	/*	@breif Function for configuring default SVM parameters. */
	void setParams();

	/*	@brief Function for training machine learning model. 
		@param src Matrix containing a 1D representation of training images.
		@param labels Vector of training image labels.
	*/
	void train(const std::vector<cv::Mat> &src, const std::vector<int> &labels);
};