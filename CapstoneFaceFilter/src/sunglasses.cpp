#include "utils.h"
#include "sunglasses.h"

Sunglasses::Sunglasses(Sunglasses &source)
{
    _asset_image = source._asset_image.clone();
};

Sunglasses Sunglasses::operator=(Sunglasses &source)
{
    if (this == &source)
        return *this;
    _asset_image = source._asset_image.clone();
    return *this;
};

Sunglasses::Sunglasses(Sunglasses &&source)
{
    // cv:Mat's copy constructor move by reference under the hood
    _asset_image = source._asset_image;
    source._asset_image.release();
};

Sunglasses &Sunglasses::operator=(Sunglasses &&source)
{
    if (this == &source)
        return *this;
    _asset_image = source._asset_image.clone();
    source._asset_image.release();
    return *this;
};

Sunglasses::Sunglasses(std::string path)
{
    _asset_image = cv::imread(path, cv::IMREAD_UNCHANGED);
}

void Sunglasses::Render(cv::Mat &image, std::vector<std::vector<cv::Point2f>> landmarks)
{
    if (!landmarks.empty())
    {
        for (int i = 0; i < landmarks.size(); i++)
        {
            cv::Mat warp_dst = cv::Mat::zeros(image.rows, image.cols, _asset_image.type());
            auto left_eye = landmarks[i][36];
            auto right_eye = landmarks[i][45];

            // Resize sunglasses to face size
            auto distance = cv::norm(right_eye - left_eye) * 1.45;
            double scale = distance / _asset_image.cols;
            int height = _asset_image.rows * scale;
            int wdith = (int)distance;
            cv::Mat resized_sunglasses;
            cv::resize(_asset_image, resized_sunglasses, cv::Size(wdith, height));

            // Translate sunglasses to face landmarks
            cv::Mat translate_mat = cv::Mat::eye(cv::Size(3, 2), CV_32F);
            auto mid_point = (left_eye + right_eye) / 2;
            translate_mat.at<float>(0, 2) = mid_point.x - resized_sunglasses.cols / 2;
            translate_mat.at<float>(1, 2) = mid_point.y - resized_sunglasses.rows / 2;
            warpAffine(resized_sunglasses, warp_dst, translate_mat, warp_dst.size());

            // Rotate sunglasses to align with face angle
            double angle = utils::FindAngleBetweenTwoLines(left_eye, right_eye, cv::Point(0, 0), cv::Point(1, 0));
            auto rot_mat = cv::getRotationMatrix2D(mid_point, angle, 1);
            warpAffine(warp_dst, warp_dst, rot_mat, warp_dst.size());

            // Split alpha channel to make a mask
            cv::Mat mask;
            std::vector<cv::Mat> channels(4);
            cv::split(warp_dst, channels);
            mask = channels.back();
            channels.pop_back();
            cv::Mat bgr_overlay;
            cv::merge(channels, bgr_overlay);

            // Draw overlay onto image
            bgr_overlay.copyTo(image, mask);
        }
    }
}