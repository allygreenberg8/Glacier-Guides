#include <iostream>
#include "TerrainParser.h"
#include "TerrainRenderer.h"

// SFML 3.0 module headers
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>



int main(){
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
    if (terrainParser.loadElevationFile("elevation.csv"))
    {
        std::cout << "TerrainParser parsed" << terrainParser.getElevation().size() << " points from OpenTopography." << std::endl;
    }
    else
    {
        std::cerr << "TerrainParser failed to parse the CSV file from OpenTopography" << std::endl;
    }

    TerrainRenderer terrainRenderer(terrainParser);

    // Create SFML window
    sf::win::VideoMode videoMode(800, 600);
    sf::gfx::RenderWindow mapWindow(videoMode, "Terrain Map");

    // Arbitrary test case path
    std::vector<int> examplePath = {10, 50, 60 , 80};
    terrainRenderer.choosePath(examplePath);

    while(mapWindow.isOpen())
    {
        sf::win::Event event;
        while(mapWindow.pollEvent(event))
        {
            if (event.type == sf::win::Event::Type::Closed)
            {
                mapWindow.close();
            }
        }
    }
    
    // Clear screen and render the terrain and the drawn path
    mapWindow.clear();
    terrainRenderer.renderTerrain(mapWindow);
    mapWindow.display();

    return 0;
}
    


















/*
#include <iostream>
#include "TerrainParser.h"
#include "TerrainRenderer.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

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
    // Error was there because 3.0 has different constructor syntax fo video mode
    sf::RenderWindow mapWindow(sf::VideoMode({800,600}), "Terrain Map");

    // Arbitrary test case path;
    std::vector<int> examplePath = {10, 50, 60 , 80};
    terrainRenderer.choosePath(examplePath);

    
    // Now write code in to render window for sfml using events
    while (mapWindow.isOpen())
    {
        // Not sure why errors here
        //try to rewrite with the swtich case style type of event handling
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
*/