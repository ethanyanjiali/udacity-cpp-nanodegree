#ifndef RENDERER_H_
#define RENDERER_H_
#include <string>
#include <opencv2/opencv.hpp>
#include "face_detector.h"
#include "face_landmark.h"
#include "asset.h"

class Renderer
{
public:
    Renderer();
    void Start(int asset_id);
    void Start(std::string path, int asset_id);
    void LoadCanvasFromImage(std::string path);

private:
    void Render(int asset_id);
    void DrawBoundingBoxes(cv::Mat &image, std::vector<cv::Rect> boxes, std::string name = "");
    void DrawLandmarks(cv::Mat &image, std::vector<std::vector<cv::Point2f>> landmarks);
    void DrawSunglasses(cv::Mat &image, std::vector<std::vector<cv::Point2f>> landmarks);

    cv::Mat canvas;
    std::vector<std::unique_ptr<Asset>> _assets;
    HaarCascadeFaceDetector face_detector;
    KazemiFaceLandmark face_landmark;
};

#endif // RENDERER_H_