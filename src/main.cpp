#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <stdint.h>

using namespace cv;
/**
checks pixel represented by a rgb array (src) and determined if it's a "hot" pixel or not
note that the vals are 8 bit unsigned numerics
*/
bool check_hit(char src[]){
	uint8_t l_cut[3] = {0,100,0}; //lower cutoff
	uint8_t u_cut[3] = {150,230,150};//
	uint8_t temp;
	for (int i = 0; i < 3; i++)
	{
		temp = src[i];
		if(temp<l_cut[i]||temp>u_cut[i]){
			return false;
		}
	}		
	return true;
}
/**
takes a image and returns a b and w image
target areas are 255(white), non target are 0 (black) 
*/
Mat threshIm(Mat image_in){
	Mat channels[3];
	Mat mask(image_in.size(),CV_8UC1);
	mask = Mat::zeros(mask.rows,mask.cols,CV_8U);
	split(image_in,channels);
	uint8_t hit = 255; // remeber has got to be a 8bit datatype
	uint8_t temp[3];

	for (int i = 0; i < (mask.rows); i++)
	{
			for (int j = 0; j < mask.cols; j++)
			{	
					for (int k = 0; k < 3; k++)
					{
						temp[k]=channels[k].at<char>(i,j);
					}
					if(check_hit(temp))
						mask.at<char>(i,j) = hit; //make sure to actually use the correct ref (<char>) otherwise shit overflows
			}
			
	}
	return mask;
}

/**
main method
at present it loads a source image, processes the image then displays both
*/
int main(int argc, char** argv)
{ 
	if (argc!=2)
	{
		printf("%s\n","Usage: ./binary imagePath" );
		return -1;
	}
	Mat src;
	Mat proc;
	src = imread(argv[1], IMREAD_COLOR); // reads file in path provided

	if (!src.data)
	{
		printf("%s\n","image not found" );
		return -1;
	}
	//image proc section
	proc = threshIm(src);

	cv::namedWindow( "Source_Image", WINDOW_AUTOSIZE ); // Create a window for display.
    cv::imshow( "Source_Image", src );                // Show our image inside it.

    cv::namedWindow( "Processed_Image", WINDOW_AUTOSIZE ); // Create a window for processed image.
    cv::imshow( "Processed_Image", proc );                

    cv::waitKey(0); // Wait for a keystroke in the window
	return 0;
}
