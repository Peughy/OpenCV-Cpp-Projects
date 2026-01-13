#include <iostream>
#include <opencv2/opencv.hpp>

class VirtualPrinter
{

public:
    VirtualPrinter(cv::VideoCapture cap);

    // draw canva
    void drawOnCanvas();

private:
    cv::VideoCapture cap;
    std::vector<std::vector<int>> points;

    // find contours
    cv::Point getContours(cv::Mat &frame, cv::Mat &maskFrame);

    // set points
    void setPoints(int &idxColor, cv::Point &point);

    // find colors
    cv::Mat getColors(cv::Mat &frame, std::vector<std::vector<int>> &hsvColorsValues);
};