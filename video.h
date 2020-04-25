/*
 * video.h
 *
 *  Created on: 23 Apr 2020
 *      Author: ahkad
 */

#ifndef VIDEO_H_
#define VIDEO_H_

#include "canvas.h"
#include "voronoi.h"
#include <memory>

class video{
public:
    video(){

    }
private:
    std::unique_ptr<canvas_image> ci;
    std::unique_ptr<voronoi_image> vi;
};



#endif /* VIDEO_H_ */
