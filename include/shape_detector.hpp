#ifndef SHAPE_DETECTION_HPP
#define SHAPE_DETECTION_HPP

#include <opencv2/opencv.hpp>

class ShapeDetector
{
private:
    cv::VideoCapture cap;

public:
    void detectShapes();
    ShapeDetector(cv::VideoCapture cap);
};

#endif // SHAPE_DETECTION_HPP