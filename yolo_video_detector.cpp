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

void CYoloVideoDetector::start (const std::string& video_path, std::unique_ptr <CYolo> yolo, const bool display, const bool save_file)
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
            #ifdef DEBUG
		        std::cout << "push frame" << std::endl;
                std::cout << "GOT A FRAME" << std::endl;                                   
            #endif
            
            #ifdef DEBUG
		        std::cout << "PROCESS A FRAME" << std::endl;
            #endif
                            
            m_yolo.get()->Detect(frame, result);
            m_yolo.get()->drawPred(frame, result, color,display,save_file);
            result.clear();
            
                        
        }
        else
        {
            #ifdef DEBUG
		        std::cout << "empty frame" << std::endl;
            #endif
        }
           
    };


    
    
}

void CYoloVideoDetector::startWithThreads (const std::string& video_path, std::unique_ptr <CYolo> yolo, const bool display, const bool save_file)
{
    m_yolo = std::move(yolo);
    m_process = true;

    // Frames capturing thread
    m_framesThread = std::thread([&](){
        
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
        while (m_process)
        {
            if (m_framesQueue.empty()) // only add one frame
            {
                video_capture_cap >> frame;
                if (!frame.empty())
                {
                    #ifdef DEBUG
		                std::cout << "push frame" << std::endl;
	                #endif
                    m_framesQueue.push(frame.clone());
                }
                else
                {
                    #ifdef DEBUG
		                std::cout << "empty frame" << std::endl;
	                #endif
                }
            }
            else
            {
                #ifdef DEBUG
		            std::cout << "BUSY WRITE" << std::endl;
                #endif
            }
        };
    });


    // Frames processing thread
    m_processingThread = std::thread([&](){
        //Generate random colors
        std::vector<cv::Scalar> color;
        srand(time(0));
        for (int i = 0; i < 80; i++) {
            int b = rand() % 256;
            int g = rand() % 256;
            int r = rand() % 256;
            color.push_back(cv::Scalar(b, g, r));
        }

        std::vector<Output> result;
        while (m_process)
        {
            // Get a next frame
            cv::Mat frame;
            {
                if (!m_framesQueue.empty())
                {
                    frame  = m_framesQueue.get();
                    #if defined DEBUG
		                std::cout << "GOT A FRAME" << std::endl;
                    #endif

                    // Process the frame
                    if (!frame.empty())
                    {
                        #if defined DEBUG
		                    std::cout << "PROCESS A FRAME" << std::endl;
                        #endif

                        bool res = m_yolo.get()->Detect(frame, result);
                        if (res)
                        {
                            m_yolo.get()->drawPred(frame, result, color,display,save_file);
                            result.clear();
                        }
                    }
                }
            }
        }
    });
}