/*
 * action.h
 *
 *  Created on: 25 Apr 2020
 *      Author: ahkad
 */

#ifndef ACTION_H_
#define ACTION_H_

#include <opencv2/opencv.hpp>

// interface class
class action{
public:
    action() = default;
    virtual void execute(cv::Mat_<cv::Vec3b>& image) = 0;
    virtual ~action(){}
private:
};

#endif /* ACTION_H_ */
