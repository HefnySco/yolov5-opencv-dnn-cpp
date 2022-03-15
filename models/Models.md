### Yolo5 Models

Models can be downloaded from [https://github.com/ultralytics/yolov5/releases](https://github.com/ultralytics/yolov5/releases "https://github.com/ultralytics/yolov5/releases")

You need to convert it to ONNEX format and this is by running command:
`python export.py --opset 12  --weights ./yolo5n.pt --include onnx`
You need to clone https://github.com/ultralytics/yolov5 first and just execute the above line and you will get your **onnx** file.


