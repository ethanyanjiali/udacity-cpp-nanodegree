#ifndef FACE_DETECTOR_H_
#define FACE_DETECTOR_H_
#include <vector>
#include <opencv2/objdetect.hpp>

class HaarCascadeFaceDetector
{
public:
    HaarCascadeFaceDetector();
    std::vector<cv::Rect> Detect(cv::Mat &);

private:
    cv::CascadeClassifier cascade;
};

#endif // FACE_DETECTOR_H_