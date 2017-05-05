#include <cstdio>
#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <math.h>
#include <stdio.h>
#include <mat.h>

using namespace std;

int main(int argc, char *argv[]) {
	if (argc != 3) {
		cout << "Needs two arguments - 1) image 2) texture filter\n" << endl;
	}
	
	// reading in the image with open cv - returns array of doubles
	cv::Mat image = cv::imread(argv[1], 0);

	// Opening the matlab file
	const char *dir = "./texturefilters/";
	const char *texturefilter = argv[2];
	char result[100];
	strcpy(result, dir);
	strcat(result, texturefilter);

	// MAT.H
	MATFile *pmat;
	mxArray *pa;
	const char *name;

	// Reopen file to read in actual arrays.
  	pmat = matOpen(result, "r");
  	if (pmat == NULL) {
    		printf("Error reopening file %s\n", result);
    		return(1);
  	}

  	//Read in each array. 
	pa = matGetNextVariable(pmat, &name);
	if (pa == NULL) {
		printf("Error reading in file %s\n", result);
		return(1);
	} 

	// Reading in dimensions from mat file
	const size_t *dims = mxGetDimensions(pa);
	int numScl = (int) dims[2];
	int r = (int) dims[0];
	

	// Closing mat file
	if (matClose(pmat) != 0) {
		printf("Error closing file %s\n",result);
		return(1);
	}

	r = floor(r/2);
	//cout << "numScl is " << numScl << endl;
	
	//initializing matrix of 1s
	double codeImg[image.rows][image.cols];
	for (int i = 0; i < image.rows; i++){
		for (int j = 0; j < image.cols; j++){
			codeImg[i][j] = 1;
		}
	}


	// creates the border around the image - it is wrapping
	int border = r;
	int bordertype = cv::BORDER_WRAP;
	cv::Mat imgWrap(image.rows + r*2, image.cols + r*2, CV_64FC1);
	cv::copyMakeBorder(image, imgWrap, border, border, border, border, bordertype);

	// Loop over scales
	cv::Mat ci; // the textured image after filter
	cv::Mat tmpcv; // the texture
	int count = 0, arrcount = 0; 
	double tmp[dims[0]*dims[1]];
	int itr = 0;
	// pr is the array of filter
	double *pr;
	pr = (double *)mxGetData(pa);

	// pull the data from the matfile into an array
	// the matlab file is in one long single array
	// we need to start w/ the last filter and work our way forward
	for (int i = numScl - 1; i >= 0; i--){
		// Diagnose array pa
		count = dims[0]*dims[1]*i; //last filter
		arrcount = 0;
		for (int j = 0; j < dims[0]; j++){
			for (int k = 0; k < dims[1]; k++){
				// place it into respective spot in real array
				tmp[arrcount+(k*dims[0])] = pr[count];
				count+=1;
			}
			arrcount+=1;
		}
		//convert the array into matlab object to use w/ filter
		tmpcv = cv::Mat(dims[0],dims[1], CV_64FC1, &tmp);

		// running the filter on the image w/ BORDER WRAP - equivalent to filter2 in matlab
		cv::filter2D(imgWrap, ci, CV_64FC1, tmpcv, cv::Point(-1, -1), 0);

		// This will convert any positive values in the matrix
		// to 2^(i-1) as it did in the matlab software
		for (int j = 0; j < image.rows; j++){
			for (int k = 0; k < image.cols; k++){
				if (ci.at<double>(j,k) > 0)
					codeImg[j][k] = codeImg[j][k] + pow(2,itr);
			}
		}
		itr++;
		//cout << "Print statement:  " << i << endl;

	}

	// Creating the histogram
	int histsize = pow(2, dims[2]);
	int histogram[histsize];
	memset(histogram, 0, histsize*sizeof(int));
	for (int j = 0; j < image.rows; j++){
		for (int k = 0; k < image.cols; k++){
			histogram[(int)codeImg[j][k]]++;
		}
	}
	
	// Outputting to a CSV file
	ofstream histfile;
	histfile.open("histogram.csv", ios::out | ios::trunc);
	for (int i = 0; i < histsize; i++)
		histfile << histogram[i] << ",";
	histfile.close();

	mxDestroyArray(pa);


	return 0;
	

}
