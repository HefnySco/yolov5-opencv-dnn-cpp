#include <stdio.h>
#include <iostream>
#include <fstream>
#include<opencv2//opencv.hpp>
#include<math.h>

#include "yolo.hpp"

std::string keys =
    "{help h usage ?	| | This app reads an image and runs yolo5 on it. }"
    "{model m|<none>| Path to model.}"
    "{img i|<none>| Path to input image.}"
	"{classes c|<none>|classes file path}"
	"{boxThreshold bt|<none>|confidence is a point is a center of a valid object.}"
	"{classThreshold ct|<none>|confidence is the point is center of this particular object.}"
	// "{ @alias           |yolov5s.onnx | An alias name of model to extract preprocessing parameters from models.yml file. }"
    // "{@image        	|./image/zidane.jpg | image to recognize. }"
    ;



using namespace std;
using namespace cv;
using namespace dnn;

int main(int argc, char** argv)
{
	CommandLineParser parser(argc, argv, keys);
	const std::string img_path = parser.get<String>("img");
	const std::string model_path = parser.get<String>("model");
    const std::string classes_path = parser.get<String>("classes");
	float boxThreshold = 0.1; //0.15;
	float classThreshold = 0.25; 
	
	if (parser.has("boxThreshold")){
		boxThreshold = parser.get<float>("boxThreshold");
	}
	if (parser.has("classThreshold")){
		classThreshold = parser.get<float>("classThreshold");
	}
    
	parser = CommandLineParser(argc, argv, keys);
    parser.about("Use this script to run classification YOLO5 deep learning networks using OpenCV.");
    if (argc == 1 || parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }
	
	cout << "-----------------------------" << endl;
 	cout << "OpenCV version : " << CV_VERSION << endl;
  	cout << "Major version : " << CV_MAJOR_VERSION << endl;
  	cout << "Minor version : " << CV_MINOR_VERSION << endl;
  	cout << "Subminor version : " << CV_SUBMINOR_VERSION << endl;
	cout << "-----------------------------" << endl;
 	
	// string img_path = argc>1?argv[1]:"./image/zidane.jpg";
    // string model_path = argc>2?argv[2]:"yolov5s.onnx";
    
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
	
	cout << "load model:" << model_path << endl;
	cout << "image path:" << img_path << endl;
	cout << "classes path:" << classes_path << endl;
	cout << "boxThreshold:" << boxThreshold << endl;
	cout << "classThreshold:" << classThreshold << endl;

	
	CYolo yolo_detector (boxThreshold, classThreshold);
	
	
	if (parser.has("classes")){
		std::vector<std::string> classes;
		string myText;
		ifstream MyReadFile(classes_path);
		while (getline (MyReadFile, myText)) {
  			// Output the text from the file
  			//classes << myText;
			classes.push_back(myText);
		}


		yolo_detector.setClasses(classes);
		MyReadFile.close();

	}

	
	

	if (yolo_detector.readModel(model_path, false)) {
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

	if (yolo_detector.Detect(img, result)) {
		yolo_detector.drawPred(img, result, color);

	}
	else {
		cout << "Detect Failed!"<<endl;
	}

	return 0;
}

