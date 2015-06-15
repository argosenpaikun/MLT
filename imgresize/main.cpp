#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string.h>
#include <fstream>
#include <ctime>
#include <Windows.h>
#include "linkedlist.h"
#include "stack.h"
using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	
	if(argc != 5) {
		cout << "Usage: ./imgresize <image path> <type pos | neg> <w> <h>\n"
			<< "Note: This process cannot be undo. Use with cautions\n"
			<< endl;
		return -1;
	}

	LinkedList list;
	Stack stack;
	string imagePath = argv[1], type = argv[2];
	int w = atoi(argv[3]), h = atoi(argv[4]);
	int num = 0;

	if(type == "pos" || type == "neg") {
		while(true) {
			string imgPath = imagePath + type +  "-" + to_string(static_cast<long long>(num)) + ".jpg";
			Mat image = imread(imgPath, 1);

			if(!image.data) {
				time_t rawtime;
				struct tm* timeinfo;
				char buffer[80];

				time(&rawtime);
				timeinfo = localtime(&rawtime);

				strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", timeinfo);
				string str(buffer);

				if(!stack.isEmpty()) {
					while(!stack.isEmpty()) {
						list.insert(stack.top());
						stack.pop();
					}

					Size size(w, h);
					for(int i = 1; i < list.getLength() + 1; i++) {
						Mat img = imread(list.retrieve(i), 1);
						Mat resizeImg;
						resize(img, resizeImg, size);

						string pathImg = list.retrieve(i);
						imwrite(pathImg, resizeImg);

						cout << pathImg << " created " << str << endl; 
					}
				} else {
					cout << "Error allocation file unable to access or does not exists!\n"
						<< "Note: requires image file named pos-0++.jpg or neg-0++.jpg.\n"
						<< endl;
					return -1;
				}
				break;
			} else {
				stack.push(imgPath);
				cout << imgPath << endl;
				++num;
			}
		}
	} else {
		cout << "Invalid type, requires pos or neg.\n" << endl;
		return -1;
	}
	return 0;
}