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

int main(int argc, char** argv) {
	if(argc != 5) {
		cout << "Usage: ./imgcopy <image> <type> <copy> <path>\n"
			<< endl;
		return -1;
	}

	string imagePath = argv[1], type = argv[2], path = argv[4];
	int copy = atoi(argv[3]);

	if(type == "pos" || type == "neg") {
		Mat img = imread(imagePath, 1);

		if(!img.data) {
			cout << "Error, allocation unable to access or does not exists!\n"
				<< endl;
			return -1;
		}

		string line;
		time_t rawtime;
		struct tm* timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", timeinfo);
		string str(buffer);

		if(CreateDirectory(path.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
			string folderCmd = "mkdir " + path;
			system(folderCmd.c_str());
		}

		for(int i = 0; i < copy; i++) {
			string pathImg = path + type + "-" + to_string(static_cast<long long>(i)) + ".jpg";
			imwrite(pathImg, img);
			cout << pathImg << " created " << str << endl;
		}
	}
	
	return 0;
}