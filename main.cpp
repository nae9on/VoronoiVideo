/*
 * main.cpp
 *
 *  Created on: 21 Apr 2020
 *      Author: ahkad
 */

#include "canvas.h"
#include "voronoi.h"

#include <string>

int main(){

    // create background image
    //std::string filename{"./images/cheetah.jpg"};
    std::string filename{"./data/in.avi"};
    canvas cimage(filename);

    // create voronoi image
    voronoi_image vimage(cimage.get_cols(),cimage.get_rows());
    vimage.add_random_sites(10000);

    cimage.setActor(&vimage);

    cimage.act(true);

    return 0;
}
