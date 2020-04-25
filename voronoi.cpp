/*
 * voronoi.cpp
 *
 *  Created on: 22 Apr 2020
 *      Author: ahkad
 */

#include "voronoi.h"
#include <vector>

voronoi_image::voronoi_image(int xmax_, int ymax_):
    xmax{xmax_},
    ymax{ymax_},
    cseg(Iso_rectangle_2(0,0,xmax,ymax)),
    rgen(0-extpercent/100,1+extpercent/100){
    }


void voronoi_image::add_random_points(size_t N){
    for(size_t i=0; i<N; ++i){
        //points.push_back(Point_2(rgen()*xmax,rgen()*ymax));
        points.emplace_back(rgen()*xmax,rgen()*ymax); // better
    }
    insert_points();
}

void voronoi_image::insert_points(){

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

const std::vector<Point_2>& voronoi_image::get_points() const{
    return points;
}

const std::list<Segment_2>& voronoi_image::get_voronoi_edges() const{
    return cseg.cropped_segments;
}

void voronoi_image::addCircle(imgType& image, int x, int y)
{
    cv::Point center(x,ymax-y);
    int radius{2};
    const cv::Scalar& color{cv::Scalar(255, 255, 255)};
    int lineType{cv::LINE_8};

    cv::circle(image, center, radius, color, cv::FILLED, lineType);
}

void voronoi_image::addLine(imgType& image, int x1, int y1, int x2, int y2)
{
    cv::Point start(x1,ymax-y1);
    cv::Point end(x2,ymax-y2);
    int thickness = 1;
    const cv::Scalar& color{cv::Scalar(255, 255, 255)};
    int lineType = cv::LINE_8;

    cv::line(image, start, end, color, thickness, lineType);
}

void voronoi_image::execute(imgType& image){

    const std::list<Segment_2>& edges = get_voronoi_edges();

    for(const auto& elem:points){
        addCircle(image, elem.x(),elem.y());
    }

    for(const auto& elem:edges){
        addLine(image, elem.source().x(), elem.source().y(),
                       elem.target().x(), elem.target().y());
    }
}

void voronoi_image::print_voronoi_edges() const{
    std::copy(cseg.cropped_segments.begin(),cseg.cropped_segments.end(),
      std::ostream_iterator<Segment_2>(std::cout,"\n"));
}
