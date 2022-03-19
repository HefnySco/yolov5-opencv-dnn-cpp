#include<stdio.h>

#include"yolo.hpp"
using namespace std;
using namespace cv;
using namespace dnn;

bool CYolo::readModel(const string &netPath, bool isCuda = false) {
	try {
		m_net = readNet(netPath);
	}
	catch (const std::exception&) {
		return false;
	}

	#ifndef YOLO_USE_CUDA
	isCuda = false;
	#endif
	
	//cuda
	if (isCuda) {
		m_net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
		m_net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);

	}
	//cpu
	else {
		
		m_net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
		m_net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
	}
	

	return true;
}

bool CYolo::Detect(Mat &SrcImg, vector<Output> &output) {
	Mat blob;
	int col = SrcImg.cols;
	int row = SrcImg.rows;
	int maxLen = MAX(col, row);
	Mat netInputImg = SrcImg.clone();
	if (maxLen > 1.2*col || maxLen > 1.2*row) {
		Mat resizeImg = Mat::zeros(maxLen, maxLen, CV_8UC3);
		SrcImg.copyTo(resizeImg(Rect(0, 0, col, row)));
		netInputImg = resizeImg;
	}

	
	blobFromImage(netInputImg, blob, 1 / 455.0, cv::Size(netWidth, netHeight), cv::Scalar(0,0,0), true, false); // 1/455.0 gets better results
	//blobFromImage(netInputImg, blob, 1 / 255.0, cv::Size(netWidth, netHeight), cv::Scalar(0,0,0), false, false); 

	//blobFromImage(netInputImg, blob, 1 / 255.0, cv::Size(netWidth, netHeight), cv::Scalar(104, 117,123), true, false);
	//If there are no problems with other settings but the results are very different, you can try the following two sentences
	//blobFromImage(netInputImg, blob, 1 / 255.0, cv::Size(netWidth, netHeight), cv::Scalar(0, 0,0), true, false);
	//blobFromImage(netInputImg, blob, 1 / 255.0, cv::Size(netWidth, netHeight), cv::Scalar(114, 114,114), true, false);
	m_net.setInput(blob);
	std::vector<cv::Mat> netOutputImg;
	m_net.forward(netOutputImg, m_net.getUnconnectedOutLayersNames());
	std::vector<int> classIds;//As a result, each id corresponds to a confidence array
	std::vector<float> confidences;//As a result, each id corresponds to a confidence array
	std::vector<float> box_scores;//As a result, each id corresponds to a confidence array
	std::vector<cv::Rect> boxes;//Each id rectangle
	
	float ratio_h = (float)netInputImg.rows / netHeight;
	float ratio_w = (float)netInputImg.cols / netWidth;
	int net_width = className.size() + 5;  //输出的网络宽度是类别数+5
	Mat out = Mat(netOutputImg[0].size[1], netOutputImg[0].size[2], netOutputImg[0].size[3], netOutputImg[0].ptr<float>());
    float* pdata = (float*)netOutputImg[0].data;
	for (int r = 0; r < out.size[0]; ++r)
    {	
		int i = net_width * r;
		float box_score = pdata[i+4]; //Get the probability that the box of each row contains an object
		cv::Mat scores(1, className.size(), CV_32FC1, &pdata[i+5]);
		Point classIdPoint;
		double max_class_socre;
		minMaxLoc(scores, 0, &max_class_socre, 0, &classIdPoint);
		//max_class_socre = (float) pdata[i+5+5]; // you can choose to scan for certain objects only...not 100% correct as you may ignore other objects with higher probabilities.
		max_class_socre = (float)max_class_socre; 
		if (box_score > m_boxThreshold) 
		{
			if (max_class_socre > m_classThreshold) 
			{
				//rect [x,y,w,h]
				float x = pdata[i+0];
				float y = pdata[i+1];
				float w = pdata[i+2];
				float h = pdata[i+3];
							
				int left = (x - 0.5*w)*ratio_w;
				int top = (y - 0.5*h)*ratio_h;
				classIds.push_back(classIdPoint.x);
				box_scores.push_back(box_score);
				confidences.push_back(max_class_socre*box_score);
				boxes.push_back(Rect(left, top, int(w*ratio_w), int(h*ratio_h)));
			}
		}
	}
	
	#if defined DEBUG
		std::cout << "boxes:" << boxes.size() << std::endl;
	#endif
	//Perform non-maximal suppression to remove redundant overlapping boxes with lower confidence（NMS）
	vector<int> nms_result;
	NMSBoxes(boxes, confidences, m_classThreshold * m_boxThreshold, m_nmsThreshold, nms_result);
	
	vector<int> ::iterator res;
	for (res = nms_result.begin();  res!= nms_result.end(); ++res) 
	{
		const int idx = *res;
		Output result;
		result.id = classIds[idx];
		result.confidence = confidences[idx];
		result.box = boxes[idx];
		output.push_back(result);
	}
	#if defined DEBUG
		std::cout << "Final boxes:" << output.size() << std::endl;
	#endif
	
	if (output.size())
		return true;
	else
		return false;
}

void CYolo::drawPred(Mat &img, vector<Output> result, vector<Scalar> color, const bool display, const bool save) {
	vector<Output> ::iterator res;
	
	for (res = result.begin();  res!= result.end(); ++res, ++m_counter)
	{
		const int left = res->box.x;
		int top = res->box.y-5;
		rectangle(img, res->box, color[res->id], 1, 8);

		string label = className[res->id] +":" + to_string(res->confidence);
		int baseLine;
		Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.1, 0.1, &baseLine);
		top = max(top, labelSize.height);
		putText(img, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.3, color[res->id], 1);
		
	}
	
	std::string filename = "out" + std::to_string(m_counter) + ".bmp";
	if (display) 
	{
		imshow("Result", img);
		cv::waitKey(1);
	}

	if (save) imwrite(filename, img);
	
	//destroyAllWindows();

}
