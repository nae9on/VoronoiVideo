/*
 * main.cpp
 *
 *  Created on: 21 Apr 2020
 *      Author: ahkad
 */

#include "canvas.h"
#include "voronoi.h"

int main(){

    // create background image
    //canvas cimage("./images/parrot.jpg");
    canvas cimage("./data/in.avi");

    // create voronoi image
    voronoi_image vimage(cimage.get_cols(),cimage.get_rows());
    vimage.add_random_points(100);

    cimage.setAction(&vimage);

    cimage.act();

    return 0;
}
