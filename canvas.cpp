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
            std::cout<<"Video reader could not be initialized for file: "<<str<<std::endl;
            std::terminate();
        }

        cv::Size inSize = cv::Size(static_cast<int>(inVid.get(cv::CAP_PROP_FRAME_WIDTH)),
                                   static_cast<int>(inVid.get(cv::CAP_PROP_FRAME_HEIGHT)));

        std::cout<<"Video frame: Width="<<inSize.width<<"  Height="<<inSize.height
                 <<" and #Frames="<<inVid.get(cv::CAP_PROP_FRAME_COUNT)<<std::endl;

        int codec_type = static_cast<int>(inVid.get(cv::CAP_PROP_FOURCC));

        rows = inSize.height;
        cols = inSize.width;
        isVideo = true;

        // initialize video writer
        std::string outFilename{"./data/out.avi"};
        outVid.open(outFilename, codec_type, inVid.get(cv::CAP_PROP_FPS), inSize, true);
        if (!outVid.isOpened())
        {
            std::cout<<"Video writer could not be initialized for file: "<<outFilename<<std::endl;
            std::terminate();
        }
    }
    else
    {
        rows = image.rows;
        cols = image.cols;
        std::cout<<"Image frame: Width="<<cols<<"  Height="<<rows<<std::endl;
    }

    window_name = wn;
}

void canvas::show(int delay/*=0*/) const{
    cv::imshow(window_name, image);
    cv::waitKey(delay); // delay in ms
}

void canvas::act(bool viewon){
    if(isVideo==false)
    {
        // delegate work to the actor
        actor->execute(image);
        writeImage();
        cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
        show(0);
    }else{
        int frameNum{-1};
        int delay = 1000/FPS;
        if(viewon) cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
        while(1)
        {
            inVid >> image;
            if (image.empty()) {break;}
            ++frameNum;
            if(frameNum%10==0) std::cout<<"Frame: " <<frameNum<<std::endl;
            // delegate work to the actor
            actor->execute(image);
            writeVideo();
            if(viewon) show(delay);
        }
    }
}

void canvas::writeImage(){
    cv::imwrite("./data/out.jpg", image); // write image
}

void canvas::writeVideo(){
    outVid << image;
}
