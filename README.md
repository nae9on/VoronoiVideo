# Voronoi Video

# Introduction

In this project, a 2D [Voronoi mesh](https://en.wikipedia.org/wiki/Voronoi_diagram) is used to create an image filter for providing a “stained glass” or mosaic effect on an image. The 2D Voronoi mesh adaptor from [The Computational Geometry Algorithms Library](https://doc.cgal.org/latest/Voronoi_diagram_2/index.html#Chapter_2D_Voronoi_Diagram_Adaptor) is integrated with [OpenCV library](https://docs.opencv.org/master/). After decomposing a series of images into 2D Voronoi mesh and rendering average color to each mesh cell, a sparse Voronoi video representation is generated.

# Requirements

[The Computational Geometry Algorithms Library](https://doc.cgal.org/latest/Voronoi_diagram_2/index.html#Chapter_2D_Voronoi_Diagram_Adaptor)

[OpenCV library](https://docs.opencv.org/master/)

[Boost.Random](https://www.boost.org/doc/libs/1_72_0/doc/html/boost_random.html)

# Other information

The build is tested on Windows with [MinGW distribution](https://nuwen.net/mingw.html) containing GCC 8.2.0 and Boost 1.69.0. Version CGAL-5.0.2 and OpenCV 4.20 are used during build. The project is organised following a [Strategy design pattern](https://en.wikipedia.org/wiki/Strategy_pattern).







