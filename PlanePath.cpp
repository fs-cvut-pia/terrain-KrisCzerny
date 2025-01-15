#include "PlanePath.h"
#include <vector>
#include <algorithm>  // std::reverse

bool PlanePath::find() {
    // Pokud start == finish, není co hledat
    if (start == finish) {
        path.clear();
        path.push_back(start);
        return true;
    }

    // Pøíprava pole pro vzdálenosti a "parent" (pro rekonstrukci)
    std::vector<double> dist(map.nx * map.ny, std::numeric_limits<double>::infinity());
    std::vector<Point> parent(map.nx * map.ny, Point(-1, -1));

    // Pøíprava priority_queue
    std::priority_queue< PlaneState, std::vector<PlaneState>, std::greater<PlaneState> > pq;

    // Inicializace pro start
    dist[indexOf(start)] = 0.0;
    pq.push({ 0.0, start });

    // Definice 8 smìrù (vèetnì diagonál)
    static const int dx[8] = { 1,1,1,0,0,-1,-1,-1 };
    static const int dy[8] = { 1,0,-1,1,-1,1,0,-1 };

    // Dijkstra
    while (!pq.empty()) {
        auto [currCost, u] = pq.top();
        pq.pop();

        // Pokud je to zastaralý záznam, pøeskoèíme
        if (currCost > dist[indexOf(u)]) continue;

        // Pokud jsme došli do cíle
        if (u == finish) {
            // Rekonstrukce cesty
            path.clear();
            Point cur = finish;
            while (cur != Point(-1, -1)) {
                path.push_back(cur);
                cur = parent[indexOf(cur)];
            }
            std::reverse(path.begin(), path.end());
            return true;
        }

        // Projdeme 8 sousedù
        for (int i = 0; i < 8; i++) {
            Point v(u.x + dx[i], u.y + dy[i]);
            if (!map.validCoords(v)) continue;

            // Letadlo nemá žádné omezení na výšku, cost = eukl. vzdálenost
            double stepCost = euclidDist(u, v);
            double newCost = dist[indexOf(u)] + stepCost;

            // Uložíme, pokud je lepší
            if (newCost < dist[indexOf(v)]) {
                dist[indexOf(v)] = newCost;
                parent[indexOf(v)] = u;
                pq.push({ newCost, v });
            }
        }
    }

    // Cesta nenalezena
    return false;
}
