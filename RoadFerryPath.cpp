#include "RoadFerryPath.h"
#include <vector>
#include <algorithm>

bool RoadFerryPath::find() {
    if (start == finish) {
        path.clear();
        path.push_back(start);
        return true;
    }

    std::vector<double> dist(map.nx * map.ny, std::numeric_limits<double>::infinity());
    std::vector<Point> parent(map.nx * map.ny, Point(-1, -1));

    std::priority_queue<RFState, std::vector<RFState>, std::greater<RFState>> pq;

    dist[indexOf(start)] = 0.0;
    pq.push({ 0.0, start });

    static const int dx[8] = { 1,1,1,0,0,-1,-1,-1 };
    static const int dy[8] = { 1,0,-1,1,-1,1,0,-1 };

    while (!pq.empty()) {
        auto [currCost, u] = pq.top();
        pq.pop();

        if (currCost > dist[indexOf(u)]) continue;
        if (u == finish) {
            // Rekonstrukce
            path.clear();
            Point cur = finish;
            while (cur != Point(-1, -1)) {
                path.push_back(cur);
                cur = parent[indexOf(cur)];
            }
            std::reverse(path.begin(), path.end());
            return true;
        }

        for (int i = 0; i < 8; i++) {
            Point v(u.x + dx[i], u.y + dy[i]);
            if (!map.validCoords(v)) continue;

            double d = euclidDist(u, v);

            int altU = map.alt(u);
            int altV = map.alt(v);

            double stepCost;
            // Pokud obì pole >= 0, hlídáme slope < 6 %, cost = d
            if (altU >= 0 && altV >= 0) {
                double slope = std::fabs(double(altV - altU)) / d;
                if (slope > 0.06) {
                    continue; // nelze
                }
                stepCost = d; // rychlost = 1
            }
            else {
                // Alespoò jedno pole < 0 => cost = 4*d, 
                // stoupání se neøeší
                stepCost = 4.0 * d;
            }

            double newCost = dist[indexOf(u)] + stepCost;
            if (newCost < dist[indexOf(v)]) {
                dist[indexOf(v)] = newCost;
                parent[indexOf(v)] = u;
                pq.push({ newCost, v });
            }
        }
    }

    return false;
}
