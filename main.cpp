#include <stdio.h>
#include <iostream>
#include<opencv2//opencv.hpp>
#include<math.h>
#include "yolo.h"

using namespace std;
using namespace cv;
using namespace dnn;

int main(int argc, char *argv[])
{
	cout << "-----------------------------" << endl;
 	cout << "OpenCV version : " << CV_VERSION << endl;
  	cout << "Major version : " << CV_MAJOR_VERSION << endl;
  	cout << "Minor version : " << CV_MINOR_VERSION << endl;
  	cout << "Subminor version : " << CV_SUBMINOR_VERSION << endl;
	cout << "-----------------------------" << endl;
 	
	string img_path = argc>1?argv[1]:"./image/zidane.jpg";
    string model_path = argc>2?argv[2]:"yolov5s.onnx";
    
	#ifdef YOLO_USE_CUDA
	int num_devices = cv::cuda::getCudaEnabledDeviceCount();
	if (num_devices <= 0) {
		cerr << "There is no cuda." << endl;
		return -1;
	}
	else {
		cout << "There is " << num_devices << " cuda units."<< endl;
		cout << "=============================" << endl;
 	}
	#endif

	Yolo yolo_detector;
	Net net;
	if (yolo_detector.readModel(net, model_path, false)) {
		cout << "read net ok!" << endl;
	}
	else {
		cout << "model not found!" << endl;
		return -1;
	}

	//Generate random colors
	vector<Scalar> color;
	srand(time(0));
	for (int i = 0; i < 80; i++) {
		int b = rand() % 256;
		int g = rand() % 256;
		int r = rand() % 256;
		color.push_back(Scalar(b, g, r));
	}
	// Read Image
	vector<Output> result;
	Mat img = imread(img_path);

	if (yolo_detector.Detect(img, net, result)) {
		yolo_detector.drawPred(img, result, color);

	}
	else {
		cout << "Detect Failed!"<<endl;
	}

	system("pause");
    return 0;
}

