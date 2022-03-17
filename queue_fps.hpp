/**
 * @file queue_fps.hpp
 * @author Mohammad S. Hefny (mohammad.hefny@gmail.com)
 * @brief 
 * @see https://docs.opencv.org/4.x/d4/db9/samples_2dnn_2object_detection_8cpp-example.html
 * @version 0.1
 * @date 2022-03-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef QUEUE_FPS_H
#define QUEUE_FPS_H

#include<opencv2/opencv.hpp>


#include <mutex>
#include <thread>
#include <queue>

template <typename T>
class CQueueFPS: public std::queue<T> 
{
    public:
        CQueueFPS(): counter(0) {};
    
    
    public:
        void push(const T& entry){
            std::lock_guard<std::mutex> lock(mutex);
            std::queue<T>::push(entry);
            counter += 1;
            if (counter == 1)
            {
                //Start counting from a second frame (warmup).
                tm.reset();
                tm.start();
            }
        };
        T get(){
            std::lock_guard<std::mutex> lock(mutex);
            T entry = this->front();
            this->pop();
            return entry;
        };
        float getFPS(){
            tm.stop();
            double fps = tm.getTimeSec();
            tm.start();
            return static_cast<float>(fps);
        };
        void clear(){
            std::lock_guard<std::mutex> lock(mutex);
            while (!this->empty())
                this->pop();
        };

        
    public:
        unsigned int counter;

    private:
        cv::TickMeter tm;
        std::mutex mutex;
};

#endif