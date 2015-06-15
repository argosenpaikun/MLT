#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include "linkedlist.h"
#include "stack.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	if(argc != 4) {
		cout << "Usage: ./createpgm <path> <type pos|neg> <image type>\n"
			<< "[<path> path location (C:/iamge/path/ or image/path/]\n"
			<< "[<type> pos (positive) | neg (negative)]\n"
			<< "[<image type> type of selected image path (*.jpg, *.png, *.*)]"
			<< "Note:\n"
			<< "Please ensure the selected path of the image are named\n"
			<< "pos-0++ / neg-0++ *.jgp, *.png, *.*"
			<< endl;
		return -1;
	}

	LinkedList list, listPath;
	Stack stack, stackPath;
	int num = 0;
	string path = argv[1], type = argv[2], imageType =  argv[3];

	if(type == "pos" || type == "neg") {
		while(true) {
			string readPath = path + type + "-" + to_string(static_cast<long long>(num)) + imageType;
			string pgmPath = path + type + "-" + to_string(static_cast<long long>(num));

			Mat image;
			image = imread(readPath, CV_LOAD_IMAGE_COLOR);
			if(!image.data) {

				time_t rawtime;
				struct tm* timeinfo;
				char buffer[80];

				time(&rawtime);
				timeinfo = localtime(&rawtime);

				strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", timeinfo);
				string str(buffer);

				if(!stack.isEmpty()) {
					while(!stack.isEmpty() && !stackPath.isEmpty()) {
						list.insert(stack.top());
						stack.pop();
						listPath.insert(stackPath.top());
						stackPath.pop();
					}

					if(list.getLength() == 1) {
						cout << "Invalid process, requires more than 1 image." <<endl;
						return -1;
					}

					for(int i = 1; i < list.getLength() + 1; i++) {
						Mat images, gray_image;
						images = imread(listPath.retrieve(i), 1);
						cvtColor(images, gray_image, CV_BGR2GRAY);
						imwrite(list.retrieve(i) + ".pgm", gray_image);

						cout << list.retrieve(i) + ".pgm created " << str << endl;
					}
				}
				else {
					cout << "File allocation unable to open or unable to perform.\n\n"
						<< "Note:\n"
						<< "Please name or renamed the selected path file to\n"
						<< "pos-0++ / neg-0++ *.jpg, *.png, *.*\n"
						<< endl;
					return -1;
				}
				break;
			} else {
				stackPath.push(readPath);
				stack.push(pgmPath);
				cout << readPath << endl;
				++num;
			}
		}
	} else {
		cout << "Error invalid type (requires pos or neg only)" << endl;
		return -1;
	}

	return 0;
}