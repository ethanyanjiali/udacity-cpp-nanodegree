#include "utils.h"
#include "asset.h"

Asset::Asset(std::string path)
{
    _asset_image = cv::imread(path, cv::IMREAD_UNCHANGED);
}

Asset::~Asset()
{
    if (!_asset_image.empty())
    {
        _asset_image.release();
    }
};