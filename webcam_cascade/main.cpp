#include <stdio.h>
#include <math.h>
#include <opencv/cv.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <Windows.h>

#include "camera.h"

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
	if(argc != 2) {
		cout << "Usage: " << "./webcam_cascade " << "<haarcascade>" << endl;
		return -1;
	}
	
	CascadeClassifier cascade;
	if(!cascade.load(argv[1])) {
		cout << "Error loading cascade file." << endl;
		return -1;
	}

	Camera camera;
	vector<Rect> images;
	namedWindow("Display", CV_WINDOW_AUTOSIZE);
	while(true) {
		Mat frame, gray_img(1, 256, CV_32F);
		camera.Retrieve(&frame);
		Mat flippedFrame;
		flip(frame, flippedFrame, 1);
		cvtColor(flippedFrame, gray_img, CV_BGR2GRAY);
		equalizeHist(gray_img, gray_img);
		cascade.detectMultiScale(gray_img, images, 1.34, 2, CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING, cvSize(100,100), cvSize(700,700));
		for(int i = 0; i < images.size(); i++) {
			Point pt1(images[i].x + images[i].width, images[i].y + images[i].height);
			Point pt2(images[i].x, images[i].y);
			rectangle(flippedFrame, pt1, pt2, cvScalar(0,255, 0), 2, 8, 0);
		}
		imshow("Display", flippedFrame);
		int c = waitKey(1);
		if(c == 27) { break; }
	}
	return 0;
}