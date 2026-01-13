#include "virtual_printer.hpp"
#include <opencv2/opencv.hpp>

VirtualPrinter::VirtualPrinter(cv::VideoCapture cap) : cap(cap)
{
    points = {};
}

// gets points
void VirtualPrinter::setPoints(int &idxColor, cv::Point &point)
{
    if (point.x != 0 && point.y != 0)
    {
        points.push_back({point.x, point.y, idxColor});
    }
}

// get contours
cv::Point VirtualPrinter::getContours(cv::Mat &frame, cv::Mat &maskFrame)
{
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(maskFrame, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<std::vector<cv::Point>> conPoly(contours.size());
    std::vector<cv::Rect> boundRect(contours.size());

    cv::Point drawingPoint(0, 0);

    for (int i = 0; i < contours.size(); i++)
    {
        double area = cv::contourArea(contours[i]); 

        if (area > 1000)
        {
            double peri = cv::arcLength(contours[i], true);
            cv::approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

            // define virtual case
            boundRect[i] = cv::boundingRect(conPoly[i]);

            // set point
            drawingPoint.x = boundRect[i].x + boundRect[i].width / 2;
            drawingPoint.y = boundRect[i].y;

            cv::drawContours(frame, conPoly, i, cv::Scalar(0, 255, 0), 2);
            cv::rectangle(frame, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(0, 255, 0), 5);
        }
    }

    return drawingPoint;
}

// find colors
cv::Mat VirtualPrinter::getColors(cv::Mat &frame, std::vector<std::vector<int>> &hsvColorsValues)
{

    cv::Mat hsvFrame, maskFrame;
    cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);

    for (int i = 0; i < hsvColorsValues.size(); i++)
    {

        cv::Scalar lower = cv::Scalar(hsvColorsValues[i][0], hsvColorsValues[i][1], hsvColorsValues[i][2]);
        cv::Scalar upper = cv::Scalar(hsvColorsValues[i][3], hsvColorsValues[i][4], hsvColorsValues[i][5]);

        // detect color
        cv::inRange(hsvFrame, lower, upper, maskFrame);
        cv::imshow("Mask", maskFrame);

        // find contours and get point
        cv::Point point = getContours(frame, maskFrame);

        // add points
        setPoints(i, point);
    }

    return maskFrame;
}

void VirtualPrinter::drawOnCanvas()
{

    std::vector<std::vector<int>> colors = {
        {255, 0, 0}};

    std::vector<std::vector<int>> hsvColorsValues = {
        {120, 0, 127, 140, 255, 255}};

    cv::Mat capFrame, frame;

    while (true)
    {
        cap.read(capFrame);
        cv::resize(capFrame, frame, cv::Size(760, 680));

        cv::putText(frame, "Press 'R' for reste", cv::Point(50, 50), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0, 0, 255));
        cv::putText(frame, "Press 'ESC' for quit", cv::Point(50, 100), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0, 0, 255));

        // detect colors
        getColors(frame, hsvColorsValues);

        for (int i = 0; i < points.size(); i++)
        {

            int idxColor = points[i][2];
            cv::circle(frame, cv::Point(points[i][0], points[i][1]), 5, cv::Scalar(colors[idxColor][0], colors[idxColor][1], colors[idxColor][2]), cv::FILLED);
        }

        // Show frame
        cv::imshow("Frame", frame);
        int key = cv::waitKey(1);

        if (key == 27 || key == 'Q' || key == 'q')
        {
            break;
        }

        if (key == 'r' || key == 'R')
        {
            points.clear();
        }
    }
}
