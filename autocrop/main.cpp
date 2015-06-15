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

bool is_border(Mat& edge, Vec3b color) {
	Mat im = edge.clone().reshape(0,1);

	bool res = true;
	for(int i = 0; i < im.cols; ++i)
		res &= (color == im.at<Vec3b>(0, i));

	return res;
}

void autocrop(Mat& src, Mat& dst) {
	Rect win(0,0, src.cols, src.rows);

	vector<Rect> edges;
	edges.push_back(Rect(0, 0, src.cols, 1));
	edges.push_back(Rect(src.cols-2, 0, 1, src.rows));
	edges.push_back(Rect(0, src.rows-2, src.cols, 1));
	edges.push_back(Rect(0, 0, 1, src.rows));

	Mat edge;
	int nborder = 0;
	Vec3b color = src.at<Vec3b>(0,0);

	for(int i = 0; i < edges.size(); ++i) {
		edge = src(edges[i]);
		nborder += is_border(edge, color);
	}

	if(nborder < 4) {
		src.copyTo(dst);
		return;
	}

	bool next;

	do {
		edge = src(Rect(win.x, win.height-2, win.width, 1));
		if(next = is_border(edge, color))
			win.height--;
	} while(next && win.height > 0);

	do {
		edge = src(Rect(win.width-2, win.y, 1, win.height));
		if(next = is_border(edge, color))
			win.width--;
	} while(next && win.width > 0);

	do {
		edge = src(Rect(win.x, win.y, win.width, 1));
		if(next = is_border(edge, color))
			win.y++, win.height--;
	} while(next && win.y <= src.rows);

	do {
		edge = src(Rect(win.x, win.y, 1, win.height));
		if(next = is_border(edge, color))
			win.x++, win.width--;
	} while(next && win.x <= src.cols);

	dst = src(win);
}

int main(int argc, char** argv) {
	if(argc != 4) {
		cout << "Usage: ./autocrop_nnetwork <path> <crop path> <type>\n"
			<< "[<path> location of image path (C:\\image\\path or image\\path\\)]\n"
			<< "[<crop path> location of crop path (C:\\image\\path or image\\path\\)]\n" 
			<< "[<type> pos (positive) | neg (negative)]"
			<< endl;
		return -1;
	}

	LinkedList list;
	Stack stack;
	int num = 0;
	string path = argv[1], filePath = argv[2];
	string type = argv[3];

	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", timeinfo);
	string str(buffer);

	if(type == "pos" || type == "neg") {
		while(true) {
			string imagePath = path + "img-" + to_string(static_cast<long long>(num)) + ".jpg";

			Mat image;
			image = imread(imagePath, 1);
			if(!image.data) {
			

				if(!stack.isEmpty()) {
					while(!stack.isEmpty()) {
						list.insert(stack.top());
						stack.pop();
					}

					if(CreateDirectory(filePath.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
						string folderCmd = "mkdir " + filePath;
						system(folderCmd.c_str());
					}

					for(int i = 1; i < list.getLength() + 1; i++) {
						Mat img = imread(list.retrieve(i), 1);
						Mat output;

						GaussianBlur(img, output, Size(5,5), 0);
						threshold(output, output, 50, 255, 0);
						autocrop(output, output);
					
						string fileImage = filePath + type + "-" + to_string(static_cast<long long>(i - 1)) + ".jpg";
						imwrite(fileImage, output);

						cout << fileImage << " created " << str << endl;
					}
				}
				else {
					cout << "File allocation unable to open or does not exists." << endl;
					cout << "Note: Please ensure the selected path of image were named img-0++." << endl;
					return -1;
				}
				break;
			} else {
				stack.push(imagePath);
				cout << imagePath << endl;
				++num;
			}
		}
	} else {
		cout << "Error invalid type, requires pos or neg." << endl;
		return -1;
	}
	
	return 0;
}