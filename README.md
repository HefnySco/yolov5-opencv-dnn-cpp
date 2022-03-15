
This repository is clonned and heavily inspired by https://github.com/UNeedCryDear/yolov5-opencv-dnn-cpp.

However I have made some changes and updates to make running it in ubuntu simple and straight forward.

Please note you need OpenCV>=4.5.0


### Compile

`mkdir build`
`cd build`
`cmake ../`
`make`

### How to Use

### `./build/yolo5_detector  ./image/bus.jpg  ./models/yolov5n.onnx`

[![Bus with Yolo5 Nano](https://raw.githubusercontent.com/HefnySco/yolov5-opencv-dnn-cpp/master/results/bus.png "Bus with Yolo5 Nano")](https://raw.githubusercontent.com/HefnySco/yolov5-opencv-dnn-cpp/master/results/bus.png "Bus with Yolo5 Nano")


### `./build/yolo5_detector  ./image/zidane.jpg  ./models/yolov5n.onnx`

[![Zidane Image with Yolo5 Nano](https://raw.githubusercontent.com/HefnySco/yolov5-opencv-dnn-cpp/master/results/zidane.png "Zidane Image with Yolo5 Nano")](https://raw.githubusercontent.com/HefnySco/yolov5-opencv-dnn-cpp/master/results/zidane.png "Zidane Image with Yolo5 Nano")

### Useful Links
[Ultralytics-Yolo5](https://github.com/ultralytics/yolov5 "Ultralytics-Yolo5")

[Detecting objects with YOLOv5, OpenCV, Python and C++](https://medium.com/mlearning-ai/detecting-objects-with-yolov5-opencv-python-and-c-c7cf13d1483c "Detecting objects with YOLOv5, OpenCV, Python and C++")

[Training YOLOv5 custom dataset with ease](https://medium.com/mlearning-ai/training-yolov5-custom-dataset-with-ease-e4f6272148ad "Training YOLOv5 custom dataset with ease")

[How to install OpenCV 4.5.2 with CUDA 11.2 and CUDNN 8.2 in Ubuntu 20.04](https://gist.github.com/raulqf/f42c718a658cddc16f9df07ecc627be7 "How to install OpenCV 4.5.2 with CUDA 11.2 and CUDNN 8.2 in Ubuntu 20.04")


