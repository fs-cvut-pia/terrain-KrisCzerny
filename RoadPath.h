#ifndef ROAD_PATH_H
#define ROAD_PATH_H

#include "Path.h"
#include <queue>
#include <limits>
#include <cmath>

struct RoadState {
    double cost;
    Point p;
    bool operator>(const RoadState& o) const {
        return cost > o.cost;
    }
};

class RoadPath : public Path {
public:
    RoadPath(TerrainMap& m, std::string name_in, Point start_in, Point finish_in)
        : Path(m, name_in, start_in, finish_in) {}

    bool find() override;

private:
    inline size_t indexOf(Point p) const {
        return p.y * map.nx + p.x;
    }
    inline double euclidDist(const Point& a, const Point& b) const {
        double dx = double(a.x - b.x);
        double dy = double(a.y - b.y);
        return std::sqrt(dx * dx + dy * dy);
    }
};

#endif
