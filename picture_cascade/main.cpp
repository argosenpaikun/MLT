#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
	if(argc != 3) {
		cout << "Usage: " << "./picture_cascade" 
		<< " " << "<image path> " << "<haarcascade.xml>" << endl;
		return -1;
	}

	CascadeClassifier cascade;
	if(!cascade.load(argv[2])) {
		cout << "Error loading cascade file" << endl;
		return -1;
	}

	Mat image, gray;
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	if(!image.data) {
		cout << "File allocation cannot be open or does not exists." << endl;
		return -1;
	}

	cvtColor(image, gray, CV_BGR2GRAY);
	equalizeHist(gray, gray);


	vector<Rect> images;
	cascade.detectMultiScale(gray, images, 1.34, 2, CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING, cvSize(0,0));
	for(int i = 0; i < images.size(); i++) {
		Point pt1(images[i].x + images[i].width, images[i].y + images[i].height);
		Point pt2(images[i].x, images[i].y);
		rectangle(image, pt1, pt2, cvScalar(0,255,0), 1, 8, 0);
	}

	namedWindow("Display", WINDOW_AUTOSIZE);
	imshow("Display", image);
	waitKey(0);

	return 0;
}