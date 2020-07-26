#include <vector>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include "face_detector.h"

HaarCascadeFaceDetector::HaarCascadeFaceDetector()
{
    if (!cascade.load("../assets/models/haarcascade_frontalface_alt.xml"))
    {
        throw std::invalid_argument("invalid haar cascade model file.");
    }
}

std::vector<cv::Rect> HaarCascadeFaceDetector::Detect(cv::Mat &image)
{
    std::vector<cv::Rect> faces;

    cv::Mat grayscale;
    cv::cvtColor(image, grayscale, cv::COLOR_BGR2GRAY);
    cascade.detectMultiScale(grayscale, faces, 1.1, 3, 0, cv::Size(30, 30));

    return faces;
}