/*
 * voronoi_example.cpp
 *
 *  Created on: 26 Apr 2020
 *      Author: ahkad
 *
 *  Reference:
 *  https://doc.cgal.org/latest/Voronoi_diagram_2/index.html#Chapter_2D_Voronoi_Diagram_Adaptor
 */

// standard includes
#include <iostream>
#include <fstream>
#include <cassert>

// includes for defining the Voronoi diagram adaptor
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Voronoi_diagram_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_traits_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_policies_2.h>

// typedefs for defining the adaptor
typedef CGAL::Exact_predicates_inexact_constructions_kernel                  K;
typedef CGAL::Delaunay_triangulation_2<K>                                    DT;
typedef CGAL::Delaunay_triangulation_adaptation_traits_2<DT>                 AT;
typedef CGAL::Delaunay_triangulation_caching_degeneracy_removal_policy_2<DT> AP;
typedef CGAL::Voronoi_diagram_2<DT,AT,AP>                                    VD;

// typedef for the result type of the point location
typedef AT::Site_2                    Site_2;
typedef AT::Point_2                   Point_2;
typedef VD::Locate_result             Locate_result;
typedef VD::Vertex_handle             Vertex_handle;
typedef VD::Face_handle               Face_handle;
typedef VD::Halfedge_handle           Halfedge_handle;
typedef VD::Ccb_halfedge_circulator   Ccb_halfedge_circulator;

void print_endpoint(Halfedge_handle e, bool is_src) {
    if (is_src) {
        if (e->has_source())
            std::cout << e->source()->point() << " ";
        else
            std::cout << "inf" << " ";
    } else {
        if (e->has_target())
            std::cout << e->target()->point() << ", ";
        else
            std::cout << "inf" << ", ";
    }
}

int main2()
{
    std::ifstream ifs("data/ex_voronoi_points.txt");
    assert(ifs);

    VD vd;
    Site_2 t;
    while (ifs>>t) {vd.insert(t);}
    ifs.close();
    assert(vd.is_valid());

    int totalVertices{0};
    for (VD::Vertex_iterator it = vd.vertices_begin(); it != vd.vertices_end(); ++it) {
        // std::cout<<it->degree()<<" "; // returns #num edges connected to a vertex
        std::cout<<it->point()<<",  ";
        if((totalVertices+1)%5==0) std::cout<<std::endl;
        ++totalVertices;
    }
    std::cout<<"Total vertices = "<<vd.number_of_vertices()<<std::endl<<std::endl;

    int totalFiniteEdges{0}, totalEdges{0};
    for (VD::Edge_iterator it = vd.edges_begin(); it != vd.edges_end(); ++it) {
        if (it->has_source())
            std::cout << it->source()->point() << " ";
        else
            std::cout << "inf" << " ";
        if (it->has_target())
            std::cout << it->target()->point() << ", ";
        else
            std::cout << "inf" << ", ";
        if((totalEdges+1)%5==0) std::cout<<std::endl;
        if (it->has_source() && it->has_target())
            ++totalFiniteEdges;
        ++totalEdges;
    }
    std::cout<<std::endl;
    std::cout<<"Total finite edges = "<<totalFiniteEdges<<std::endl;
    std::cout<<"Total infinite edges = "<<totalEdges-totalFiniteEdges<<std::endl;
    std::cout<<"Total half-edges = "<<vd.number_of_halfedges()<<std::endl<<std::endl;

    for (VD::Face_iterator it = vd.faces_begin(); it != vd.faces_end(); ++it) {
        if (!it->is_unbounded()) {
            Ccb_halfedge_circulator ec_start = it->ccb();
            Ccb_halfedge_circulator ec = ec_start;
            do {
                print_endpoint(ec, true);
                print_endpoint(ec, false);
            } while (++ec != ec_start);
            std::cout << "\n";
        }
    }
    std::cout<<"Total faces = "<<vd.number_of_faces()<<"\n\n";

    std::ifstream ifq("data/ex_query_points.txt");
    assert(ifq);
    Point_2 p;
    while (ifq >> p) {
        std::cout << "Query point (" << p.x() << "," << p.y()
                << ") lies on a Voronoi " << std::flush;
        Locate_result lr = vd.locate(p);
        if (Vertex_handle *v = boost::get<Vertex_handle>(&lr)) {
            std::cout << "vertex." << std::endl;
            std::cout << "The Voronoi vertex is: " << (*v)->point() << std::endl;
        } else if (Halfedge_handle *e = boost::get<Halfedge_handle>(&lr)) {
            std::cout << "edge." << std::endl;
            std::cout << "The source and target vertices "
                    << "of the Voronoi edge are:" << std::endl;
            print_endpoint(*e, true);
            print_endpoint(*e, false);
            std::cout<<std::endl;
        } else if (Face_handle* f = boost::get<Face_handle>(&lr)) {
            std::cout << "face." << std::endl;
            std::cout << "The vertices of the Voronoi face are"
                    << " (in counterclockwise order):" << std::endl;
            Ccb_halfedge_circulator ec_start = (*f)->ccb();
            Ccb_halfedge_circulator ec = ec_start;
            do {
                print_endpoint(ec, true);
                print_endpoint(ec, false);
            } while (++ec != ec_start);
            std::cout<<std::endl;
        }
        std::cout << std::endl;
    }
    ifq.close();
    return 0;
}
