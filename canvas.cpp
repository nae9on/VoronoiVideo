/*
 * canvas.cpp
 *
 *  Created on: 22 Apr 2020
 *      Author: ahkad
 */

#include "canvas.h"

canvas_image::canvas_image(int rows_, int cols_, const std::string& wn/*="Default"*/){
    rows = rows_;
    cols = cols_;
    image = cv::Mat::zeros(rows, cols, CV_8UC3);
    window_name = wn;
}

canvas_image::canvas_image(const std::string& str, const std::string& wn/*="Default"*/){
    image = imread(str, cv::IMREAD_UNCHANGED);
    rows = image.rows;
    cols = image.cols;
    window_name = wn;
}

void canvas_image::show_image(int delay/*=0*/){
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::imshow(window_name, image);
    cv::waitKey(delay); // delay in ms
}

void canvas_image::addCircle(int x, int y)
{
    cv::Point center(x,rows-y);
    int radius{2};
    const cv::Scalar& color{cv::Scalar(255, 255, 255)};
    int lineType{cv::LINE_8};

    cv::circle(image, center, radius, color, cv::FILLED, lineType);
}

void canvas_image::addLine(int x1, int y1, int x2, int y2)
{
    cv::Point start(x1,rows-y1);
    cv::Point end(x2,rows-y2);
    int thickness = 1;
    const cv::Scalar& color{cv::Scalar(255, 255, 255)};
    int lineType = cv::LINE_8;

    cv::line(image, start, end, color, thickness, lineType);
}
