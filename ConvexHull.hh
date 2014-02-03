#ifndef CONVEXHULL_HH
#define CONVEXHULL_HH

#include <algorithm>
#include <vector>

typedef int coord_t;
typedef long long coord2_t;

struct Point2d {
    coord_t x, y;

    bool operator <(const Point &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
};

coord2_t cross(const Point2d &O, const Point2d &A, const Point2d &B);

std::vector<Point2d> convex_hull(std::vector<Point2d> P);


#endif
