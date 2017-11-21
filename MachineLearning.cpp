#include "MachineLearning.h"

MachineLearning::MachineLearning(bool trained)
{
	this->trained = trained;
	if (!trained) {
		svm = SVM::create();
		this->setParams();

		this->read_csv(CSV_FILE_LOCATION, this->images, this->labels);
		this->train(images, labels);
	}
	else
	{
		svm = SVM::load("digit_classifier.yml");
	}
}

MachineLearning::~MachineLearning()
{
}

int MachineLearning::predict(const cv::Mat & src)
{
	std::vector<cv::Mat> images;
	images.push_back(src);
	cv::Mat array1D;

	this->convert_to_ml(images, array1D);

	return (int) svm->predict(array1D);
}

void MachineLearning::read_csv(const std::string filename, std::vector<cv::Mat>& images, std::vector<int>& labels, char separator)
{
	std::ifstream file(filename.c_str(), std::ifstream::in);
	if (!file)
	{
		std::string error_message = "No valid input file was given. Exiting program now.\n";
		CV_Error(CV_StsBadArg, error_message);
	}
	std::string line, path, classlabel;
	while (getline(file, line))
	{
		std::stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty())
		{
			images.push_back(cv::imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
	std::cout << images[0].size << std::endl;
}

void MachineLearning::convert_to_ml(const std::vector<cv::Mat>& images, cv::Mat & trainData)
{
	const int rows = (int)images[0].rows;
	const int cols = (int)images[0].cols;

	const int numImages = images.size();
	trainData = cv::Mat(numImages, rows*cols, CV_32FC1);

	for (int i = 0; i < numImages; ++i)
	{
		cv::Mat temp = images[i].reshape(1, 1);
		temp.copyTo(trainData.row(i));
	}
}

void MachineLearning::setParams()
{
	/* Default values SVM */
	svm->setCoef0(0.0);
	svm->setDegree(3);
	svm->setTermCriteria(cv::TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, 1e-3));
	svm->setGamma(0);
	svm->setKernel(cv::ml::SVM::LINEAR);
	svm->setNu(0.5);
	svm->setP(0.1); // for EPSILON_SVR, epsilon in loss function?
	svm->setC(0.01); // From paper, soft classifier
	svm->setType(cv::ml::SVM::EPS_SVR); // C_SVC; // EPSILON_SVR; // may be also NU_SVR; // do regression task
}

void MachineLearning::train(const std::vector<cv::Mat>& src, const std::vector<int>& labels)
{
	cv::Mat trainData;
	this->convert_to_ml(this->images, trainData);

	std::cout << "Start training..." << std::endl;
	svm->train(trainData, cv::ml::ROW_SAMPLE, cv::Mat(labels));

	// Save svm detector so we don't have to repeat the training
	svm->save("digit_classifier.yml");
	std::cout << "Training complete." << std::endl;
}