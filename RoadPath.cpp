#include "RoadPath.h"
#include <vector>
#include <algorithm>

bool RoadPath::find() {
    if (start == finish) {
        path.clear();
        path.push_back(start);
        return true;
    }

    std::vector<double> dist(map.nx * map.ny, std::numeric_limits<double>::infinity());
    std::vector<Point> parent(map.nx * map.ny, Point(-1, -1));

    std::priority_queue<RoadState, std::vector<RoadState>, std::greater<RoadState>> pq;

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

        // Kontrola, zda i start/finish mají alt>0? 
        // Zadání øíká "pouze po polích, kde výška > 0", 
        // vìtšinou to chápeme i pro start/finish, 
        // ale lze doladit, pokud to je jinak.
        if (map.alt(u) <= 0) continue;

        for (int i = 0; i < 8; i++) {
            Point v(u.x + dx[i], u.y + dy[i]);
            if (!map.validCoords(v)) continue;
            if (map.alt(v) <= 0) continue; // silnice jen alt>0

            double distUV = euclidDist(u, v);
            double altU = (double)map.alt(u);
            double altV = (double)map.alt(v);
            double slope = std::fabs(altV - altU) / distUV;
            if (slope > 0.06) {
                continue; // pøekroèení max. sklonu
            }

            double newCost = dist[indexOf(u)] + distUV;
            if (newCost < dist[indexOf(v)]) {
                dist[indexOf(v)] = newCost;
                parent[indexOf(v)] = u;
                pq.push({ newCost, v });
            }
        }
    }

    return false;
}
