#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{

    cv::VideoCapture cap(0); // Open the default camera
    if (!cap.isOpened())
    { // Check if we succeeded
        std::cerr << "[ERREUR] Probleme d'ouverture de la camera." << std::endl;
        return -1;
    }

    cv::Mat frame;
    cv::Mat grayFrame, blurFrame, cannyFrame, dilFrame;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9));
    std::string formType;

    while (true)
    {
        cap >> frame;
        
        // Process the frame (convert to grayscale, blur, edge detection, dilation)
        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(grayFrame, blurFrame, cv::Size(7, 7), 5);
        cv::Canny(blurFrame, cannyFrame, 50, 150);
        cv::dilate(cannyFrame, dilFrame, kernel);
        
        // find contours (not displayed, but could be used for further processing)
        std::vector<std::vector<cv::Point>> contours;

        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(dilFrame, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        std::vector<std::vector<cv::Point>> countourPoly(contours.size());

        // bounding rectangles
        std::vector<cv::Rect> boundRect(contours.size());
        
        // get area
        cv::putText(frame, "Press \"ESC\" to quit", cv::Point(50, 50), cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(255, 0, 0), 2);
        for (int i = 0; i < contours.size(); i++)
        {
            double area = cv::contourArea(contours[i]);

            if (area > 1000) // Filter contours by area
            {
                float perimeter = cv::arcLength(contours[i], true);
                cv::approxPolyDP(contours[i], countourPoly[i], 0.02 * perimeter, true);
                
                boundRect[i] = cv::boundingRect(countourPoly[i]);
                cv::drawContours(frame, countourPoly, i, cv::Scalar(255, 0, 255), 2);
                cv::rectangle(frame, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(0, 255, 0), 2);

                // draw type of figure
                int nbrCorners = (int)countourPoly[i].size();
                if (nbrCorners == 3)
                    formType = "Triangle";
                else if (nbrCorners == 4)
                {
                    float aspectRatio = (float)boundRect[i].width / (float)boundRect[i].height;
                    if (aspectRatio > 0.95 && aspectRatio < 1.05)
                        formType = "Square";
                    else
                        formType = "Rectangle";
                }
                else if (nbrCorners > 4 && nbrCorners < 8)
                    formType = "Polygon";
                else
                    formType = "Circle";

                // write the form type
                cv::putText(frame, formType, cv::Point(boundRect[i].x, boundRect[i].y - 10), cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(255, 0, 0), 2);
            }
        }

        cv::imshow("Camera", frame); // Display the frame

        int key = cv::waitKey(1);
        if (key == 27 || key == 'q') // Exit on 'ESC' or 'q' key
        {
            break;
        }
    }
}