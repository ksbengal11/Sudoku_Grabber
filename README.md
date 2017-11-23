# Sudoku Grabber
A project I worked on to become familiar with OpenCV applications.

----
## Goal
Extract sudoku grid and its digits from an image. 

----
## Steps
1. In the image, find contour with the largest area and extract sudoku grid
2. Inside the grid, detect shapes that resemble squares
3. Within each square, find contours and enclose them inside a circle. Digits are present if circle area is less than the contour area and circle center is inisde the contour
4. Using a dataset of number images, train a support vector machine model and save the output
5. Recognize detected digits using the trained model

---
## Improvements
The dectection accuray is not very good. Improvements could include using a larger training dataset, modifying svm parameters, or using a different learning model

---
### Contact
karanbengali4@gmail.com

My inspiration for this project came from the following blog: http://aishack.in/tutorials/sudoku-grabber-opencv-detection/
