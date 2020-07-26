#ifndef ASSET_H_
#define ASSET_H_
#include <memory>
#include <opencv2/opencv.hpp>

class Asset
{
public:
    Asset(){};
    ~Asset();
    Asset(std::string path);
    virtual void Render(cv::Mat &image, std::vector<std::vector<cv::Point2f>> landmarks)
    {
        std::cout << "This shouldn't be called" << std::endl;
    };

protected:
    cv::Mat _asset_image;
};

#endif // ASSET_H_