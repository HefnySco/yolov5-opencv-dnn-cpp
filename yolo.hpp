#ifndef YOLO_H
#define YOLO_H
#include<iostream>
#include<opencv2/opencv.hpp>


struct Output {
	int id;//result category id
	float confidence;//result confidence
	cv::Rect box;//Rectangle
};

class CYolo {
public:
	CYolo() {
	}
	CYolo(float boxThreshold, float classThreshold): m_boxThreshold(boxThreshold), m_classThreshold(classThreshold) {
	}
	~CYolo() {}

	bool readModel(const std::string &netPath, bool isCuda) ;
	bool Detect(cv::Mat &SrcImg, std::vector<Output> &output);
	void drawPred(cv::Mat &img, std::vector<Output> result, std::vector<cv::Scalar> color);
	void setClasses(std::vector<std::string> &classes) {
		className = classes;
	}
private:
	const float m_netAnchors[3][6] = { { 10.0, 13.0, 16.0, 30.0, 33.0, 23.0 },{ 30.0, 61.0, 62.0, 45.0, 59.0, 119.0 },{ 116.0, 90.0, 156.0, 198.0, 373.0, 326.0 } };
	const float m_netStride[3] = { 8, 16.0,32 };
	const int netWidth = 640;
	const int netHeight = 640;
	float m_nmsThreshold = 0.25; //0.25;
	float m_boxThreshold = 0.1; //0.15;
	float m_classThreshold = 0.25; 
	std::vector<std::string> className = { "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
		"fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
		"elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
		"skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
		"tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
		"sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch",
		"potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell phone",
		"microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear",
		"hair drier", "toothbrush" };
	
	cv::dnn::Net m_net;
	int m_counter = 0;

};

#endif