/*
 * canvas.cpp
 *
 *  Created on: 22 Apr 2020
 *      Author: ahkad
 */

#include "canvas.h"

canvas::canvas(int rows_, int cols_, const std::string& wn/*="Default"*/){
    rows = rows_;
    cols = cols_;
    image = cv::Mat::zeros(rows, cols, CV_8UC3);
    window_name = wn;
}

canvas::canvas(const std::string& str, const std::string& wn/*="Default"*/){

    image = imread(str, cv::IMREAD_UNCHANGED);

    if(image.data==nullptr)
    {
        std::cout<<str<<" is not a valid image because of: "
                "missing file or improper permissions or invalid format)"<<std::endl;

        // try if it is a video format
        inVid.open(str);
        if (!inVid.isOpened())
        {
            std::cout<<"Video writer could not be initialized for file: "<<str<<std::endl;
            std::terminate();
        }

        cv::Size inSize = cv::Size(static_cast<int>(inVid.get(cv::CAP_PROP_FRAME_WIDTH)),
                                   static_cast<int>(inVid.get(cv::CAP_PROP_FRAME_HEIGHT)));

        std::cout<<"Video frame: Width="<<inSize.width<<"  Height="<<inSize.height
                 <<" and #Frames="<<inVid.get(cv::CAP_PROP_FRAME_COUNT)<<std::endl;

        rows = inSize.height;
        cols = inSize.width;
        isVideo = true;
    }
    else
    {
        rows = image.rows;
        cols = image.cols;
    }

    window_name = wn;
}

void canvas::show(int delay/*=0*/){
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::imshow(window_name, image);
    cv::waitKey(delay); // delay in ms
}
