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
    cseg(Iso_rectangle_2(0,0,xmax,ymax)),
    rgen(0-extpercent/100,1+extpercent/100){
    }

void voronoi_image::add_points(const std::vector<Point_2>& points){

    if(dt_or_vd){

        // Create Delaunay triangulation
        dtmesh.insert(points.begin(),points.end());
        //  Get Voronoi edges from DT and crop them.
        dtmesh.draw_dual(cseg);

    }else{

        // Create Voronoi diagram
        vdmesh.insert(points.begin(),points.end());

        // Crop Voronoi edges
        for(auto it=vdmesh.edges_begin(); it!=vdmesh.edges_end(); ++it){
            if(it->is_segment()){
                cseg<<Segment_2(it->source()->point(),it->target()->point());
            }

        }// end for
    }
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
    std::copy(cseg.cropped_segments.begin(),cseg.cropped_segments.end(),
      std::ostream_iterator<Segment_2>(std::cout,"\n"));
}

const std::list<Segment_2>& voronoi_image::get_voronoi_edges() const{
    return cseg.cropped_segments;
}
