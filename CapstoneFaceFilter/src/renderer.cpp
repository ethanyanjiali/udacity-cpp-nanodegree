#include <memory>
#include <iostream>
#include <exception>
#include <ctime>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include "renderer.h"
#include "face_detector.h"
#include "utils.h"
#include "asset.h"
#include "sunglasses.h"

Renderer::Renderer()
{
    std::vector<std::string> sunglasses_paths{
        "../assets/images/sunglasses1.png",
        "../assets/images/sunglasses2.png",
        "../assets/images/sunglasses3.png",
    };

    for (auto path : sunglasses_paths)
    {
        _assets.emplace_back(std::make_unique<Sunglasses>(path));
    }
}

void Renderer::LoadCanvasFromImage(std::string path)
{
    cv::Mat image;
    image = cv::imread(path, 1);

    if (!image.data)
    {
        throw std::invalid_argument("No image data.");
    }
    else
    {
        canvas = std::move(image);
    }
}

void Renderer::Start(std::string path, int asset_id)
{
    LoadCanvasFromImage(path);
    Render(asset_id);
    cv::namedWindow("Face Filter", cv::WINDOW_AUTOSIZE);
    imshow("Face Filter", canvas);
    cv::waitKey(0);
}

void Renderer::Start(int asset_id)
{
    cv::Mat frame;
    cv::VideoCapture cap;
    cap.open(0, cv::CAP_ANY);
    if (!cap.isOpened())
    {
        return;
    }

    std::time_t timeBegin = std::time(0);
    int tick = 0;
    long frame_counter = 0;

    for (;;)
    {
        cap.read(frame);
        if (frame.empty())
        {
            break;
        }

        canvas = std::move(frame);
        Render(asset_id);

        imshow("Face Filter", canvas);

        frame_counter++;
        std::time_t timeNow = std::time(0) - timeBegin;
        if (timeNow - tick >= 1)
        {
            tick++;
            std::cout << "Frames per second: " << frame_counter << std::endl;
            frame_counter = 0;
        }

        if (cv::waitKey(5) >= 0)
            break;
    }
}

void Renderer::Render(int asset_id)
{
    auto faces = face_detector.Detect(canvas);
    // DrawBoundingBoxes(canvas, faces, "face");

    auto landmarks = face_landmark.DetectLandmarks(canvas, faces);
    // DrawLandmarks(canvas, landmarks);

    _assets[asset_id]->Render(canvas, landmarks);
}

void Renderer::DrawBoundingBoxes(cv::Mat &image, std::vector<cv::Rect> boxes, std::string name)
{

    for (auto box : boxes)
    {
        cv::rectangle(image, box, cv::Scalar(255, 0, 0));
        if (!name.empty())
        {
            cv::putText(image, name, cv::Point(box.x, box.y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(255, 0, 0));
        }
    }
}

void Renderer::DrawLandmarks(cv::Mat &image, std::vector<std::vector<cv::Point2f>> landmarks)
{
    if (!landmarks.empty())
    {
        for (int i = 0; i < landmarks.size(); i++)
        {
            for (int j = 0; j < landmarks[i].size(); j++)
            {
                cv::circle(image, landmarks[i][j], 5, cv::Scalar(0, 0, 255), cv::FILLED);
            }
        }
    }
}