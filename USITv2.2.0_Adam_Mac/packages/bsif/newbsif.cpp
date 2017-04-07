#include <cstdio>
#include <map>
#include <iostream>
#include <matio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <time.h>
#include <string>
//#include <mat.h>

using namespace std;

int main(int argc, char *argv[]) {
	if (argc != 3) {
		cout << "Needs two arguments - 1) image 2) texture filter\n" << endl;
	}
	
	// reading in the image with open cv - returns array of doubles
	cv::Mat image = cv::imread(argv[1]);
	image.convertTo(image, CV_64FC1);

	// Opening the matlab file
	const char *dir = "./texturefilters/";
	const char *texturefilter = argv[2];
	char result[100];
	strcpy(result, dir);
	strcat(result, texturefilter);
	cout << "result is " << result << endl;
	// matio call to open file
	mat_t *mat = Mat_Open(result, MAT_ACC_RDONLY);
	matvar_t *matvar;

	// reading in data (number of bits of textureFilter)
	matvar = Mat_VarReadNextInfo(mat)
	numScl = matvar->dims[2];
	Mat_VarFree(matvar);

	Mat_Close(mat);

	return 0;
	

}
