#include <iostream>
#include <opencv2/opencv.hpp>

// #include "shape_detector.hpp"
// #include "color_detector.hpp"
#include "virtual_printer.hpp"

int main()
{
    cv::VideoCapture cap(0); // Open the default camera
    if (!cap.isOpened())
    {
        std::cerr << "[ERREUR] Probleme d'ouverture de la camera." << std::endl;
        return -1;
    }

    // ShapeDetector shapeDetector(cap);
    // shapeDetector.detectShapes();

    // ColorDetector colorDetector(cap);
    // colorDetector.detectColors(); 

    VirtualPrinter virtualPrinter(cap);
    virtualPrinter.drawOnCanvas();

    return 0;
}