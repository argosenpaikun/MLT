#include <cstdlib>

#include "camera.h"
#include "exceptions.h"

#include <iostream>

Camera::Camera() {
	capture = cvCaptureFromCAM(MAIN_CAMERA);
	if(!capture) {
		throw CameraException();
	}
}

Camera::~Camera() {
	if(capture) {
		cvReleaseCapture(&capture);
	}
}

void Camera::Retrieve(cv::Mat *image) {
	IplImage *frame = cvQueryFrame(capture);
	*image = cv::Mat(frame);
}