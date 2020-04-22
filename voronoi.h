/*
 * voronoi.h
 *
 *  Created on: 22 Apr 2020
 *      Author: ahkad
 */

#ifndef VORONOI_H_
#define VORONOI_H_

#include "boost_random.h"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Voronoi_diagram_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_traits_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_policies_2.h>
#include <vector>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point_2 = K::Point_2;
using Iso_rectangle_2 = K::Iso_rectangle_2;
using Segment_2 = K::Segment_2;
using Ray_2 = K::Ray_2;
using Line_2 = K::Line_2;
using DT = CGAL::Delaunay_triangulation_2<K>;
using AT = CGAL::Delaunay_triangulation_adaptation_traits_2<DT>;
using AP = CGAL::Delaunay_triangulation_caching_degeneracy_removal_policy_2<DT>;
using VD = CGAL::Voronoi_diagram_2<DT,AT,AP>;

struct cropped_voronoi_from_delaunay{

    cropped_voronoi_from_delaunay(const Iso_rectangle_2& bbox_):bbox(bbox_){}

    Iso_rectangle_2 bbox;
    std::list<Segment_2> cropped_voronoi_segment;

    template<class RSL>
    void crop_and_extract_segment(const RSL& rsl){
        CGAL::Object obj = CGAL::intersection(rsl,bbox);
        const Segment_2* s=CGAL::object_cast<Segment_2>(&obj);
        if(s) cropped_voronoi_segment.push_back(*s);
        }

    void operator<<(const Ray_2& ray)    {crop_and_extract_segment(ray);}
    void operator<<(const Line_2& line)  {crop_and_extract_segment(line);}
    void operator<<(const Segment_2& seg){crop_and_extract_segment(seg);}
};

class voronoi_image{
public:
    voronoi_image() = delete;
    voronoi_image(int xmax_, int ymax_);
    void add_points(const std::vector<Point_2>& points);
    std::vector<Point_2>& get_points();
    std::vector<Point_2> add_random_points(size_t N);
    void print_voronoi_edges() const;
    std::list<Segment_2>& get_voronoi_edges();
private:
    int xmax;
    int ymax;
    DT tmesh;
    VD vmesh;
    cropped_voronoi_from_delaunay vstruct;
    boost_real rgen;
};

#endif /* VORONOI_H_ */
