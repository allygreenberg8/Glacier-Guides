#include <iostream>
#include "TerrainParser.h"

int main() {
    std::cout << "Glacier Guides build is successful!" << std::endl;

    TerrainParser terrainParser;
    if (terrainParser.loadFile("map.osm"))
    {
        std::cout << "TerrainParser parsed" << terrainParser.getPoints().size() << " points from OpenSkiMap." << std::endl;
        std::cout << "TerrainParser parsed" << terrainParser.getTrails().size() << " trails from OpenSkiMap." << std::endl;
    }
    else
    {
        std::cerr << "TerrainParser failed to parse the OSM file from OpenSkiMap" << std::endl;
    }

    return 0;
}
