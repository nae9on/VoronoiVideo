/*
 * canvas.h
 *
 *  Created on: 22 Apr 2020
 *      Author: ahkad
 */

#ifndef CANVAS_H_
#define CANVAS_H_

#include "action.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

class canvas{
public:
    canvas() = delete;
    canvas(int rows, int cols, const std::string& wn="Default");
    canvas(const std::string& str, const std::string& wn="Default");
    int get_rows(){return rows;}
    int get_cols(){return cols;}
    void show(int delay=0);
    void setAction(action* ac_){
        ac = ac_;
    }
    void act(){
        if(isVideo==false)
        {
            ac->execute(image);
            show(0);
        }else{
            int frameNum{-1};
            int delay{33};
            for(;;)
            {
                inVid >> image;
                if (image.empty()){break;}
                ++frameNum;
                if(frameNum%10==0) std::cout<<"Frame: " <<frameNum<<std::endl;
                ac->execute(image);
                show(delay);
            }
        }
    }
private:
    int rows;
    int cols;
    bool isVideo = false;
    cv::Mat_<cv::Vec3b> image;
    cv::VideoCapture inVid;
    std::string window_name;
    action* ac = nullptr;
};

#endif /* CANVAS_H_ */
