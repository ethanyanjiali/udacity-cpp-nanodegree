#ifndef FACE_LANDMARK_H_
#define FACE_LANDMARK_H_
#include <vector>
#include <opencv2/face.hpp>
#include "face_detector.h"

class KazemiFaceLandmark
{
public:
    KazemiFaceLandmark();
    std::vector<std::vector<cv::Point2f>> DetectLandmarks(cv::Mat image, std::vector<cv::Rect> faces);

private:
    HaarCascadeFaceDetector face_detector;
    cv::Ptr<cv::face::Facemark> facemark;
};

#endif // FACE_LANDMARK_H_