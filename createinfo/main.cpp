#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include "linkedlist.h"
#include "stack.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	if(argc != 8) {
		cout << "Usage: ./createinfo <path> <object> <x> <y> <width> <height> <filename>\n"
			<< " [<path> path location (C:/image/path/ or image/path/]\n"
			<< " [<object> number of object on the image]\n"
			<< " [<x> location of object x]\n"
			<< " [<y> location of object y]\n"
			<< " [<width> width of image]\n"
			<< " [<height> height of image]\n"
			<< " [<filename> filename for .info]\n"
			<< endl;
		return -1;
	}

	LinkedList list;
	Stack stack;
	int num = 0;
	string path = argv[1], filename = argv[7];
	while(true) {
		string imagePath = path + "pos-" + to_string(static_cast<long long>(num)) + ".pgm";

		Mat image;
		image = imread(imagePath, CV_LOAD_IMAGE_COLOR);
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

				string filePath = filename + ".info";
				ofstream file;
				file.open(filePath);

				for(int i = 1; i < list.getLength() + 1; i++) {
					file << list.retrieve(i) << " " << argv[2] 
					<< " " << argv[3] << " " << argv[4] << " " << argv[5] << " " << argv[6] 
					<< endl;
				}
				file.close();
				cout << filename << ".info" << " created " << str << endl;
			}
			else {
				cout << "File allocation unable to open or does not exists." << endl;
				return -1;
			}
			break;
		} else {
			stack.push(imagePath);
			cout << imagePath << endl;
			++num;
		}
	}
	
	return 0;
}