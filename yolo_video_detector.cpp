#include <thread>
#include <unistd.h>
#include "yolo_video_detector.hpp"

#if defined(CV_CXX11) && defined(HAVE_THREADS)
#define USE_THREADS 1
#endif


void CYoloVideoDetector::uninit ()
{
    m_process = false;
    m_framesThread.join();
    m_processingThread.join();
}

void CYoloVideoDetector::start (const std::string& video_path, std::unique_ptr <CYolo> yolo)
{
    m_yolo = std::move(yolo);
    m_process = true;

    //Generate random colors
	std::vector<cv::Scalar> color;
	srand(time(0));
	for (int i = 0; i < 80; i++) {
		int b = rand() % 256;
		int g = rand() % 256;
		int r = rand() % 256;
		color.push_back(cv::Scalar(b, g, r));
	}

    // Frames capturing thread
    
    cv::VideoCapture video_capture_cap;
        video_capture_cap.open(video_path);
         if (video_capture_cap.isOpened()) {
            video_capture_cap.set(
                cv::CAP_PROP_FRAME_WIDTH,
                640);
            video_capture_cap.set(
                cv::CAP_PROP_FRAME_HEIGHT,
                480);
            video_capture_cap.set(
                cv::CAP_PROP_FPS,
                10); 
         }
    cv::Mat frame;
    std::vector<Output> result;
    while (m_process)
    {
        video_capture_cap >> frame;
        if (!frame.empty())
        {
            std::cout << "push frame" << std::endl;
            std::cout << "GOT A FRAME" << std::endl;                                   
            // Process the frame
            if (!frame.empty())
            {
                std::cout << "PROCESS A FRAME" << std::endl;
                            
                m_yolo.get()->Detect(frame, result);
                m_yolo.get()->drawPred(frame, result, color);
                result.clear();
            }
                        
        }
        else
        {
            std::cout << "empty frame" << std::endl;
        }
           
    };


    
    
}

// void CYoloVideoDetector::start (const std::string& video_path, std::unique_ptr <CYolo> yolo)
// {
//     m_yolo = std::move(yolo);
//     m_process = true;

//     //Generate random colors
// 	std::vector<cv::Scalar> color;
// 	srand(time(0));
// 	for (int i = 0; i < 80; i++) {
// 		int b = rand() % 256;
// 		int g = rand() % 256;
// 		int r = rand() % 256;
// 		color.push_back(cv::Scalar(b, g, r));
// 	}

//     // Frames capturing thread
    
//     m_framesThread = std::thread([&](){
//         cv::VideoCapture video_capture_cap;
//         video_capture_cap.open(video_path);
//          if (video_capture_cap.isOpened()) {
//             video_capture_cap.set(
//                 cv::CAP_PROP_FRAME_WIDTH,
//                 640);
//             video_capture_cap.set(
//                 cv::CAP_PROP_FRAME_HEIGHT,
//                 480);
//             video_capture_cap.set(
//                 cv::CAP_PROP_FPS,
//                 10); 
//          }
//         cv::Mat frame;
//         while (m_process)
//         {
//             if (m_framesQueue.empty()) // only add one frame
//             {
//                 video_capture_cap >> frame;
//                 if (!frame.empty())
//                 {
//                     std::cout << "push frame" << std::endl;
//                     m_framesQueue.push(frame.clone());
//                 }
//                 else
//                 {
//                     std::cout << "empty frame" << std::endl;
                    
//                     //break;
//                 }
//             }
//             else
//             {
//                 std::cout << "BUSY WRITE" << std::endl;
//                 sleep(1);
//             }
//         };
//     });


//     // Frames processing thread
//     std::vector<Output> result;
//     m_processingThread = std::thread([&](){
//         while (m_process)
//         {
//             // Get a next frame
//             cv::Mat frame;
//             {
//                 if (!m_framesQueue.empty())
//                 {
//                    frame  = m_framesQueue.get();
//                     m_framesQueue.clear();
//                     std::cout << "GOT A FRAME" << std::endl;
//                     //m_framesQueue.clear();  // Skip the rest of frames
                    
//                     // Process the frame
//                     if (!frame.empty())
//                     {
//                         std::cout << "PROCESS A FRAME" << std::endl;
                            
//                         //AsyncArray async_out = futureOutputs.front();
//                         //cv::Mat out;
//                         m_yolo.get()->Detect(frame, result);
//                         m_yolo.get()->drawPred(frame, result, color);
//                         result.clear();
                        
//                     }
//                 }
                
//             }
            
//         }
//     });
    
// }