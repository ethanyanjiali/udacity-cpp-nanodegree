#include <iostream>
#include "utils.h"

double utils::FindAngleBetweenTwoLines(cv::Point2f src_point1, cv::Point2f src_point2, cv::Point2f dst_point1, cv::Point2f dst_point2)
{
    double src_angle = 0;
    double dst_angle = 0;

    auto src_delta = src_point1 - src_point2;
    if (src_delta.x == 0)
    {
        src_angle = 90;
    }
    else
    {
        double src_radius = std::atan(src_delta.y / src_delta.x);
        src_angle = src_radius * (180.0 / M_PI);
    }

    auto dst_delta = dst_point1 - dst_point2;
    if (dst_delta.x == 0)
    {
        dst_angle = 90;
    }
    else
    {
        double dst_radius = std::atan(dst_delta.y / dst_delta.x);
        dst_angle = dst_radius * (180.0 / M_PI);
    }

    return dst_angle - src_angle;
}

void utils::CreateAlphaImage(const cv::Mat &mat, cv::Mat &dst)
{
    std::vector<cv::Mat> matChannels;
    cv::split(mat, matChannels);

    // create alpha channel
    cv::Mat alpha = matChannels.at(0) + matChannels.at(1) + matChannels.at(2);
    matChannels.push_back(alpha);

    cv::merge(matChannels, dst);
}

int utils::GetAssetId()
{
    int asset_id = 0;
    std::cout << "Choose the sunglasses style, enter a number from 0 to 2." << std::endl;
    std::cin >> asset_id;
    return asset_id;
}

int utils::GetInputMode()
{
    int input_mode = 0;
    std::cout << "Do you wish to use webcam or use a photo? Enter 1 for webcam and 2 for photo." << std::endl;
    std::cin >> input_mode;
    return input_mode;
}

std::string utils::GetPhotoFilePath(std::string default_path)
{
    std::cout << "Please specify the image path, or enter 1 to use the default image." << std::endl;
    std::string path;
    std::cin >> path;
    if (path == "1")
    {
        return default_path;
    }
    return path;
}