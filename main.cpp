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
    // const int rows{400}, cols{600};
    // canvas_image cimage(rows,cols);
    canvas_image cimage("./images/parrot.jpg");

    // create voronoi image
    voronoi_image vimage(cimage.get_cols(),cimage.get_rows());

    /*std::vector<Point_2> points;
    points.push_back(Point_2(0.25*cols,0.25*rows));
    points.push_back(Point_2(0.75*cols,0.25*rows));
    points.push_back(Point_2(0.5*cols,0.75*rows));
    vimage.add_points(points);*/

    std::vector<Point_2> points = vimage.add_random_points(100);

    //vimage.print_voronoi_edges();

    const std::list<Segment_2>& edges = vimage.get_voronoi_edges();

    for(const auto& elem:points){
        cimage.addCircle(elem.x(),elem.y());
    }

    for(const auto& elem:edges){
        cimage.addLine(elem.source().x(), elem.source().y(),
                       elem.target().x(), elem.target().y());
    }

    cimage.show_image(0);

    return 0;
}
