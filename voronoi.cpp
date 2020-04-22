/*
 * voronoi.cpp
 *
 *  Created on: 22 Apr 2020
 *      Author: ahkad
 */

#include "voronoi.h"

voronoi_image::voronoi_image(int xmax_, int ymax_):
    xmax{xmax_},
    ymax{ymax_},
    vstruct(Iso_rectangle_2(0,0,xmax,ymax)),
    rgen(0,1){
    }

void voronoi_image::add_points(const std::vector<Point_2>& points){
    tmesh.insert(points.begin(),points.end()); // update tmesh
    //vmesh.insert(points.begin(),points.end()); //update vmesh
    tmesh.draw_dual(vstruct); // update vstruct
}

std::vector<Point_2> voronoi_image::add_random_points(size_t N){
    std::vector<Point_2> points;
    for(size_t i=0; i<N; ++i){
        //points.push_back(Point_2(rgen()*xmax,rgen()*ymax));
        points.emplace_back(rgen()*xmax,rgen()*ymax); // better
    }
    add_points(points);
    return points;
}

void voronoi_image::print_voronoi_edges() const{
    std::copy(vstruct.cropped_voronoi_segment.begin(),vstruct.cropped_voronoi_segment.end(),
      std::ostream_iterator<Segment_2>(std::cout,"\n"));
}

std::list<Segment_2>& voronoi_image::get_voronoi_edges() {
    return vstruct.cropped_voronoi_segment;
}


