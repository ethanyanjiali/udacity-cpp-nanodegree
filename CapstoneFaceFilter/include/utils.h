#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <opencv2/opencv.hpp>

namespace utils
{
    double FindAngleBetweenTwoLines(cv::Point2f src_point1, cv::Point2f src_point2, cv::Point2f dst_point1, cv::Point2f dst_point2);
    void CreateAlphaImage(const cv::Mat &mat, cv::Mat &dst);
    int GetInputMode();
    int GetAssetId();
    std::string GetPhotoFilePath(std::string default_path);
} // namespace utils

#endif // UTILS_H