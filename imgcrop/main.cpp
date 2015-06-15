#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;
using namespace cv;

Mat src, img, ROI;
Rect cropRect(0, 0, 0, 0);
Point P1(0,0);
Point P2(0,0);

const char* winName = "Crop Image";
bool clicked=false;
int i = 0;
char imgName[15];
string type, folder;
ofstream file;
ifstream readFile("imgcrop.info");

void checkBoundary() {
	// check cropping rectangle exceed image boundary
	if(cropRect.width>img.cols-cropRect.x)
		cropRect.width=img.cols-cropRect.x;

	if(cropRect.height>img.rows-cropRect.y)
		cropRect.height=img.rows-cropRect.y;

	if(cropRect.x<0)
		cropRect.x=0;
	if(cropRect.y<0)
		cropRect.height=0;
}

void showImage() {
	img=src.clone();
	checkBoundary();
	if(cropRect.width>0 && cropRect.height>0) {
		ROI=src(cropRect);
		imshow("cropped", ROI);
	}

	rectangle(img, cropRect, Scalar(0,255,0), 1, 8, 0);
	imshow(winName, img);
}

void onMouse(int event, int x, int y, int f, void*) {
	switch(event) {
	case CV_EVENT_LBUTTONDOWN:
		clicked=true;
		P1.x=x;
		P1.y=y;
		P2.x=x;
		P2.y=y;
		break;
	case CV_EVENT_LBUTTONUP:
		P2.x=x;
		P2.y=y;
		clicked=false;
		break;
	case CV_EVENT_MOUSEMOVE:
		if(clicked) {
			P2.x=x;
			P2.y=y;
		} break;
	default: break;
	}

	if(clicked) {
		if(P1.x>P2.x) {
			cropRect.x = P2.x;
			cropRect.width = P1.x-P2.x;
		} else {
			cropRect.x = P1.x;
			cropRect.width = P2.x-P1.x;
		}

		if(P1.y>P2.y) {
			cropRect.y = P2.y;
			cropRect.height = P1.y-P2.y;
		} else {
			cropRect.y = P1.y;
			cropRect.height = P2.y-P1.y;
		}
	}
	showImage();
}

int main(int argc, char** argv) {
	string line;
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", timeinfo);
	string str(buffer);

	if(argc != 4) {
		cout << "Usage: ./imgcrop " << "<image path> <saved path> <type>\n"
			<< "[<image path> location of selected image]\n"
			<< "[<save path> location of cropped image to be saved (C:\\image\\data\\ or image\\data\\)]\n"
			<< "[<type> (pos | neg)]\n"
			<< endl;
		return -1;
	}

	src = imread(argv[1],1);
	folder = argv[2];
	type = argv[3];

	if(CreateDirectory(folder.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
		string folderCmd = "mkdir " + folder;
		system(folderCmd.c_str());
	}

	if(!src.data) {
		cout << "Error image unable to access or does not exists." << endl;
		return -1;
	} 
	
	if(type == "pos" || type == "neg") {
		cout << "Click and drag for Selection" << endl << endl;
		cout << "------> Press 's' to save" << endl << endl;

		cout << "------> Press '8' to move up" << endl;
		cout << "------> Press '2' to move down" << endl;
		cout << "------> Press '6' to move right" << endl;
		cout << "------> Press '4' to move left" << endl << endl;

		cout << "------> Press 'w' increase top" << endl;
		cout << "------> Press 'x' increase bottom" << endl;
		cout << "------> Press 'd' increase right" << endl;
		cout << "------> Press 'a' increase left" << endl << endl;

		cout << "------> Press 't' decrease top" << endl;
		cout << "------> Press 'b' decrease bottom" << endl;
		cout << "------> Press 'h' decrease right" << endl;
		cout << "------> Press 'f' decrease left" << endl << endl;

		cout << "------> Press 'g' resize image" << endl << endl;

		cout << "------> Press 'r' to reset" << endl;
		cout << "------> Press 'c' check current data" << endl;
		cout << "------> Press 'd' reset data" << endl;
		cout << "------> Press 'Esc' to quit" << endl << endl;

		cout << "Note: Please check the current imgcrop.data data before saving the cropped image." << endl << endl;

		namedWindow(winName, WINDOW_NORMAL);
		setMouseCallback(winName, onMouse, NULL);
		imshow(winName, src);

		while(true) {
			if(readFile.is_open()) {
				while(getline(readFile, line)) {
					i = atoi(line.c_str());
				}
				readFile.close();
			}

			char c = waitKey();
			if(c == 's' && ROI.data) {
				sprintf(imgName, "%d.jpg", i++);
				imwrite(folder + type + "-" + imgName, ROI);
			
				cout << type << "-" << imgName << " created on " << str << endl;

				file.open("imgcrop.info");
				file << i;
				file.close();

			}
			if(c=='6') cropRect.x++;
			if(c=='4') cropRect.x--;
			if(c=='8') cropRect.y--;
			if(c=='2') cropRect.y++;

			if(c=='w') { cropRect.y--; cropRect.height++; }
			if(c=='d') cropRect.width++;
			if(c=='x') cropRect.height++;
			if(c=='a') { cropRect.x--; cropRect.width++; }

			if(c=='t') { cropRect.y++; cropRect.height--; }
			if(c=='h') cropRect.width--;
			if(c=='b') cropRect.height--;
			if(c=='f') { cropRect.x++; cropRect.width--; }

			if(c==27) break;
			if(c=='r') {
				cropRect.x = 0;
				cropRect.y = 0;
				cropRect.width = 0;
				cropRect.height = 0;
			}
			if(c=='d') {
				file.open("imgcrop.info");
				file << 0;
				file.close();

				i = 0;
				cout << "imgcrop.data is reseted on " << str << endl;
			}
			if(c=='c') {
				if(readFile.is_open()) {
					while(getline(readFile, line)) {
						i = atoi(line.c_str());
					}
					readFile.close();
				}
				cout << "imgcrop.data current data is " << i << endl;
			}
			if(c=='g') {
				int width, height;

				cout << "Resizing " << argv[1] << "\n\n"
					<< "Note:\n"
					<< "Select the command line to enter width and height.\n" 
					<< "Diselect the Crop Image windows first and do not hit key any while the crop image windows is selected.\n"
					<< "It may cause uncertain functions on the program." << endl << endl;
				cout <<"Warning: This process cannot be undone after the selected image was resized." << endl;
				try {
					cout << " - width  : ";
					cin >> width;
					cout << " - height : ";
					cin >> height;

					Size size(width, height);
					resize(src, src, size);

					cout << argv[1] << " resized on " << str << endl; 
				} catch(exception& e) {
					cout << e.what() << endl << endl;
				}
			}
			showImage();
		}
	} else {
		cout << "Invalid type name, requires pos or neg" << endl;
		return -1;
	}
	
	return 0;
}