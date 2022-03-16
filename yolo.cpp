#include<stdio.h>
#include"yolo.h";
using namespace std;
using namespace cv;
using namespace dnn;

bool Yolo::readModel(Net &net, string &netPath,bool isCuda = false) {
	try {
		net = readNet(netPath);
	}
	catch (const std::exception&) {
		return false;
	}
	//cuda
	if (isCuda) {
		net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
		net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);

	}
	//cpu
	else {
		
		net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
		net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
	}
	return true;
}

bool Yolo::Detect(Mat &SrcImg, Net &net, vector<Output> &output) {
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
	net.setInput(blob);
	std::vector<cv::Mat> netOutputImg;
	net.forward(netOutputImg, net.getUnconnectedOutLayersNames());
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
    {				int i = net_width * r;
					float box_score = pdata[i+4]; //Get the probability that the box of each row contains an object
						cv::Mat scores(1, className.size(), CV_32FC1, &pdata[i+5]);
						Point classIdPoint;
						double max_class_socre;
						minMaxLoc(scores, 0, &max_class_socre, 0, &classIdPoint);
						//max_class_socre = (float) pdata[i+5+5]; // you can choose to scan for certain objects only...not 100% correct as you may ignore other objects with higher probabilities.
						max_class_socre = (float)max_class_socre; 
						std::cout << className[classIdPoint.x] << " max_class_socre: " << max_class_socre << "	 box_score: " << box_score << std::endl;
						
					if (box_score > boxThreshold) {
						if (max_class_socre > classThreshold) {
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
							std::cout << "class:" << className[classIdPoint.x] << " 	max_class_socre:" << max_class_socre << std::endl;
						}
					}
	}

	//Perform non-maximal suppression to remove redundant overlapping boxes with lower confidence（NMS）
	vector<int> nms_result;
	NMSBoxes(boxes, confidences, classThreshold * boxThreshold, nmsThreshold, nms_result);
	for (int i = 0; i < nms_result.size(); i++) {
		int idx = nms_result[i];
		Output result;
		result.id = classIds[idx];
		result.confidence = confidences[idx];
		result.box = boxes[idx];
		output.push_back(result);
	}

	if (output.size())
		return true;
	else
		return false;
}

void Yolo::drawPred(Mat &img, vector<Output> result, vector<Scalar> color) {
	for (int i = 0; i < result.size(); i++) {
		int left, top;
		left = result[i].box.x;
		top = result[i].box.y;
		int color_num = i;
		rectangle(img, result[i].box, color[result[i].id], 2, 8);

		string label = className[result[i].id] +":" + to_string(result[i].confidence);
							 
		int baseLine;
		Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
		top = max(top, labelSize.height);
		//rectangle(frame, Point(left, top - int(1.5 * labelSize.height)), Point(left + int(1.5 * labelSize.width), top + baseLine), Scalar(0, 255, 0), FILLED);
		putText(img, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 1, color[result[i].id], 2);
	}
	imshow("Result", img);
	imwrite("out.bmp", img);
	waitKey();
	//destroyAllWindows();

}
