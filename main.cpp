#include "TerrainMap.h"
#include "Path.h"
#include <vector>
#include <iostream>
#include <string>

// Pøidáváme naše odvozené tøídy s implementací find():
#include "PlanePath.h"
#include "BoatPath.h"
#include "RoadPath.h"
#include "RoadFerryPath.h"
#include "RailPath.h"

int main(int argc, char* argv[]) {
    const int nx = 256;
    const int ny = 256;

    // 1) Pokusíme se naèíst název souboru s mapou z argumentu
    std::string terrain_filename;
    if (argc > 1) {
        terrain_filename = argv[1];
    }
    else {
        std::cout << "No terrain file specified! Použije se výchozí: terrain.dat" << std::endl;
        terrain_filename = "terrain.dat";
    }

    // 2) Vytvoøíme mapu
    TerrainMap m(nx, ny, terrain_filename);

    // 3) Zeptáme se uživatele na souøadnice
    int startX, startY;
    std::cout << "Zadejte souøadnice poèátku (x y): ";
    std::cin >> startX >> startY;

    int finishX, finishY;
    std::cout << "Zadejte souøadnice konce (x y): ";
    std::cin >> finishX >> finishY;

    // 4) Vytvoøíme objekty typu Point
    Point start(startX, startY);
    Point finish(finishX, finishY);

    // 5) Pøidáme pìt variant cest
    std::vector<Path*> paths = {
       new PlanePath(m, "Plane",       start, finish),
       new BoatPath(m,  "Boat",        start, finish),
       new RoadPath(m,  "Road",        start, finish),
       new RoadFerryPath(m, "Road+Ferry", start, finish),
       new RailPath(m,  "Rail",        start, finish)
    };

    // 6) Spustíme hledání pro každou variantu
    for (auto& p : paths) {
        std::cout << "Path search: " << p->getName() << std::endl;
        std::cout << "=============" << std::endl;
        bool ok = p->find();
        if (!ok) {
            std::cout << "Path not found!" << std::endl;
        }
        p->printStats();
        std::cout << "=============" << std::endl;
        p->saveToFile(); // Uloží se napø. do "Plane.dat", "Boat.dat", atd.
        delete p;
    }

    return 0;
}
