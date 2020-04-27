/*
 * voronoi.cpp
 *
 *  Created on: 22 Apr 2020
 *      Author: ahkad
 */

#include "voronoi.h"
#include <algorithm>
#include <cassert>
#include <valarray>

voronoi_image::voronoi_image(int xmax_, int ymax_):
    xmax{xmax_},
    ymax{ymax_},
    cseg(Iso_rectangle_2(0,0,xmax,ymax)),
    rgen(0-extpercent/100,1+extpercent/100){}

void voronoi_image::add_random_sites(size_t N){
    for(size_t i=0; i<N; ++i){
        //sites.push_back(Point_2(rgen()*xmax,rgen()*ymax));
        sites.emplace_back(rgen()*xmax,rgen()*ymax); // better
    }
    insert_sites();
}

void voronoi_image::insert_sites(){

    // Create Voronoi diagram
    vdmesh.insert(sites.cbegin(),sites.cend());

    // Crop Voronoi edges for display purpose only
    for(auto it=vdmesh.edges_begin(); it!=vdmesh.edges_end(); ++it){
        if(it->is_segment()){
            cseg<<Segment_2(it->source()->point(),it->target()->point());
        }
    }

    assert(vdmesh.number_of_faces()==sites.size());

    print_voronoi_diagram_info();

    update_centroidIndexPairVec();

    assignPixelsToFaces();
}

void voronoi_image::update_centroidIndexPairVec(){
    size_t index{0};
    for (const auto &p : sites) {
        VD::Locate_result lr = vdmesh.locate(p);
        if (VD::Face_handle *f = boost::get<VD::Face_handle>(&lr)) {
            if (!(*f)->is_unbounded()) {
                VD::Ccb_halfedge_circulator ec_start = (*f)->ccb();
                VD::Ccb_halfedge_circulator ec = ec_start;
                std::vector<Point_2> vec;
                do {
                    vec.push_back(ec->source()->point());
                } while (++ec != ec_start);
                cindex.emplace_back(CGAL::centroid(vec.cbegin(), vec.cend()), index);
            }
        } else {
            assert("Site not located on a face\n" && false);
        }
        ++index;
    }
    std::cout<<"Total bounded faces are "<<cindex.size()<<std::endl;
    // sort vector by y coordinate
    auto lambda = [](const centroidIndexPair& p1, const centroidIndexPair& p2){
        if(p1.first.y()==p2.first.y())
            return p1.first.x()<p2.first.x();
        else
            return p1.first.y()<p2.first.y();
    };
    std::sort(cindex.begin(),cindex.end(),lambda);
    //for(const auto& e:cindex) std::cout<<e.first<<" "<<e.second<<std::endl;
}

void voronoi_image::assignPixelsToFaces(){

    facelist.resize(vdmesh.number_of_faces());

    for(int i=0; i<xmax; ++i){
        for(int j=0; j<ymax; ++j){

            VD::Locate_result lr = vdmesh.locate(Point_2(i, j));

            if (VD::Face_handle *f = boost::get<VD::Face_handle>(&lr)) {

                if (!(*f)->is_unbounded()) {

                    VD::Ccb_halfedge_circulator ec_start = (*f)->ccb();
                    VD::Ccb_halfedge_circulator ec = ec_start;
                    std::vector<Point_2> vec;
                    do {
                        vec.push_back(ec->source()->point());
                    } while (++ec != ec_start);

                    Point_2 c = CGAL::centroid(vec.cbegin(), vec.cend());

                    auto lambda = [](const centroidIndexPair& p1,
                            const centroidIndexPair& p2){
                        if(p1.first.y()==p2.first.y())
                            return p1.first.x()<p2.first.x();
                        else
                            return p1.first.y()<p2.first.y();
                    };

                    auto it = std::lower_bound(cindex.cbegin(), cindex.cend(),
                            std::make_pair(c,0), lambda);

                    if (it == cindex.cend())
                        assert("Bounded face not found in list\n" && false);

                    facelist[it->second].emplace_back(i, j);
                }
            } else {
                assert("Site not located on a face\n" && false);
            } // end if
        } // end inner loop
    } // end outer loop

    /*int i{0}, sum{0};
    for(const auto& fl:facelist){
        sum+=fl.size();
        std::cout<<i++<<" facelist has size "<<fl.size()<<std::endl;
    }
    std::cout<<"Total sum = "<<sum<<std::endl;*/
}

void voronoi_image::addCircle(imgType& image, int x, int y,
        cv::Scalar color = cv::Scalar(255, 255, 255))
{
    cv::Point center(x,ymax-y);
    int radius{2};
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

void voronoi_image::updatePixels(imgType& image) {

    int nc{image.channels()}; // get #channels

    for (const auto &face : facelist){

        if (!face.empty()) {

            std::valarray<double> avg(nc);

            for (const auto &xypair : face) {

                assert(xypair.first<xmax && xypair.second<ymax);

                uchar *p = image.ptr < uchar > (ymax - xypair.second - 1);
                for (int ch = 0; ch < nc; ++ch) {
                    avg[ch] += p[xypair.first * nc + ch];
                }
            }

            avg/=face.size();

            /*std::cout << "Average color ";
            for (int ch = 0; ch < nc; ++ch) {
                std::cout << avg[ch] << " ";
            }
            std::cout << std::endl;*/

            for (const auto &xypair : face) {
                uchar *p = image.ptr < uchar > (ymax - xypair.second - 1);
                for (int ch = 0; ch < nc; ++ch) {
                    p[xypair.first * nc + ch] = static_cast<uchar>(avg[ch]);
                }
            }
        }
    }

}

void voronoi_image::execute(imgType& image){

    // Add sites
    /*for(const auto& p:sites){
        addCircle(image, p.x(), p.y());
    }*/

    // Add edges
    /*const std::list<Segment_2>& edges = get_voronoi_edges();
    for(const auto& e:edges){
        addLine(image, e.source().x(), e.source().y(),
                       e.target().x(), e.target().y());
    }*/

    // Add centroids of bounded faces
    /*for(const auto& p:cindex){
        addCircle(image, p.first.x(), p.first.y(), cv::Scalar(0, 0, 255));
    }*/

    // update pixels
    updatePixels(image);
}

const std::vector<Point_2>& voronoi_image::get_sites() const{
    return sites;
}

const std::list<Segment_2>& voronoi_image::get_voronoi_edges() const{
    return cseg.cropped_segments;
}

void voronoi_image::print_cropped_segments() const{
    std::copy(cseg.cropped_segments.begin(),cseg.cropped_segments.end(),
      std::ostream_iterator<Segment_2>(std::cout,"\n"));
}

void voronoi_image::print_voronoi_diagram_info() const{
    std::cout<<"Total vertices = "<<vdmesh.number_of_vertices()<<std::endl;
    std::cout<<"Total edges (including un-bounded) = "<<vdmesh.number_of_halfedges()/2<<std::endl;
    std::cout<<"Total faces (including un-bounded) = "<<vdmesh.number_of_faces()<<std::endl;
}
