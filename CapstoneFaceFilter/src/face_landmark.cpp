#include <vector>
#include <iostream>
#include "face_landmark.h"

KazemiFaceLandmark::KazemiFaceLandmark()
{
    facemark = cv::face::FacemarkKazemi::create();
    facemark->loadModel("../assets/models/face_landmark_model.dat");
}

std::vector<std::vector<cv::Point2f>> KazemiFaceLandmark::DetectLandmarks(cv::Mat image, std::vector<cv::Rect> faces)
{
    std::vector<std::vector<cv::Point2f>> landmarks;
    try
    {
        facemark->fit(image, faces, landmarks);
    }
    catch (const std::exception &e)
    {
        return landmarks;
    }

    return landmarks;
}