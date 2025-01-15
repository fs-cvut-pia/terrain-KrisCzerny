#include "BoatPath.h"
#include <vector>
#include <algorithm>

bool BoatPath::find() {
    if (start == finish) {
        path.clear();
        path.push_back(start);
        return true;
    }

    std::vector<double> dist(map.nx * map.ny, std::numeric_limits<double>::infinity());
    std::vector<Point> parent(map.nx * map.ny, Point(-1, -1));

    std::priority_queue<BoatState, std::vector<BoatState>, std::greater<BoatState>> pq;

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

            // Loï mùže plout jen tam, kde alt < 0 (kromì startu a finish)
            // => Pokud v != finish a v != start, pak alt < 0
            if (v != finish && v != start) {
                if (map.alt(v) >= 0) continue;
            }
            // Cost = euklidovská vzdálenost
            double stepCost = euclidDist(u, v);
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
