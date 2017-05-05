#include <cstdio>
#include <vector>
#include <map>
#include <iostream>
#include <matio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <time.h>
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
	int size_of_x = (2*r) + 1;
	
	//initializing matrix of 1s
	double codeImg[image.rows][image.cols];
	for (int i = 0; i < image.rows; i++){
		for (int j = 0; j < image.cols; j++){
			codeImg[i][j] = 1;
		}
	}
	double x[size_of_x];
	int elm = -r;
	for (int i = 0; i < size_of_x; i++){
		x[i] = elm;
		elm++;
	}

	// wrapping the image - currently semi hardcoded - can be changed later
	int border = r;
	int bordertype = cv::BORDER_WRAP;
	//cv::Mat imgWrap(image.rows + r*2, image.cols + r*2, CV_8UC1);
	//cv::copyMakeBorder(image, imgWrap, border, border, border, border, bordertype);

	//cout << imgWrap << endl;

	// Loop over scales
	cv::Mat ci;
	cv::Mat tmpcv;
	int count = 0, arrcount = 0; 
	double tmp[dims[0]*dims[1]];
	int itr = 0;
	for (int i = numScl - 1; i >= 0; i--){
		// Diagnose array pa
		count = 121*i;
		arrcount = 0;
		double *pr;
		pr = (double *)mxGetData(pa);
		for (int j = 0; j < dims[0]; j++){
			for (int k = 0; k < dims[1]; k++){
				tmp[arrcount+(k*dims[0])] = pr[count];

				count+=1;
			}
			arrcount+=1;
			//cout << endl;
		}
		tmpcv = cv::Mat(11,11, CV_64FC1, &tmp);
		//cout << tmpcv << endl;

		cv::filter2D(image, ci, CV_64FC1, tmpcv, cv::Point(-1, -1), 0, cv::BORDER_WRAP);

		//cout << ci << endl;
		for (int j = 0; j < image.rows; j++){
			for (int k = 0; k < image.cols; k++){
				if (ci.at<double>(j,k) > 0)
					codeImg[j][k] = codeImg[j][k] + pow(2,itr);
			}
		}
		itr++;

	}

	int histogram[256] = {0};
	for (int j = 0; j < image.rows; j++){
		for (int k = 0; k < image.cols; k++){
			histogram[(int)codeImg[j][k]]++;
		}
	}
	for (int i = 0; i < 256; i++)
		cout << histogram[i] << ",";

	mxDestroyArray(pa);












	/*
	// matio call to open file
	mat_t *mat = Mat_Open(result, MAT_ACC_RDONLY);
	matvar_t *matvar;

	// reading in data (number of bits of textureFilter)
	matvar = Mat_VarReadNextInfo(mat);
	int numScl = matvar->dims[2];
	int r = matvar->dims[0];
	// make spatial coordinates for sliding window
	r = floor(r/2);
	cout << "numScl is " << matvar->dims[0] << endl;
//	cout << matvar->compression << endl;
//cout << (double*)(matvar->data)<<endl;
//	double* data = (double*)(matvar->data);
//	cout << data[0] << endl;
	int size_of_x = (2*r) + 1;
	Mat_VarFree(matvar);
	Mat_Close(mat);
	
	//initializing matrix of 1s
	//vector<vector<double>> codeImg;
	double codeImg[image.rows][image.cols];
	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++)
			codeImg[i][j] = 1;
	double x[size_of_x];
	int elm = -r;
	for (int i = 0; i < size_of_x; i++){
		x[i] = elm;
		elm++;
	}

	// wrapping the image - currently semi hardcoded - can be changed later
	int border = r;
	int bordertype = cv::BORDER_WRAP;
	cv::Mat buffer(image.rows + r*2, image.cols + r*2, CV_8UC1);
	cv::copyMakeBorder(image, buffer, border, border, border, border, bordertype);
	//cout << buffer << endl;

	// Loop over scales
	for (int i = 1; i < numScl; i++){

	}

*/
	return 0;
	

}
