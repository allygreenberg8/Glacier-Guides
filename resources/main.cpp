#include <iostream>
#include "TerrainParser.h"
#include "TerrainRenderer.h"
#include <SFML/Graphics.hpp>

int main()
 {
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

    // Initialize TerrainRenderer
    TerrainRenderer terrainRenderer(terrainParser);

    // Create SFML window
    // Unsure why SFML error
    sf::RenderWindow mapWindow(sf::VideoMode(800,600), "Terrain Map");
    
    // Arbitrary test case path;
    std::vector<double> examplePath = {10, 50, 60 , 80};
    terrainRenderer.choosePath(examplePath);

    // Now write code in to render window for sfml using events
    while (mapWindow.isOpen())
    {
        // Not sure why errors here
        sf::Event event;
        while(mapWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                mapWindow.close();
            }

            // Clear screen and render the terrain and the drawn path
            mapWindow.clear();
            terrainRenderer.renderTerrain(mapWindow);
            mapWindow.display();
        }
    }



    return 0;
}
