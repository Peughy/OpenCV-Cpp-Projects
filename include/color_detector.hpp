#ifndef COLOR_DETECTION_HPP
#define COLOR_DETECTION_HPP

#include <opencv2/opencv.hpp>

class ColorDetector
{

private:
    cv::VideoCapture cap;

public:
    void detectColors();
    ColorDetector(cv::VideoCapture cap);
};

#endif // COLOR_DETECTION_HPP