#ifndef SUNGLASSES_H_
#define SUNGLASSES_H_
#include "asset.h"

class Sunglasses : public Asset
{
public:
    Sunglasses(Sunglasses &source);
    Sunglasses operator=(Sunglasses &source);
    Sunglasses(Sunglasses &&source);
    Sunglasses &operator=(Sunglasses &&source);
    Sunglasses(std::string path);
    virtual void Render(cv::Mat &image, std::vector<std::vector<cv::Point2f>> landmarks);
};

#endif // SUNGLASSES_H_