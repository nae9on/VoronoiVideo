/*
 * canvas.h
 *
 *  Created on: 22 Apr 2020
 *      Author: ahkad
 */

#ifndef CANVAS_H_
#define CANVAS_H_

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

class canvas_image{
public:
    canvas_image() = delete;
    canvas_image(int rows, int cols, const std::string& wn="Default");
    canvas_image(const std::string& , const std::string& wn="Default");
    int get_rows(){return rows;}
    int get_cols(){return cols;}
    void show_image(int delay=0);
    void addCircle(int x, int y);
    void addLine(int x1, int y1, int x2, int y2);
private:
    int rows;
    int cols;
    cv::Mat_<cv::Vec3b> image;
    std::string window_name;
};

#endif /* CANVAS_H_ */
