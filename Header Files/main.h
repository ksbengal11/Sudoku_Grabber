#pragma once
#include "libraries.h"
#include "Grid.h"
#include "MachineLearning.h"

const std::string SUDOKU_IMAGE_PATH = "Images/sudoku_2.png";
using namespace std;
using namespace cv;

#if !defined VK_ESCAPE
#define VK_ESCAPE 0x1B
#endif

#define TRAIN 0