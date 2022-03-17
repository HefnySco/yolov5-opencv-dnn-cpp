#include "queue_fps.hpp"

// template <typename T>
// void CQueueFPS<T>::push(const T& entry)
// {
//     //std::lock_guard<std::mutex> lock(mutex);
//     std::queue<T>::push(entry);
//     counter += 1;
//     if (counter == 1)
//     {
//         //Start counting from a second frame (warmup).
//         tm.reset();
//         tm.start();
//     }
// }


// template <typename T>
// T CQueueFPS<T>::get()
// {
//     std::lock_guard<std::mutex> lock(mutex);
//     T entry = this->front();
//     this->pop();
//     return entry;
// }


// template <typename T>
// float CQueueFPS<T>::getFPS()
// {
//     tm.stop();
//     double fps = counter; // / tm.getTimeSec();
//     tm.start();
//     return static_cast<float>(fps);
// }

// template <typename T>
// void CQueueFPS<T>::clear()
// {
//     std::lock_guard<std::mutex> lock(mutex);
//     while (!this->empty())
//         this->pop();
// }