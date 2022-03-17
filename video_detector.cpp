#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <math.h>
#include <opencv2//opencv.hpp>
#include "yolo.hpp"
#include "yolo_video_detector.hpp"
#include "queue_fps.hpp"
std::string keys =
    "{help h usage ?	| | This app reads video and runs yolo5 on it. }"
    "{model m|<none>| Path to model.}"
    "{video v|<none>| Path to camera or video.}"
	"{classes c|<none>|classes file path}"
	"{boxThreshold bt|<none>|confidence is a point is a center of a valid object.}"
	"{classThreshold ct|<none>|confidence is the point is center of this particular object.}"
	// "{ @alias           |yolov5s.onnx | An alias name of model to extract preprocessing parameters from models.yml file. }"
    // "{@image        	|./image/zidane.jpg | image to recognize. }"
    ;

bool m_exit = false;
CYoloVideoDetector yolo_detector;
std::unique_ptr<CYolo> yolo;

void uninit ()
{
	yolo_detector.stop();
	m_exit = true;
}

// ------------------------------------------------------------------------------
//   Quit Signal Handler
// ------------------------------------------------------------------------------
// this function is called when you press Ctrl-C
void quit_handler( int sig )
{
	std::cout <<  "TERMINATING AT USER REQUEST" <<  std::endl;
	
	try 
    {
        uninit();
	}
	catch (int error){}

}



void init (int argc, char** argv)
{
	signal(SIGINT,quit_handler);
}

int main(int argc, char** argv)
{
	init(argc, argv);
	cv::CommandLineParser parser(argc, argv, keys);
	const std::string video_path = parser.get<cv::String>("video");
	const std::string model_path = parser.get<cv::String>("model");
    const std::string classes_path = parser.get<cv::String>("classes");
	// const std::string video_path = "/dev/video0";
	// const std::string model_path = "../yolov5s.onnx";
    // const std::string classes_path = "../models/coco.txt";
	float boxThreshold = 0.1; //0.15;
	float classThreshold = 0.25; 
	
	if (parser.has("boxThreshold")){
		boxThreshold = parser.get<float>("boxThreshold");
	}
	if (parser.has("classThreshold")){
		classThreshold = parser.get<float>("classThreshold");
	}
    
	parser = cv::CommandLineParser(argc, argv, keys);
    parser.about("Use this script to run classification YOLO5 deep learning networks using OpenCV.");
    if (argc == 1 || parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }
	
	std::cout << "-----------------------------" << std::endl;
 	std::cout << "OpenCV version : " << CV_VERSION << std::endl;
  	std::cout << "Major version : " << CV_MAJOR_VERSION << std::endl;
  	std::cout << "Minor version : " << CV_MINOR_VERSION << std::endl;
  	std::cout << "Subminor version : " << CV_SUBMINOR_VERSION << std::endl;
	std::cout << "-----------------------------" << std::endl;
 	
	// string img_path = argc>1?argv[1]:"./image/zidane.jpg";
    // string model_path = argc>2?argv[2]:"yolov5s.onnx";
    
	#ifdef YOLO_USE_CUDA
	int num_devices = cv::cuda::getCudaEnabledDeviceCount();
	if (num_devices <= 0) {
		std::cerr << "There is no cuda." << std::endl;
		return -1;
	}
	else {
		std::cout << "There is " << num_devices << " cuda units."<< std::endl;
		std::cout << "=============================" << std::endl;
 	}
	#endif
	
	std::cout << "load model:" << model_path << std::endl;
	std::cout << "video path:" << video_path << std::endl;
	std::cout << "classes path:" << classes_path << std::endl;
	std::cout << "boxThreshold:" << boxThreshold << std::endl;
	std::cout << "classThreshold:" << classThreshold << std::endl;

	
	yolo=  std::unique_ptr<CYolo>(new CYolo (boxThreshold, classThreshold));
	
	
	if (parser.has("classes")){
		std::vector<std::string> classes;
		std::string myText;
		std::ifstream MyReadFile(classes_path);
		while (std::getline (MyReadFile, myText)) {
  			// Output the text from the file
  			//classes << myText;
			classes.push_back(myText);
		}


		yolo.get()->setClasses(classes);
		MyReadFile.close();

	}

	
	

	if (yolo.get()->readModel(model_path, false)) {
		std::cout << "read net ok!" << std::endl;
	}
	else {
		std::cerr << "model not found!" << std::endl;
		return -1;
	}

	//Generate random colors
	std::vector<cv::Scalar> color;
	srand(time(0));
	for (int i = 0; i < 80; i++) {
		int b = rand() % 256;
		int g = rand() % 256;
		int r = rand() % 256;
		color.push_back(cv::Scalar(b, g, r));
	}
	
	
	
	yolo_detector.start(video_path, std::move(yolo));
    
	while (!m_exit) sleep (1000);
    return 0;
}

