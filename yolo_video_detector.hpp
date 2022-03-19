/**
 * @file yolo_video_detector.hpp
 * @author Mohammad S. Hefny (mohammad.hefny@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef YOLO_VIDEO_DETECTOR_H
#define YOLO_VIDEO_DETECTOR_H
#include <memory>
#include<iostream>

#include "yolo.hpp"
#include "queue_fps.hpp"

class CYoloVideoDetector
{

    public:
        CYoloVideoDetector ()
        {

        }

        ~CYoloVideoDetector()
        {
            uninit();
        }

    public:
        void start(const std::string& video_path, std::unique_ptr <CYolo> yolo, const bool display, const bool save_file);
        void startWithThreads(const std::string& video_path, std::unique_ptr <CYolo> yolo, const bool display, const bool save_file);
        void stop () {uninit();};
        void uninit ();

    private:
        std::unique_ptr <CYolo> m_yolo;
        bool m_process = false;
        CQueueFPS<cv::Mat> m_framesQueue; 
        std::thread m_framesThread;  
        std::thread m_processingThread;
        
};

#endif




