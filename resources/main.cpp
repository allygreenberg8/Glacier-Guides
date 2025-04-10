#include <iostream>
#include "TerrainParser.h"

int main() {
    std::cout << "Glacier Guides build is successful!" << std::endl;

    // Create terrain parser object
    TerrainParser terrainParser;
    // Check if terrain parsing for OpenSkiMap works correctly
    if (terrainParser.loadFile("map.osm"))
    {
        std::cout << "TerrainParser parsed" << terrainParser.getPoints().size() << " points from OpenSkiMap." << std::endl;
        std::cout << "TerrainParser parsed" << terrainParser.getTrails().size() << " trails from OpenSkiMap." << std::endl;
    }
    else
    {
        std::cerr << "TerrainParser failed to parse the OSM file from OpenSkiMap" << std::endl;
    }

    // Check if parsing for elevation data from OpenTopography works correctly
    if (terrainParser.loadElevationFile("elevation.csv"))
    {
        std::cout << "TerrainParser parsed" << terrainParser.getElevation().size() << " points from OpenTopography." << std::endl;
    }
    else
    {
        std::cerr << "TerrainParser failed to parse the CSV file from OpenTopography" << std::endl;
    }

    return 0;
}
