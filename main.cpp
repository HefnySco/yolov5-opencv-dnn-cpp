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
	string img_path = argc>1?argv[1]:"./image/zidane.jpg";
    string model_path = argc>2?argv[2]:"yolov5s.onnx";
    
	//int num_devices = cv::cuda::getCudaEnabledDeviceCount();
	//if (num_devices <= 0) {
		//cerr << "There is no cuda." << endl;
		//return -1;
	//}
	//else {
		//cout << num_devices << endl;
	//}

	Yolo test;
	Net net;
	if (test.readModel(net, model_path, false)) {
		cout << "read net ok!" << endl;
	}
	else {
		cout << "model not found!" << endl;
		return -1;
	}

	//生成随机颜色
	vector<Scalar> color;
	srand(time(0));
	for (int i = 0; i < 80; i++) {
		int b = rand() % 256;
		int g = rand() % 256;
		int r = rand() % 256;
		color.push_back(Scalar(b, g, r));
	}
	vector<Output> result;
	Mat img = imread(img_path);

	if (test.Detect(img, net, result)) {
		test.drawPred(img, result, color);

	}
	else {
		cout << "Detect Failed!"<<endl;
	}

	system("pause");
    return 0;
}

