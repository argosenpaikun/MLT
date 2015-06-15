#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <opencv/cv.h>
#include <opencv/highgui.h>

const int MAIN_CAMERA = 0;
class Camera {
public:
	Camera();
	~Camera();

	void Retrieve(cv::Mat *image);
private:
	CvCapture* capture;
};
#endif