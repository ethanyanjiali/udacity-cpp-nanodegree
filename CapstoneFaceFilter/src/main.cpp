#include <iostream>
#include <opencv2/opencv.hpp>
#include "renderer.h"
#include "utils.h"

int main()
{
    int input_mode = utils::GetInputMode();
    while (input_mode != 1 && input_mode != 2)
    {
        std::cout << input_mode << " is not a valid option." << std::endl;
        input_mode = utils::GetInputMode();
    }

    int asset_id = utils::GetAssetId();
    while (asset_id < 0 || asset_id > 2)
    {
        std::cout << asset_id << " is not a valid option." << std::endl;
        asset_id = utils::GetAssetId();
    }

    Renderer renderer = Renderer();

    if (input_mode == 1)
    {
        renderer.Start(asset_id);
    }
    else if (input_mode == 2)
    {
        std::string path = utils::GetPhotoFilePath("../assets/images/face.jpg");
        renderer.Start(path, asset_id);
    }

    return 0;
}
