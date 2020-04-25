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
    using imgType = cv::Mat_<cv::Vec3b>;
public:
    canvas() = delete;
    canvas(int rows, int cols, const std::string& wn="Default");
    canvas(const std::string& str, const std::string& wn="Default");
    int get_rows() const {return rows;}
    int get_cols()const {return cols;}
    void show(int delay=0) const;
    void setAction(action* actptr_){
        actptr = actptr_;
    }
    void act();
    void writeImage();
    void writeVideo();
private:
    int rows;
    int cols;
    int FPS = 30;
    bool isVideo{false};
    imgType image;
    cv::VideoCapture inVid;
    cv::VideoWriter outVid;
    std::string window_name;
    action* actptr{nullptr};
};

#endif /* CANVAS_H_ */
