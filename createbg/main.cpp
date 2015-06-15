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

	if(argc != 3) {
		cout << "Usage: ./createinfo <path> <filename>\n"
			<< " [<path> path location (C:/image/path/ or image/path/]\n"
			<< " [<filename> filename for .txt]\n"
			<< endl;
		return -1;
	}

	LinkedList list;
	Stack stack;
	int num = 0;
	string path = argv[1], filename = argv[2];
	while(true) {
		string imagePath = path + "neg-" + to_string(static_cast<long long>(num)) + ".pgm";

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

				string filePath = filename + ".txt";
				ofstream file;
				file.open(filePath);

				for(int i = 1; i < list.getLength() + 1; i++) {
					file << list.retrieve(i) << endl;
				}
				file.close();
				cout << filename << ".txt" << " created " << str << endl;
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