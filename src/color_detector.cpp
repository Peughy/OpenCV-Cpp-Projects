#include <opencv2/opencv.hpp>
#include "color_detector.hpp"

ColorDetector::ColorDetector(cv::VideoCapture capture) : cap(capture) {}

void ColorDetector::detectColors()
{
    cv::Mat frame;
    cv::Mat hsvFrame, mask;

    int hmin = 0, hmax = 179;
    int smin = 0, smax = 255;
    int vmin = 0, vmax = 255;

    cv::namedWindow("Trackbars", (640, 200));
    cv::createTrackbar("Hue Min", "Trackbars", &hmin, 179);
    cv::createTrackbar("Hue Max", "Trackbars", &hmax, 179);
    cv::createTrackbar("Sat Min", "Trackbars", &smin, 255);
    cv::createTrackbar("Sat Max", "Trackbars", &smax, 255);
    cv::createTrackbar("Val Min", "Trackbars", &vmin, 255);
    cv::createTrackbar("Val Max", "Trackbars", &vmax, 255);

    while (true)
    {
        cap >> frame;

        cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);
        cv::inRange(hsvFrame, cv::Scalar(hmin, smin, vmin), cv::Scalar(hmax, smax, vmax), mask);
        // std::cout << hmin << "," << smin << "," << vmin << "," << hmax << "," << smax << "," << vmax << std::endl;

        cv::imshow("HSV frame", hsvFrame);
        cv::imshow("Original Frame", frame);
        cv::imshow("Mask", mask);

        int key = cv::waitKey(1);
        if (key == 27 || key == 'q')
            break;
    }
}